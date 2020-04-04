#include <iostream>

#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core/everything.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	setWindowIcon(QIcon(":/img/logo.ico"));
    ui->setupUi(this);
    globaldbLoaded = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    cout << "About: 2019-02-15" << endl;
    QMessageBox::about(NULL, "About", "Everything is a search engine on Linux that quickly locates files and folders based on file names.\ndevelop by C++/Qt on Ubuntu;\ncreated by longbin.li@aptiv.com on 2019-02-15");
}

void MainWindow::on_actionQuit_2_triggered()
{
    cout << "quit" << endl;
    exit(0);
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    string line = arg1.toStdString();
    QString qstr;
    vector<string> findkey;
    vector<string>::iterator it;
    cout << "lineEdit changed: " << line << endl;

    if (0 == line.length())
    {
        return;
    }
	if (3 > line.length())
	{
		return;
	}
    findkey = search(line);
    if (!findkey.size())
    {
        cout << "no match: " << line << endl;
        return;
    }
    cout << "match: " << line << endl;
    ui->textBrowser->setText("");

    vector<string> flist = getflist(findkey);
    vector<string>::iterator fit;
    vector<string> attr;
    vector<string>::iterator itattr;
	string filename = "";
# if 0
    for (fit = flist.begin(); fit != flist.end(); fit++)
	{
		filename = *fit;
		prettyPrint(filename);
	}
# else
	int idx = 0;
    string str = "";
	size_t max_filename_len = 0;
	for (fit = flist.begin(); fit != flist.end(); fit++)
	{
		filename = *fit;
		filename = filename.substr(filename.find_last_of("/")+1);
		if (max_filename_len < filename.size())
		{
			max_filename_len = filename.size();
		}
	}
	if (max_filename_len > 40)
	{
		max_filename_len = 40;
	}
    for (fit = flist.begin(); fit != flist.end(); fit++)
    {
		// cout << "list: " << *fit << endl;
        str = "";
		idx = 0;
		filename = *fit;
        attr = getattr(filename);
		filename = filename.substr(filename.find_last_of("/")+1);
        for (itattr = attr.begin(); itattr != attr.end(); itattr++, idx++)
        {
            str.append(*itattr);
			if ((1 == idx)
					&& (max_filename_len > filename.size()))
			{
				str.append(max_filename_len - (*itattr).size(), ' ');
			}
            str.append(" ");
        }
        //str.append("\n");

        qstr = QString::fromStdString(str);
        ui->textBrowser->append(qstr);
# endif
    }
}
void MainWindow::winloaddb(bool force)
{
	int ret = 0;
    if (force)
    {
        globaldbLoaded = false;
    }
    if (!globaldbLoaded)
    {
        globaldbLoaded = true;
        ret = dbload();
		if (0 == ret)
		{
			cout << "dbload ok." << endl;
		}
		else
		{
			cout << "dbload failed." << endl;
			QMessageBox::warning(this, "dbload failed.", "updatedb first", QMessageBox::Ok);
		}
    }
}

void MainWindow::winupdatedb()
{
    dbinit();
    cout << "dbsave ok." << endl;
    dbsave();
    cout << "dbinit ok." << endl;
    winloaddb(true);
}

void MainWindow::on_actionSetting_triggered()
{
    winupdatedb();
}

void MainWindow::on_actionSetting_2_triggered()
{
    winloaddb(true);
}

void MainWindow::on_lineEdit_returnPressed()
{
    ui->textBrowser->setFocus();
}
