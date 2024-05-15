/*********************************************************************************
 *      Copyright:  (C) 2024 linuxer<linuxer@email.com>
 *                  All rights reserved.
 *
 *       Filename:  ds18b20.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(03/03/24)
 *         Author:  linuxer <linuxer@email.com>
 *      ChangeLog:  1, Release initial version on "03/03/24 20:48:42"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>

#define CHIP_SN_SIZE 128
int get_temperature(float *temperature);
/*    
int main(int argc, char *argv[])
{
	float                   temperature;
	int                     rv;
	char                    chip_sn[CHIP_SN_SIZE];
	if(get_temperature(&temperature,chip_sn) < 0)
	{
		printf("get temperrature failure: %d\n", rv);
	}
	printf("%s temperature: %.4fºC\n", chip_sn, temperature);

}*/

int get_temperature(float *temperature)
{
	int                    fd;
	char                   buf[128];
	char                  *ptr=NULL;
	float                  temp;
	int                    rv;
	DIR                   *dirp = NULL;
	char                   w1_path[64]="/sys/bus/w1/devices/";
	//char                   ds18b20_path[64];
	struct dirent         *direntp = NULL;
	char                   chip_sn[32];
	int                    found = 0;

    dirp = opendir(w1_path);
	if(!dirp)
	{
		printf("open foler %s failure: %s\n", w1_path, strerror(errno));
		return -1;
	}
	
	while((direntp = readdir(dirp)) != NULL )
	{
		if(strstr(direntp->d_name, "28-"))
		{
			strncpy(chip_sn,direntp->d_name,CHIP_SN_SIZE);
			found = 1;
		}
	}
	closedir(dirp);
	
	if(!found)
	{
		printf("Can not find ds18b20 chipset\n");
		return -2;
	}
	//snprintf(ds18b20_path, sizeof(ds18b20_path), "%s/%s/w1_slave",w1_path, chip_sn);
	strncat(w1_path, chip_sn,sizeof(w1_path)-strlen(w1_path));
	strncat(w1_path, "/w1_slave", sizeof(w1_path)-strlen(w1_path));
	//printf("w1_path: %s\n", w1_path);

	if((fd = open(w1_path, O_RDONLY)) < 0)
	{
		printf("open file failure: %s\n", strerror(errno))	;
		return -3;
	}
	
	memset(buf, 0, sizeof(buf));
	if((rv = read(fd, buf, sizeof(buf))) < 0)
	{
		printf("Read data failure: %s\n", strerror(errno));
		return -4;
	}
	//printf("%s\n", buf);
	ptr = strstr(buf, "t=");

	if(ptr == NULL)
	{
		printf("Can not find t= string\n");
		return -5;
	}
	ptr+=2;
	//printf("%s\n", ptr);
	temp = atof(ptr)/1000;
	*temperature = temp;
	//printf("temperature: %.4fºC\n", temp);
	close(fd);
	return 0;
}
