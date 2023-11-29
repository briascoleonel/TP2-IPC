#include "Common.h"
#include "Funciones_Server.h"

void *Thread_Conex_Codigo(void *arg)
{
    int connfd;                                     
    struct sockaddr_in server_addr;                
    struct conx_arg_struct *arguments = arg;                      

    char env_msg[MAXLINE];
    char recvline[MAXLINE];
    char recvline2[MAXLINE];

    //Variables para enviar
    long unsigned int cant_bytes_env;
    long int cant_bytes_recv;
    long int escr_ret_val;
    long int cant_bytes_recv2;
    int vacio = 1;

    if((connfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        printf("Error al crea socket.\n");
        exit(EXIT_FAILURE);
    }

    bzero(&server_addr, sizeof(server_addr));       //Establece los primeros n bytes del área de bytes a partir de s en cero
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(arguments->IPV4_iport);            //Convierte los bytes a estandar de bytes de red

    //Crea una estructura de dirección de red
    if(inet_pton(AF_INET,arguments->IPV4_Server_Address, &server_addr.sin_addr) <= 0)
    {
        printf("Fallo al convertir direccion IP  a binario\n");
        exit(EXIT_FAILURE);
    }

    //Conexion con el server
    if(connect(connfd, (SA*) &server_addr, sizeof(server_addr)) < 0)
    {
        printf("Falla al conectar con el servidor de conexion %d.\nAsegurese de que el server este funcionando.\n", arguments->id);
        exit(EXIT_FAILURE);
    }

    while(*(arguments->salir) == 0 )
    {
        pthread_mutex_lock(arguments->list_lock);
        vacio = isEmpty_db_request_list(arguments->list);
        pthread_mutex_unlock(arguments->list_lock);

        while(vacio)
        {
            if(*(arguments->salir) == 0)
            {
                pthread_mutex_lock(arguments->list_lock);
                vacio = isEmpty_db_request_list(arguments->list);
                pthread_mutex_unlock(arguments->list_lock);
            }
            else
            {
                break;
            }
        }

        if(*(arguments->salir) == 0)
        {
            db_request *req = malloc(sizeof(db_request));
            char t = 'd';

            pthread_mutex_lock(arguments->list_lock);
            req = get_db_request(arguments->list,0);
            pthread_mutex_unlock(arguments->list_lock);
            if(req != NULL)
            {
                t = get_tipo_mensaje(req->env_msg,env_msg);
                memset(env_msg,0,MAXLINE);
                memset(env_msg,req->env_msg);
            }
            else
            {
                exit(0);
            }
            if((t == 'a') || (t == 'b'))
            {
                if(strcmp(env_msg, "Tipo B | salir\n"))
                {
                    cant_bytes_env = strlen(env_msg);
                    escr_ret_val = 0;
                    while(escr_ret_val <= 0)
                    {
                        if(*(arguments->salir) == 0)
                        {
                            escr_ret_val = send(connfd,env_msg,cant_bytes_env,MSG_DONTWAIT);
                        }
                        else
                        {
                            break;
                        }
                    }

                    memset(recvline,0,MAXLINE);
                    cant_bytes_recv = 0;
                    while((recvline[cant_bytes_recv-1] != '\n') && (recvline[cant_bytes_recv-2] != '\r'))
                    {
                        if(*(arguments->salir) == 0)
                        {
                            while(cant_bytes_recv <= 0)
                            {
                                if(*(arguments->salir) == 0)
                                {
                                    cant_bytes_recv = recv(connfd,recvline,MAXLINE-1,MSG_DONTWAIT);
                                }
                                else
                                {
                                    break;
                                }
                            }
                            if((recvline[cant_bytes_recv-1] == '\n') && (recvline[cant_bytes_recv-2] == '\r'))
                            {
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    memset(env_msg,0,MAXLINE);
                    strcpy(env_msg,recvline);
                    cant_bytes_env = srtlen(env_msg);
                    escr_ret_val = 0;
                    while(escr_ret_val <= 0)
                    {
                        if(*(arguments->salir) == 0)
                        {
                            if(*(req->conn) > 0)
                            {
                                escr_ret_val = send(*(req->conn),env_msg,cant_bytes_env,MSG_DONTWAIT);
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    if((escr_ret_val == -1) || ((long unsigned int)escr_ret_val != cant_bytes_env))
                    {
                        printf("Error al enviar mensaje en Thread Conexion %d, fd= %d\n",arguments->id,*(req->conn));
                        //printf("El errno es: ")
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
    }


}