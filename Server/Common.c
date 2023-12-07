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

db_request_list *new_db_request_list()
{
    db_request_list *l = malloc(sizeof(db_request_list));
    if(!l)
    {
        printf("Error al asignar memoria\n");
        exit(EXIT_FAILURE);
    }
    l->node = NULL;
    return l;
}

void add_db_request(db_request_list *self, int id, int *conn, char *sendmsg)
{
    db_request *aux = (self->node);
    db_request *new;
    new = new_db_request(id,conn,sendmsg);
    if(aux == NULL)
    {
        self->node = new;
    }
    else
    {
        while(aux->next != NULL)
        {
            aux = aux->next;
        }
        aux->next = new;
    }
}

void remove_db_request(db_request_list *self, int id)
{
    db_request *current = self->node;
    db_request *aux;
    db_request *aux2;
    db_request *aux3;
    
    //int i = 0;

    if(id == 0)
    {
        self->node = self->node->next;
        free(current);
    }
    else
    {
        aux2 = current->next;
        aux = current;
        while(aux2 != NULL)
        {
            if(aux2->id == id)
            //if(i == id)
            {
                aux->next = aux2->next;
                aux3 = aux->next;
                while(aux3 != NULL)
                {
                    aux3->id--;
                    aux3 = aux3->next;
                }
                free(aux2);
                break;
            }
            else
            {
                aux = aux->next;
                aux2 = aux2->next;
                //i++;
            }
        }
    }
}

int db_request_list_getNextID(db_request_list *self)
{
    db_request *aux = self->node;
    if(self->node == NULL)
    {
        return 0;
    }
    else
    {
        while(aux->next != NULL)
        {
            aux = aux->next;
        }
        return aux->id + 1;
    }
}
