#include "Common.h"
#include "Funciones_Server.h"

void* Task(void * arg)
{
    struct local_threads_arg_struct *argumentos = arg;

    char recvline[MAXLINE+1];     //Buffer que recibe, se pone MAXLINE +1
    char aux[MAXLINE];          //Auxilar para borrar checksum y fin de mensaje
    long int bytes_lect;
    int num_conx;
    int ACK = 1;

    while((argumentos->thread_salida == 0) && (*(argumentos->salir) == 0))
    {
        if((argumentos->thread_salida == 0) && (*(argumentos->salir) == 0))
        {
            bytes_lect = 0;
            while(bytes_lect <= 0)
            {
                if((argumentos->thread_salida == 0) && (*(argumentos->salir) == 0))
                {
                    bytes_lect = recv(*(argumentos->socket_conx),recvline,MAXLINE-1,MSG_DONTWAIT);
                }
                else
                {
                    if(*(argumentos->socket_conx) != -1)
                    {
                        if(close(*(argumentos->socket_conx))<0)
                        {
                            printf("Soy el task: %d, hubo un error al cerrar la conexion %d.\n",argumentos->id,*(argumentos->socket_conx));
                            //printf("Errno:");
                        }
                        else
                        {
                            *(argumentos->socket_conx) = -1;
                        }
                    }
                    break;
                }
            }
        }
        if(!argumentos->thread_salida && *(argumentos->salir) == 0)
        {
            if(bytes_lect > 0)
            {
                if(!argumentos->thread_salida && *(argumentos->salir) == 0)
                {
                    pthread_mutex_lock(argumentos->lock);
                    argumentos->segs = 0;
                    pthread_mutex_unlock(argumentos->lock);
                }

                if(recvline[bytes_lect - 1] == '\n')
                {
                    char rawmsg[MAXLINE];
                    char t;
                    if((argumentos->thread_salida == 0) && (*(argumentos->salir) == 0))
                    {
                        t = get_tipo_mensaje(recvline,rawmsg);
                    }

                    if(t == 'c')
                    {
                        if((strcmp(recvline,"Tam recibido\n")) && (recvline[0] != '\0'))
                        {
                            strcpy(aux,recvline);
                            pthread_mutex_lock(argumentos->req_list_lock);
                            add_db_request(argumentos->list,db_request_list_getNextID(argumentos->list),argumentos->socket_conx,aux);
                            pthread_mutex_unlock(argumentos->req_list_lock);

                            for(int i = 0; i < 5; i++)
                            {
                                if(*(argumentos->ack_arg[i].conx_socket) == *(argumentos->socket_conx))
                                {
                                    num_conx = i;
                                }
                            }
                            
                            while(ACK)
                            {
                                if((argumentos->thread_salida == 0) && (*(argumentos->salir) == 0))
                                {
                                    pthread_mutex_lock(argumentos->ack_arg[num_conx].ack_lock);
                                    ACK = *(argumentos->ack_arg[num_conx].ack);
                                    pthread_mutex_unlock(argumentos->ack_arg[num_conx].ack_lock);
                                }
                                else
                                {
                                    break;
                                }
                            }
                            if(!ACK)
                            {
                                pthread_mutex_lock(argumentos->ack_arg[num_conx].ack_lock);
                                *(argumentos->ack_arg[num_conx].ack) = 1;
                                pthread_mutex_unlock(argumentos->ack_arg[num_conx].ack_lock);
                            }
                        }
                        pthread_mutex_lock(argumentos->lock);
                        argumentos->thread_salida = 1;
                        pthread_mutex_unlock(argumentos->lock);
                    }
                    if(t == 'b')
                    {
                        if((argumentos->thread_salida == 0) && (*(argumentos->salir) == 0))
                        {
                            strcpy(aux,recvline);
                            if(!strcmp(aux,"Tipo B | salir\n"))
                            {
                                pthread_mutex_lock(argumentos->lock);
                                argumentos->thread_salida = 1;
                                pthread_mutex_unlock(argumentos->lock);
                            }
                            pthread_mutex_lock(argumentos->req_list_lock);
                            add_db_request(argumentos->list, db_request_list_getNextID(argumentos->list), argumentos->socket_conx,aux);
                            pthread_mutex_unlock(argumentos->req_list_lock);
                        }
                    }
                    if(t == 'a')
                    {
                        if((argumentos->thread_salida == 0) && (*(argumentos->salir) == 0))
                        {
                            strcpy(aux,recvline);
                            pthread_mutex_lock(argumentos->req_list_lock);
                            add_db_request(argumentos->list, db_request_list_getNextID(argumentos->list), argumentos->socket_conx,aux);
                            pthread_mutex_unlock(argumentos->req_list_lock);
                        }
                        pthread_mutex_lock(argumentos->lock);
                        argumentos->thread_salida = 1;
                        pthread_mutex_unlock(argumentos->lock);
                    }
                    bytes_lect = 0;
                    memset(recvline,0,MAXLINE);
                }
            }
        }
        else
        {
            break;
        }
    }
    while(*(argumentos->socket_conx) != -1){}
    liberar_Handler(argumentos->Handlers,argumentos->id, argumentos->handler_lock);
    return NULL;
}