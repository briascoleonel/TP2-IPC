#include "Common.h"

#define FILEPATH "test.db"

void* Task(void * arg)
{
    struct local_threads_arg_struct *argumentos = arg;

    char recvline[MAXLINE+1];     //Buffer que recibe, se pone MAXLINE +1
    char aux[MAXLINE];          //Auxilar para borrar checksum y fin de mensaje
    char rawmsg[MAXLINE] = "";       //Mensaje en crudo
    long int bytes_lect;
    unsigned long int conv_bytes_lect;
    char file_tam[MAXLINE];
    char buffer[MAXLINE];
    long int bytes_escr;

    char recvline_2[MAXLINE+1];
    long int bytes_lect_2;

    int rc = 0;
    char query[MAXLINE] = "";
    char resp[MAXLINE];

    while(*(argumentos->salir) == 0)
    {
        bytes_lect = 0;
        while(bytes_lect <= 0)
        {
            if(*(argumentos->salir) == 0)
            {
                bytes_lect = recv(*(argumentos->socket_conx), recvline, MAXLINE-1,MSG_DONTWAIT);
            }
            else
            {
                if(close(*(argumentos->socket_conx)) < 0)
                {
                    printf("Error al cerrar conexion %d.\n", *(argumentos->socket_conx));
                }
                break;
            }
        }

        if(*(argumentos->salir) == 0)
        {
            while(bytes_lect > 0)
            {
                if(*(argumentos->salir) == 0)
                {
                    if(recvline[bytes_lect -1] == '\n')
                    {
                        memset(aux,0,MAXLINE);
                        memset(rawmsg,0,MAXLINE);

                        strcpy(aux,recvline);
                        strcpy(rawmsg,aux);
                        rawmsg[strcspn(rawmsg,'\n')] = 0;
                        char t = get_tipo_mensaje(aux,query);

                        char addMsg[MAXLINE];
                        strcpy(addMsg,rawmsg);
                        addMsg[strcspn(addMsg,"\n")] = 0;
                        pthread_mutex_lock(argumentos->db_lock);
                        add_msg(argumentos->db,addMsg, argumentos->lastid);
                        pthread_mutex_unlock(argumentos->db_lock);

                        if((t == 'a') || (t == 'b'))
                        {
                            get_Query(rc,argumentos->db,query,resp,argumentos->db_lock);
                            strcat(resp,"\r\n");
                            bytes_lect = write(*(argumentos->socket_conx), resp, strlen(resp));
                            if(bytes_lect < 0)
                            {
                                printf("Errro de escritura.\n");
                                exit(EXIT_FAILURE);
                            }
                        }

                        if(t == 'c')
                        {
                            struct stat sb;
                            long int bytes_file;

                            if(stat(FILEPATH,&sb))
                            {
                                perror("stat");
                                exit(EXIT_FAILURE);
                            }
                            else
                            {
                                bytes_file = sb.st_size;
                            }

                            sprintf(file_tam,"%ld",bytes_file);
                            strcat(file_tam,"\r\n");
                            bytes_lect = write(*(argumentos->socket_conx), file_tam, strlen(file_tam));
                            if(bytes_lect < 0)
                            {
                                printf("Error de escritura.\n");
                                exit(EXIT_FAILURE);
                            }

                            bytes_lect_2 = 0;
                            memset(recvline_2,0,MAXLINE);
                            while(bytes_lect_2 <= 0)
                            {
                                if(*(argumentos->salir) == 0)
                                {
                                    bytes_lect_2 = recv(*(argumentos->socket_conx), recvline_2,MAXLINE-1,MSG_DONTWAIT);
                                }
                                else
                                {
                                    break;
                                }
                            }

                            int input_file = open(FILEPATH, O_RDONLY);
                            long int bytes_enviados = 0;

                            while(bytes_file > 0)
                            {
                                if(*(argumentos->salir) == 0)
                                {
                                    long int bytes_lect_3 = read(input_file, buffer, sizeof(buffer));
                                    if(bytes_lect_3 < 0)
                                    {
                                        printf("Error al leer archivo.\n");
                                        exit(EXIT_FAILURE);
                                    }

                                    long int bytes_escritos = write(*(argumentos->socket_conx), buffer, (unsigned long int)bytes_lect_3);
                                    if(bytes_escritos <= 0)
                                    {
                                        printf("Error al enviar.\n");
                                        exit(EXIT_FAILURE);
                                    }

                                    bytes_file -= bytes_escritos;
                                    bytes_enviados += bytes_escritos;
                                }
                                else
                                {
                                    break;
                                }
                            }
                            close(input_file);
                        }

                        if(t == 'd')
                        {
                            printf("No se ha especificado el tipo de cliente.\n");
                        }

                        bytes_lect = 0;
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }
    
    liberar_Handler(argumentos->Handlers,argumentos->id, argumentos->handler_lock);
    return NULL;
}