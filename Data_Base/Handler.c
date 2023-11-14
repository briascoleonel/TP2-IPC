#include "Common.h"

#define FILEPATH "test.db"

void* Task(void * arg)
{
    struct local_threads_arg_struct *argumentos = arg;

    char recvline[MAXLINE+1];               //Buffer que recibe, se pone MAXLINE +1
    char aux[MAXLINE];                      //Auxilar para borrar checksum y fin de mensaje
    char rawmsg[MAXLINE] = "";              //Mensaje en crudo
    long int bytes_lect;                    //Se utiliza para enviar el archivo
    char file_tam[MAXLINE] = "";            //Para construccion del msj con el tamaño del archivo                 
    char buffer[MAXLINE] = "";              //Se utiliza para enviar el archivo
    long int bytes_escr;                    //Se utiliza para enviar el archivo        

    char recvline_2[MAXLINE+1];             //Se utiliza para enviar el ACK
    long int bytes_lect_2;                  //Se utiliza para enviarl el ACK

    int rc = 0;                             //Para operaciones con DB
    char query[MAXLINE] = "";               //Query
    char resp[MAXLINE];                     //Respuesta

    
    //LECTURA DE MENSAJE
    while(*(argumentos->salir) == 0)        //Mientras no se haya intrudico salir
    {
        bytes_lect = 0;                     //Inicialmente en 0
        while(bytes_lect <= 0)              //Cuando sea mayor a 0
        {
            if(*(argumentos->salir) == 0)   //Se llama a funcion recv
            {
                bytes_lect = recv(*(argumentos->socket_conx), recvline, MAXLINE-1,MSG_DONTWAIT);
            }
            else                            //Dos opciones: se aca el tiempo o se produce error de lectura
            {
                if(close(*(argumentos->socket_conx)) < 0) //Cierra y muestra error en caso de que lo haya
                {
                    printf("Error al cerrar conexion %d.\n", *(argumentos->socket_conx));
                }
                break;
            }
        }

        //EVALUACION DE MENSAJE
        if(*(argumentos->salir) == 0)       //Si no se instrodujo salir
        {
            while(bytes_lect > 0)           //Y se hayan recibido mensajes
            {
                if(*(argumentos->salir) == 0)
                {
                    if(recvline[bytes_lect -1] == '\n')     //Detecatmos el final del mensaje
                    {
                        memset(aux,0,MAXLINE);              //Se borra el contendio de aux colocando ceros
                        memset(rawmsg,0,MAXLINE);           //Se borra el contenido de rawmsg colocando ceros

                        strcpy(aux,recvline);               //Copiamos la query(recvline)
                        strcpy(rawmsg,aux);                 //La pasamos al aux
                        rawmsg[strcspn(rawmsg,'\n')] = 0;   //Trunca la cadena donde se encuentra el primer '\n'
                        
                        //Llama a la funcion que devuelce el tipo de mensaje (en Common) y lo almacena en t
                        char t = get_tipo_mensaje(aux,query);

                        //Agregamos el mensaje a la tabla de mensajes de la DB
                        char addMsg[MAXLINE];               
                        strcpy(addMsg,rawmsg);
                        addMsg[strcspn(addMsg,"\n")] = 0;
                        pthread_mutex_lock(argumentos->db_lock);
                        add_msg(argumentos->db,addMsg, argumentos->lastid);
                        pthread_mutex_unlock(argumentos->db_lock);

                        //DISTINCION DE TIPOS
                        if((t == 'a') || (t == 'b'))            //Si es a o b -> Hace la query y envia el la respuesta
                        {
                            get_Query(rc,argumentos->db,query,resp,argumentos->db_lock);        //Llamada a funcion para obtener query (en Funciones_DB)
                            strcat(resp,"\r\n");                                                //Agrega \r\n
                            bytes_lect = write(*(argumentos->socket_conx), resp, strlen(resp)); //Envia la respuesta
                            //Handlear error
                            if(bytes_lect < 0)
                            {
                                printf("Errro de escritura.\n");
                                exit(EXIT_FAILURE);
                            }
                        }

                        if(t == 'c')                            //Si es c -> Realiza el handshake
                        {
                            struct stat sb;                     //Struct para el tamaño del archivo 
                            long int bytes_file;                //Cantidad de bytes del archivo

                            if(stat(FILEPATH,&sb))
                            {
                                perror("stat");
                                exit(EXIT_FAILURE);
                            }
                            else
                            {
                                bytes_file = sb.st_size;        //Guardamos el tamaño
                            }

                            //Enviamos el tamaño del archivo
                            sprintf(file_tam,"%ld",bytes_file); //Convierte de valor numerico a cadena     
                            strcat(file_tam,"\r\n");            //Agregamos \r\n
                            bytes_lect = write(*(argumentos->socket_conx), file_tam, strlen(file_tam));     //Envio 
                            if(bytes_lect < 0)      //Handlear error
                            {
                                printf("Error de escritura.\n");
                                exit(EXIT_FAILURE);
                            }

                            //HANDSHAKE
                            bytes_lect_2 = 0;
                            memset(recvline_2,0,MAXLINE);       //Hacemos 0
                            while(bytes_lect_2 <= 0)
                            {
                                if(*(argumentos->salir) == 0)
                                {
                                    //Recibe los datos (no bloqueante), almacenando en recvline_2 y guardando cantidad de bytes en bytes_lect_2
                                    bytes_lect_2 = recv(*(argumentos->socket_conx), recvline_2,MAXLINE-1,MSG_DONTWAIT);  
                                }
                                else
                                {
                                    break;
                                }
                            }

                            //ENVIO DEL ACK
                            int input_file = open(FILEPATH, O_RDONLY);      //Abre el archivo
                            long int bytes_enviados = 0;                    //Cant bytes enviados

                            while(bytes_file > 0)       //Mientras el tamaño sea mayor a 0
                            {
                                if(*(argumentos->salir) == 0)
                                {
                                    long int bytes_lect_3 = read(input_file, buffer, sizeof(buffer));  //Lee los datos desde el input file, alamacena en buffer y guarda la cantidad e bytes en bytes_lect_3
                                    //Handlear error
                                    if(bytes_lect_3 < 0)
                                    {
                                        printf("Error al leer archivo.\n");
                                        exit(EXIT_FAILURE);
                                    }

                                    long int bytes_escritos = write(*(argumentos->socket_conx), buffer, (unsigned long int)bytes_lect_3);       //Escribe y guarda la cantidad de bytes escritos
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

                        if(t == 'd')  //Si es d -> Sale mensaje de que no se especifico el tipo de cliente
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