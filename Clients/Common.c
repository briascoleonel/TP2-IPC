#include <sys/socket.h>             //Definiciones basicas de sockets
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>                 //Para funciones con argumentos var
#include <errno.h>
#include <sys/time.h>
#include <stdarg.h>
#include <pthread.h>          //Para funciones thread
#include <ctype.h>
#include <netinet/in.h>             //Para struct de in
#include <net/if.h>                 //Network interfaces
#include <sys/un.h>
#include <fcntl.h>


#include "Common.h"

#define FILEPATH "test.db"


int dir_IPv4_valida(char *ipAddr)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET,ipAddr, &(sa.sin_addr));
    return result != 0;
}

int dir_IPv6_valida(char *ipAddr)
{
    struct sockaddr_in6 sa;
    int result = inet_pton(AF_INET6,ipAddr, &(sa.sin6_addr));
    return result != 0;
}

int filename_valido(char *string) 
{
    //Controla la existencia de los siguientes caracteres: \/:*?"<>|
    if((strchr(string,(int)'/') != NULL) || (strchr(string,(int)':') != NULL) || (strchr(string,(int)'*') != NULL) || (strchr(string,(int)'?') != NULL) || 
    (strchr(string,(int)'<') != NULL) || (strchr(string,(int)'>') != NULL) || (strchr(string,(int)'|') != NULL))
    {
        return 0;
    }
    return 1;
}

int get_input(char *string, int max)
{
    if(!fgets(string,max,stdin))
    {
        printf("Error al leer el string introducido.\n");
        exit(EXIT_FAILURE);
    }
    if(!strchr(string,'\n'))
    {
        char c;
        do
        {
            c = (char)getchar();
        }
        while(c != '\n' && c != EOF);
        return -1;
    }
    else
    {
        return 0;
    }
}

void descargar_file(int *socket, unsigned long int tam_file)
{
    char recvline[MAXLINE];
    long int bytes_rcv = 0;
    long int total_bytes_rcv = 0;
    long int bytes_esc = 0;
    unsigned long int TAM = tam_file;

    int target = open(FILEPATH,O_WRONLY|O_CREAT|O_TRUNC,0644);
    if(target == -1)
    {
        printf("Error al crear/leer el archivo.\n");
        exit(EXIT_FAILURE);
    }

    while(TAM != 0)
    {
        memset(recvline,0,MAXLINE);

        bytes_rcv = read(*(socket),recvline,MAXLINE);
        TAM -= (unsigned long int)bytes_rcv;
        total_bytes_rcv += bytes_rcv;

        bytes_esc += write(target,recvline,(unsigned long int)bytes_rcv); 
    }

    if((close(target) < 0))
    {
        printf("Error al cerrar.\n");
        exit(EXIT_FAILURE);
    }
}



