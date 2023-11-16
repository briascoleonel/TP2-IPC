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


#include "Common.h"


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



