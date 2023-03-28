#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <mariadb/mysql.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int *ledcheck(MYSQL *connection);
void ledcontrol(int *columnvalue);
// #define test

typedef struct stoplight
{
	int red;
	int yellow;
	int green;
}STOPLIGHT;


int main(int ac, char **ap) {

	// struct stoplight stoplight1[]= {{21, 20, 16},
    //                                 {22, 27, 17}};

#ifdef test

 	printf("\nstoplight 1 is %d,%d,%d\n", stoplight1[0].red, stoplight1[0].yellow, stoplight1[0].green);
    printf("\nstoplight 2 is %d,%d,%d\n", stoplight1[1].red, stoplight1[1].yellow, stoplight1[1].green);
#endif

	MYSQL *connection = mysql_init(NULL);
	mysql_real_connect(connection, "localhost", "pipo", "theclown", "mijndb", 0, NULL, 0);
	if(connection == NULL) {
		printf("Kan geen verbinding met de MariaDB server maken \n");
	exit(-1);
	}
	
	int file_descriptor;

	file_descriptor = open("/sys/class/gpio/export", O_WRONLY);
	write(file_descriptor, "21", 2);
	close(file_descriptor);
	sleep(1);
	
	file_descriptor = open("/sys/class/gpio/export", O_WRONLY);
	write(file_descriptor, "20", 2);
	close(file_descriptor);
	sleep(1);
	
	file_descriptor = open("/sys/class/gpio/export", O_WRONLY);
	write(file_descriptor, "16", 2);
	close(file_descriptor);
	sleep(1);
	
	file_descriptor = open("/sys/class/gpio/export", O_WRONLY);
	write(file_descriptor, "22", 2);
	close(file_descriptor);
	
	sleep(1);file_descriptor = open("/sys/class/gpio/export", O_WRONLY);
	write(file_descriptor, "17", 2);
	close(file_descriptor);
	
	sleep(1);file_descriptor = open("/sys/class/gpio/export", O_WRONLY);
	write(file_descriptor, "27", 2);
	close(file_descriptor);
	sleep(1);
	
	// sets pins as output
	file_descriptor = open("/sys/class/gpio/gpio21/direction", O_WRONLY);
	write(file_descriptor, "out", 3);
	close(file_descriptor);
	
	file_descriptor = open("/sys/class/gpio/gpio20/direction", O_WRONLY);
	write(file_descriptor, "out", 3);
	close(file_descriptor);
	
	file_descriptor = open("/sys/class/gpio/gpio16/direction", O_WRONLY);
	write(file_descriptor, "out", 3);
	close(file_descriptor);
	
	file_descriptor = open("/sys/class/gpio/gpio22/direction", O_WRONLY);
	write(file_descriptor, "out", 3);
	close(file_descriptor);
	
	file_descriptor = open("/sys/class/gpio/gpio27/direction", O_WRONLY);
	write(file_descriptor, "out", 3);
	close(file_descriptor);
	
	file_descriptor = open("/sys/class/gpio/gpio17/direction", O_WRONLY);
	write(file_descriptor, "out", 3);
	close(file_descriptor);
	


	

	while(1){

		int * columnvalue =  ledcheck(connection);
		ledcontrol(columnvalue);

		free(columnvalue);

		sleep(5);

	}
	

	
	file_descriptor = open("/sys/class/gpio/unexport", O_WRONLY);
	write(file_descriptor, "21", 2);
	close(file_descriptor);
	sleep(1);


	file_descriptor = open("/sys/class/gpio/unexport", O_WRONLY);
	write(file_descriptor, "22", 2);
	close(file_descriptor);
	sleep(1);

		mysql_close(connection);
	file_descriptor = open("/sys/class/gpio/unexport", O_WRONLY);
	write(file_descriptor, "27", 2);
	close(file_descriptor);
	sleep(1);


	file_descriptor = open("/sys/class/gpio/unexport", O_WRONLY);
	write(file_descriptor, "16", 2);
	close(file_descriptor);
	sleep(1);

		mysql_close(connection);
	file_descriptor = open("/sys/class/gpio/unexport", O_WRONLY);
	write(file_descriptor, "17", 2);
	close(file_descriptor);
	sleep(1);


	file_descriptor = open("/sys/class/gpio/unexport", O_WRONLY);
	write(file_descriptor, "20", 2);
	close(file_descriptor);
	sleep(1);

	
	mysql_close(connection);
	return 0;
}

