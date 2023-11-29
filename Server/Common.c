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


#include "Verificar_Argumentos_Server.h"
#include "Common.h"

typedef struct db_req db_request;


int isEmpty_db_request_list(db_request_list *self)
{
    db_request *aux = self->node;
    if(aux == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

db_request *get_db_request(db_request_list *pl, int id)
{
    db_request *aux = pl->node;
    int i = 0;
    while(aux != NULL)
    {
        /*if(aux->id == id)
        {
            return aux;
        }*/
        if(i == id)
        {
            return aux;
        }
        aux = aux->next;
        i++;
    }
    return NULL;
}

void remove_req_list_head(db_request_list *self)
{
    db_request *aux = self->node;
    //db_request *aux2 = self->node;
    if(aux == NULL)
    {
        printf("No hay requests en la lista\n");
        return;
    }
    /*while(aux2 != NULL)
    {
        aux2 = aux2->next;
    }*/
    self->node = aux->next;
    free(aux);
    return;
}