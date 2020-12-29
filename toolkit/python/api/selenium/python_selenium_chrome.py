#! /usr/bin/env python
# coding=utf-8

from selenium import webdriver
from selenium.webdriver.support.ui import Select
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions
from selenium.webdriver.common.by import By
from selenium.common.exceptions import TimeoutException
from selenium.webdriver.common.action_chains import ActionChains

import random
import time

try: input = raw_input
except: pass

class MyBrowser(object):
    def __init__(self, url, exe_driver=''):
        print("url: %s" % url)
        print("driver: %s" % exe_driver)
        if exe_driver:
            self.driver = webdriver.Chrome(executable_path = exe_driver)
        else:
            self.driver = webdriver.Chrome()
        self.driver.set_page_load_timeout(5)
        self.driver.set_script_timeout(5)#这两种设置都进行才有效
        try:
            self.driver.get(url)
        except Exception as reason:
            print("error: %s" % reason)
            self.driver.execute_script('window.stop()')

    def quit(self):
        self.driver.quit()

    def wait_until_with_timeout(self, condition, timeout=5):
        try:
            WebDriverWait(self.driver, timeout).until(condition)
        except Exception as reason:
            print("WebDriverWait error: %s" % reason)
            return

    def random_sleep(self):
        time.sleep(random.randint(5, 10)/10.0)

    def switch_to_active_window(self):
        self.driver.switch_to.active_element

    def click(self, element):
        if element:
            element.click()

    def alert_click(self, accept=True):
        time.sleep(2)
        #WebDriverWait(self.driver,10).until(expected_conditions.alert_is_present())
        element = self.driver.switch_to.active_element
        element.click()
        return
        alert = self.driver.switch_to.alert #切到弹出框
        if alert:
            print(alert.text)
            if accept:
                alert.accept() #确定
            else:
                alert.dismiss() #取消

    def swipe_screen(self, start, dx, dy):
        # 模拟滑动屏幕
        if not start:
            return
        print("swipe_screen %s to: %d,%d" % (start.text, dx, dy))
        # 点击第一个点
        ActionChains(self.driver).move_to_element(start).click_and_hold().perform()
        time.sleep(0.2)
        # 拖动
        times = 0
        if times == 0:
            ActionChains(self.driver).move_by_offset(dx, dy).perform()
        for i in range(times):
            print("swipe_screen %s to: %d,%d" % (start.text, dx, dy))
            ActionChains(self.driver).move_by_offset(dx/times, dy/times).perform()
            time.sleep(1/times)
        # 松开鼠标
        ActionChains(self.driver).release().perform()

    def swipe_screen(self, start, end):
        if not start:
            return
        if not end:
            return
        self.swipe_screen(start, end.location['x'], end.location['y'])

    def hover(self, by, value):
        element = self.find_element(by, value)
        if element:
            ActionChains(self.driver).move_to_element(element).perform()
        return element

    def find_element(self, by, value):
        '''
        open url with chrome and right-click an element then select "Inspect"
        go to "Elements" window and right-click the selected source code
        then select "Copy", "Copy selector" or "Copy XPath" or "Copy full XPath"
        '''
        print("find element by:%s, value:%s" % (by, value))
        try:
            if (by == "id"):
                element = self.driver.find_element_by_id(value)
            elif (by == "name"):
                element = self.driver.find_element_by_name(value)
            elif (by == "xpath"):
                #"Copy XPath" or "Copy full XPath"
                element = self.driver.find_element_by_xpath(value)
            elif (by == "classname"):
                element = self.driver.find_element_by_class_name(value)
            elif (by == "css"):
                #"Copy selector"
                element = self.driver.find_element_by_css_selector(value)
            elif (by == "link_text"):
                #should be included by tag <a></a>
                element = self.driver.find_element_by_link_text(value)
            else:
                print("unknown method:%s, please check" % by)
                return None
        except:
            return None
        if element:
            ""
            #print("tag:%s, text:%s, loc:%s, size:%s" % (element.tag_name, element.text, element.location, element.size))
        return element

def open():
    chrome_driver = 'C:\Program Files (x86)\Google\Chrome\Application\chromedriver.exe'
    url = 'https://www.baidu.com'

    # create browser
    browser = MyBrowser(url, chrome_driver)
    condition = expected_conditions.title_is("百度一下，你就知道")
    browser.wait_until_with_timeout(condition)

    return browser

def login(browser):
    username = 'test'
    password = '123456'

    # btn_login selector: #u1 > a
    btn_login = browser.find_element('css', '#u1 > a')
    browser.click(btn_login)
    time.sleep(1)

    # btn_longin_by_username xpath: //*[@id="TANGRAM__PSP_11__footerULoginBtn"]
    by, value = locator = ('xpath', r'//*[@id="TANGRAM__PSP_11__footerULoginBtn"]')
    condition = expected_conditions.visibility_of_element_located(locator)
    browser.wait_until_with_timeout(condition)
    btn_longin_by_username = browser.find_element(by, value)
    print(btn_longin_by_username.text)
    btn_longin_by_username.click()

    # input_username xpath: //*[@id="TANGRAM__PSP_11__userName"]
    by, value = locator = ('xpath', r'//*[@id="TANGRAM__PSP_11__userName"]')
    condition = expected_conditions.visibility_of_element_located(locator)
    browser.wait_until_with_timeout(condition)
    input_username = browser.find_element(by, value)
    input_username.send_keys(username)

    # input_password xpath: //*[@id="TANGRAM__PSP_11__password"]
    input_password = browser.find_element('xpath', r'//*[@id="TANGRAM__PSP_11__password"]')
    input_password.send_keys(password)

    # btn_login xpath: //*[@id="TANGRAM__PSP_11__submit"]
    btn_login = browser.find_element('xpath', r'//*[@id="TANGRAM__PSP_11__submit"]')
    print(btn_login.text)
    btn_login.click()

    time.sleep(20)
    #browser.quit()

if __name__ == '__main__':
    browser = open()
    login(browser)