int *ledcheck(MYSQL *connection){

	int *columnvalue = malloc(6*sizeof(int));
	int rows = 0;
	// int columnvalue[rows];
		// checks the value of the column LEDstate
	if (mysql_query(connection, "select LEDstate from LED LIMIT 6")) {
    	printf("%s\n", mysql_error(connection));
	}
	MYSQL_ROW row;

	//stores the results in the pointer resutl
	MYSQL_RES *result = mysql_use_result(connection);
	if(result == NULL){
		printf("%s\n", mysql_error(connection));
	}
	// fetches the results in row 
	while((row = mysql_fetch_row(result)))
	{
		for (int j = 0; j < 6; j++)
		{
			columnvalue[rows] = atoi(row[0]);
		}
			rows++;

		
		mysql_data_seek(result,0);
	}
		
#ifdef test
	for (int i = 0; i < 6; i++)
	{	
					
		printf("column value is: %d\n", columnvalue[i]);
	}
#endif


	mysql_free_result(result);// frees the results

	return columnvalue;
}

void gpiowrite(int pin, int hor){
    
	int fd;
	char path[50];
	sprintf (path, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if(fd == -1){

		fprintf(stderr,"faild to open directory.");
	}

	char value[5];
	sprintf(value,"%d",hor);
    write(fd,  value, 1);


	close(fd);
}



void ledcontrol(int *columnvalue){
	
	struct stoplight stoplight1[]= {{21, 20, 16},
                                    {22, 27, 17}};

	int *column = malloc(6*sizeof(columnvalue));

	for (int c = 0; c < 6; c++)
	{
		column[c] = columnvalue[c];
		printf("colllllllll is %d\n", column[c]);
		
	}
	printf("\n");

#ifdef test
	for (int t = 0; t < 6; t++)
	{
		printf("colllllllll is %d\n", column[c]);
	}
	
#endif

	int file_descriptor;
	if(column[0] == 1){

		//if columnvalue is other than 1 sets pin21 valuse to 1
		gpiowrite(stoplight1[0].red,1);

	}else if(column[0] != 1){
	
		//if columnvalue is other than 1 sets pin21 valuse to 0
		gpiowrite(stoplight1[0].red,0);
	}
	if(column[1] == 1){

		//if columnvalue is equal to 1, then sets pin17 to 1
		gpiowrite(stoplight1[1].green,1);

	}else if(column[1] != 1){
	
		//if columnvalue is other than 1 sets pin17 valuse to 0
		gpiowrite(stoplight1[1].green,0);
		
	}
	if(column[2] == 1){

		//if columnvalue is equal to 1, then sets pin20 to 1.
		gpiowrite(stoplight1[0].yellow,1);
			
	}else if(column[2] != 1){
	
		//if columnvalue is other than 1 sets pin20 valuse to 0
		gpiowrite(stoplight1[0].yellow,0);
	}
	if(column[3] == 1){

		//if columnvalue is equal to 1, then sets pin27 to 1.
		gpiowrite(stoplight1[1].yellow,1);

	}else if(column[3] != 1){
	
		//if columnvalue is other than 1 sets pin27 valuse to 0
		gpiowrite(stoplight1[1].yellow,0);
		
	}
	if(column[4] == 1){

		//if columnvalue is equal to 1, then sets pin16 to 1.
		gpiowrite(stoplight1[0].green,1);
		
	}else if(column[4] != 1){
	
		//if columnvalue is other than 1 sets pin16 valuse to 0

		gpiowrite(stoplight1[0].green,0);
		
	}

	if(column[5] == 1){

		//if columnvalue is equal to 1, then sets pin22 to 1.

		gpiowrite(stoplight1[1].red,1);
			
		
		
	}else if(column[5] != 1){
	
		//if columnvalue is other than 1 sets pin22 valuse to 0
		gpiowrite(stoplight1[1].red,0);
		
	}
}