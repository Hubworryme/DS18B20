/*********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  commit_tem.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(02/05/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "02/05/24 17:27:33"
 *                 
 ********************************************************************************/

#include<stdio.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>
#include<syslog.h>
#include<time.h>
#include<sys/time.h>
#include<netdb.h>
#include<unistd.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<errno.h>
#include<signal.h>
#include<netinet/in.h>
#include<getopt.h>
#include<dirent.h>
#include "myhead.h"

void sig_stop(int signum);

int g_stop=0;

int main(int argc,char **argv)
{
	int                rv=-1;
	int                daemon_run=0;
	int                opt;
	int                idx;
	int                s_time=0;
	int                port;
	int                log_fd;
	int                sock_fd=-1;
	char               *ser_ip;
	char               buf[512];
	char               date_time[32];
	char               *id="master";
	char               *hostname;
	float              temper;
	struct hostent     *getname=NULL;

	struct option    opts[]={
		{"daemon",no_argument,NULL,'d'},
		{"time",required_argument,NULL,'t'},
		{"ip",required_argument,NULL,'i'},
		{"port",required_argument,NULL,'p'},
		{"help",no_argument,NULL,'h'},
		{NULL,0,NULL,0}
	};

	while((opt=getopt_long(argc,argv,"dt:i:p:h",opts,&idx))!=-1)
	{
		switch(opt)
		{
			case'd':
				    daemon_run=1;
				    break;
			case't':
				    s_time=atoi(optarg);
				    break;
			case'i':
				    hostname=optarg;
				    break;
			case'p':
				    port=atoi(optarg);
				    break;
			case'h':
				    print_usage(argv[0]);

			return 0;
		}
	}

	if(!port||!hostname)
	{
		print_usage(argv[0]);
		return 0;
	}

	getname=gethostbyname(hostname);
	if(getname==NULL)
	{
		printf("get hostname failure:%s\n",strerror(h_errno));
		return 0;
	}

	ser_ip=inet_ntoa(*(struct in_addr *)getname->h_addr);
	if(ser_ip==NULL)
	{
		printf("get ser_ip failure:%s\n",strerror(errno));
		return 0;
	}
	printf("ser_ip[%s]\n",ser_ip);

	if(daemon_run)
	{
		printf("program %s is running at background now!\n",argv[0]);

		log_fd=open("receive_temper.log",O_CREAT|O_RDWR,0666);
		if(log_fd<0)
		{
			printf("open the logfile failure:%s\n",strerror(errno));
			return 0;
		}

		dup2(log_fd,STDOUT_FILENO);
		dup2(log_fd,STDERR_FILENO);

		if((daemon(1,1))<0)
		{
			printf("daemon failure:%s\n",strerror(errno));
			return 0;
		}
	}

	signal(SIGTERM,sig_stop);
	signal(SIGPIPE,SIG_IGN);

	memset(buf,0,sizeof(buf));
	while(!g_stop)
	{
		if((get_temperature(&temper))<0)
		{
			printf("get temperature failure!\n");
			continue;
		}

		get_time_ms(date_time,sizeof(date_time));

		memset(buf,0,sizeof(buf));

		snprintf(buf,sizeof(buf),"%s/%s/%.3f%c",id,date_time,temper,'C');

		if(sock_fd<0)
		{
			if((sock_fd=connect_server(port,ser_ip))<0)
			{
				printf("try to connect to server again!\n");
				continue;
			}
		}

		if(sock_fd>=0)
		{
			rv=write(sock_fd,buf,sizeof(buf));
			if(rv>=0)
			{
				printf("message has send to server successfully!\n");
			}
			else
			{
				printf("send message to server failure:%s\n",strerror(errno));
				close(sock_fd);
				sock_fd=-1;
			}
		}

		sleep(s_time);
	}

	close(sock_fd);

	return 0;
}

void print_usage(char *progname)
{
	printf("-d(--daemon):let program run in the background.\n");
	printf("-p(--port):set server port.\n");
	printf("-h(--help):print the help information.\n");
	printf("-t(--time):set samping time.\n");
	printf("-i(--ip):set the ip of server.\n");

	return ;
}

void sig_stop(int signum)
{
	if(SIGUSR1==signum)
	{
		g_stop=1;
	}

	return ;
}
