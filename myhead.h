/********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  myhead.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(02/05/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "02/05/24 18:48:50"
 *                 
 ********************************************************************************/

void print_usage(char *progname);
int get_time_ms(char *buff, int len);
int get_temperature(float *temper);
int connect_server(int port,char *ser_ip);
