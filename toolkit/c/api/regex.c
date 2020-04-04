/*****************************************
 * Copyright (C) 2018 * Ltd. All rights reserved.
 * 
 * File name   : regex.c
 * Author      : longbin
 * Created date: 2018-03-28 21:52:12
 * Description : 
 *
 *******************************************/

#include <stdio.h>
#include <string.h>
#include <string.h>
#include <regex.h>

// 提取子串
char *getsubstr(char *dst, const char *src, regmatch_t *pmatch)
{
	if (NULL == src || NULL == dst || NULL == pmatch) {
		return NULL;
	}
	size_t length = 0;

	length = pmatch->rm_eo - pmatch->rm_so;
	memcpy(dst, src + pmatch->rm_so, length);
	dst[length] = '\0';

	return dst;
}

char *search(regex_t *preg, const char *str, int cflags)
{
	return NULL;
}

char *findall(char *str, size_t start, size_t end)
{
	return NULL;
}

char *sub(regex_t *preg, char *prepl, char *str, size_t count)
{
	return NULL;
}

int main(void)
{
	regmatch_t match;
	regex_t reg;
	// 正则表达式
	const char *pattern = "[a-z]+";
	// 待搜索的字符串
	char str[] = "HELLOsaiYear2012@gmail.com";
	char dst[256] = {0};
	char *p = NULL;

	printf("%s\n", str);
	printf("01234567890123456789012345\n");
	//编译正则表达式
	/* int regcomp(regex_t *preg, const char *pattern, int cflags) */
	regcomp(&reg, pattern, REG_EXTENDED);
	int offset = 0;
	int status = 0;
	while (offset < strlen(str)) {
		/*  int regexec(regex_t *preg, char *str, size_t nmatch, regmatch_t pmatch[], int eflags) */
		status = regexec(&reg, str + offset, 1, &match, 0);
		/* 匹配正则表达式，注意regexec()函数一次只能匹配一个，不能连续匹配 */
		if (status == REG_NOMATCH) {
			printf("No Match\n");
		} else if (match.rm_so != -1) {
			printf("Match: ");
			p = getsubstr(dst, str + offset, &match);
			printf("[%d, %d]: %s\n", offset + match.rm_so, offset + match.rm_eo, p);
		}
		offset += match.rm_eo;
	}

	//释放正则表达式
	//void regfree(regex_t *preg);
	regfree(&reg);

	return 0;
}

