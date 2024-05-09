/*********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  connect_server.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(02/05/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "02/05/24 17:05:40"
 *                 
 ********************************************************************************/

#include<sys/types.h>
#include<sys/socket.h>
#include<errno.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdio.h>

int connect_server(int port,char *ser_ip);

int connect_server(int port,char *ser_ip)
{
	int                    conn_fd=-1;
	struct sockaddr_in     ser_addr;

	conn_fd=socket(AF_INET,SOCK_STREAM,0);
	if(conn_fd<0)
	{
		printf("create socket failure:%s\n",strerror(errno));
		return -2;
	}

	memset(&ser_addr,0,sizeof(ser_addr));
	ser_addr.sin_family=AF_INET;
	ser_addr.sin_port=htons(port);
	inet_aton(ser_ip,&ser_addr.sin_addr);

	if(connect(conn_fd,(struct sockaddr *)&ser_addr,sizeof(ser_addr))<0)
	{
		printf("connect to server failure:%s\n",strerror(errno));
		return -3;
	}

	printf("connect to server successfully\n");

	return conn_fd;
}
