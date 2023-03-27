/* A simple server in the internet domain using TCP
   The port number is passed as an argument
   This version runs forever, forking off a separate
   process for each connection
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stddef.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mariadb/mysql.h>
#include "header.h"

// This function is called when a system call fails. It displays a message about the error on stderr and then aborts the program.
void error(const char *msg)
{
   perror(msg);
   exit(1);
}

int main(int argc, char *argv[])
{

   MYSQL *connection = mysql_init(NULL);
   mysql_real_connect(connection, "localhost", "pipo", "theclown", "mijndb", 0, NULL, 0);
   if (connection == NULL)
   {
      printf("Kan geen verbinding met de MariaDB server maken \n");
      exit(-1);
   }

   int sockfd, newsockfd, portno; /*sockfd and newsockfd variables store the values returned by the socket system call and the accept system call.
                                    portno stores the port number on which the server accepts connections.*/

   socklen_t clilen; // stores the size of the address of the client. This is needed for the accept system call.

   char buffer[256]; // The server reads characters from the socket connection into this buffer.

   struct sockaddr_in serv_addr, cli_addr; // structure containing an internet address. This structure is defined in netinet/in.h.

   int n; // n is the return value for the read() and write() calls; i.e. it contains the number of characters read or written.

   if (argc < 2) // This code displays an error message
   {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(1);
   }

   sockfd = socket(AF_INET, SOCK_STREAM, 0); /*The socket() system call creates a new socket. It takes three arguments.
                                               The first is the address domain of the socket.*/

   if (sockfd < 0)
   {
      error("ERROR opening socket");
   }

   bzero((char *)&serv_addr, sizeof(serv_addr)); /*The function bzero() sets all values in a buffer to zero. It takes two arguments,
                                                  the first is a pointer to the buffer and the second is the size of the buffer.
                                                   Thus, this line initializes serv_addr to zeros.*/

   portno = atoi(argv[1]); /* The port number on which the server will listen for connections is passed in as an argument,
                             and this statement uses the atoi() function to convert this from a string of digits to an integer. */

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);

   if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
   { // The bind() system call binds a socket to an address,
      error("ERROR on binding");
   }

   listen(sockfd, 5); // The listen system call allows the process to listen on the socket for connections.
   clilen = sizeof(cli_addr);

   while (1)
   {
      // The accept() system call causes the process to block until a client connects to the server.
      newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

      if (newsockfd < 0)
      {
         error("ERROR on accept");
      }

      bzero(buffer, 256);// initializes the buffer using the bzero() function, and then reads from the socket.
      n = read(newsockfd, buffer, 255);
      if (n < 0)
      {
         error("ERROR reading from socket");
      }

      printf("Here is the message: %s\n", buffer); // prints out what the client sent.

      if (strncmp(buffer, "F", 1) == 0)
      {
         if (mysql_query(connection, "UPDATE LED SET LEDstate = '1' WHERE ledID = 1;"))
         {
            printf("%s\n", mysql_error(connection));
         }
      }
      else
      {

         if (mysql_query(connection, "UPDATE LED SET LEDstate = '0' WHERE ledID = 1;"))
         {
            printf("%s\n", mysql_error(connection));
         }
      }

      n = write(newsockfd, "I got your message", 18);/*Once a connection has been established,
                                                       both ends can both read and write to the connection.*/
      if (n < 0)
         error("ERROR writing to socket");
      close(newsockfd);
   }

   close(sockfd);
   mysql_close(connection);
   return 0;
}