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
#include <sqlite3.h>

#include "Server_IPv4.h"
#include "Verificar_Argumentos_Server.h"
#include "Config_Socket_IPv4.h"
#include "Handler.h"
#include "Common.h"
#include "Funciones_DB.h"


int dir_IPv4_valida(char *ipAddr)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET,ipAddr, &(sa.sin_addr));
    return result != 0;
}

void ocupar_handler(int *Handlers, int i, pthread_mutex_t *lock)
{
    pthread_mutex_lock(lock);
    Handlers[i] = 0;
    pthread_mutex_unlock(lock);
}

void liberar_Handler(int *Handlers, int i, pthread_mutex_t *lock)
{
    pthread_mutex_lock(lock);
    Handlers[i] = 1;
    pthread_mutex_unlock(lock);
}


int get_prim_hand_disp(int *Handlers, long unsigned int maxHandlers)
{
    for(unsigned long int i = 0; i < maxHandlers; i++)
    {
        if(Handlers[i])
        {
            return (int)i;
        }
    }
    return -1;
}

int get_cant_hand_disp(int *Handlers, long unsigned int maxHandlers)
{
    int amount = 0;
    for(unsigned long int i = 0; i < maxHandlers; i++)
    {
        if(Handlers[i])
        {
            amount++;
        }
    }
    return amount;
}

char get_tipo_mensaje(char *string, char *msg)
{
    char tipo[MAXLINE];
    char query[MAXLINE];
    int i = 0;
    int j = 0;
    int parte = 0;

    memset(msg,0,MAXLINE);
    memset(query,0,MAXLINE);

    while(string[i != '\n'])
    {
        if((string[i+2] != '\n') && (string[i] == ' ') && (string[i+1] == '|') && (string[i+2] == ' '))
        {
            parte = 1;
        }
        if(parte == 1)
        {
            break;
        }
        if(parte == 0)
        {
            tipo[i] = string [i];
        }
        i++;
    }

    i += 3;
    j = 0;
    while(string[i] != '\n')
    {
        query[j] = string[i];
        i++;
        j++;
    }
    strcpy(msg,query);
    if(!strcmp(tipo, "Tipo A"))
    {
        return 'a';
    }
    if(!strcmp(tipo, "Tipo B"))
    {
        return 'b';
    }
    if(!strcmp(tipo, "Tipo C"))
    {
        return 'c';
    }
    return 'd';
}

