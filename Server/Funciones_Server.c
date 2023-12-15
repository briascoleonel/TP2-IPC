#include "Common.h"
#include "Estructuras.h"

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

int get_prim_hand_disp(int *Handlers, long unsigned int maxHandlers, pthread_mutex_t *lock)
{
    int prim = -1;    
    int encontrado = 0;
    for(unsigned long int i = 0; i < maxHandlers; i++)
    {
        if(!encontrado)
        {
            pthread_mutex_lock(&lock[i]);
            if(Handlers[i])
            {
                prim = (int)i;
                encontrado = 1;
            }
            pthread_mutex_unlock(&lock[i]);
        }

    }
    return prim;
}

int get_cant_hand_disp(int *Handlers, long unsigned int maxHandlers, pthread_mutex_t *lock)
{
    int amount = 0;
    for(unsigned long int i = 0; i < maxHandlers; i++)
    {
        pthread_mutex_lock(&lock[i]);
        if(Handlers[i])
        {
            amount++;
        }
        pthread_mutex_unlock(&lock[i]);
    }
    return amount;
}

char get_tipo_mensaje(char *string, char *msg)
{
    char tipo[MAXLINE];
    char query[MAXLINE];
    int i = 0;
    int parte = 0;

    while(string[i] != '\n')
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
    int j = 0;
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

