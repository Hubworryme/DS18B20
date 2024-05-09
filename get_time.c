/*********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  get_time.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(02/05/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "02/05/24 16:39:51"
 *                 
 ********************************************************************************/
/* 
void get_time(char *date_time);

void get_time(char *date_time)
{
	time_t       time1;
	struct   tm  *p;

	time(&time1);
	p=gmtime(&time1);
	snprintf(date_time,32,"%d/%d/%d %d:%d:%d",1900+p->tm_year,1+p->tm_mon,p->tm_mday,(p->tm_hour+8),p->tm_min,p->tm_sec);

	return ;
}
*/
/* 
void get_time(char *timen);
void get_time(char *timen)
{
	time_t now;
	now=time(NULL);
	timen=ctime(&now);
	return ;
}*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
 
// *  * 获取系统时间，这种方式获取得到的格式是： Wed Mar 30 20:41:21 2022
int printf_time(void)
{
	time_t timep;
	time(&timep);
	char *s = ctime(&timep);
	printf("date:%s",s);
	return 0;
}
 
// *  * 获取系统时间，这种方式获取得到的格式是： 2022-03-30 20:41:21.104
int get_time_ms(char *buff, int len)
{
	struct timeval tv;
	struct tm* ptm;
	char time_string[40];
	long milliseconds;
					
	if(buff == NULL)
	{
		printf("%s buff is NULL.\n", __func__);
		return -1;
	}
						 
	gettimeofday(&tv, NULL);
							 
	ptm = localtime (&(tv.tv_sec));
								 
	strftime (time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", ptm);  //输出格式为: 2022-03-30 20:38:37
	milliseconds = tv.tv_usec / 1000;
	snprintf (buff, len, "%s.%03ld", time_string, milliseconds);            //输出格式为: 2022-03-30 20:38:37.182
    return 0;
}
