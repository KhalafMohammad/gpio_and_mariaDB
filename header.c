#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <mariadb/mysql.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


void gpiowrite(int pin, int hor){

	int fd;
	char path[50];
	sprintf (path, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if(fd == -1){

		fprintf(stderr,"faild to open directory write.");
	}

	char value[5];
	sprintf(value,"%d",hor);
    write(fd,  value, 1);


	close(fd);
}

void gpioex(int pin){

    int fd;
    char path[56];
    sprintf(path,"/sys/class/gpio/export");
    fd = open(path, O_WRONLY);
    if(fd == -1){

		fprintf(stderr,"faild to open directory export.");
	}

    char value[16];
    sprintf(value, "%d", pin);
    write(fd, value, 2);
    close(fd);

}

void gpiounex(int pin){


    int fd;
    char path[56];
    sprintf(path,"/sys/class/gpio/unexport");
    fd = open(path, O_WRONLY);
    if(fd == -1){

		fprintf(stderr,"faild to open directory unex.");
	}

    char value[16];
    sprintf(value, "%d", pin);
    write(fd, value, 2);
    close(fd);
}

static int gpiodir(int pin, int di){

    static const char s_directions_str[]  = "in\0out";
    int fd;
	char path[50];
	sprintf (path, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if(fd == -1){

		fprintf(stderr,"faild to open directory dir.");
	}

    write(fd, &s_directions_str[0 == di ? 0 :3], 0 == di ? 2: 3);


	close(fd);
    return 0;
}