#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <mariadb/mysql.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "header.c"


void gpiowrite(int pin, int hor);
static int gpiodir(int pin, int di);
void gpioex(int pin);
void gpiounex(int pin);