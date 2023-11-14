#include "Common.h"

//Decalaracion de funciones utilizadas
void verificar_argumentos_Unix(int argc, char *argv[]);      //Comprobar argumentos

int main(int argc, char *argv[])
{
    //VARIABLES
    int sockfd;                                 //File descriptor de socket
    struct sockaddr_un server_addr;             //Struct para especificar server address

    //Para obtener mensaje por stdin que sera enviado al server
    char string[MAXLINE];                       //Ingresado en stdin
    char aux[MAXLINE];                          //Auxiliar utilizado para guardar sin salto de linea "/n"
    char fin_de_msg[7] = "\n";                  //Fin de mensaje agregado para simular HTTP
    char env_msg[MAXLINE];
    char recvline[MAXLINE];

    //Variables para enviar
    long unsigned int cant_bytes_env;
    long int cant_bytes_recv;
    long int escr_ret_val;

    //Llamamos a la verificadora de argumentos
    verificar_argumentos_Unix(argc,argv);

    //Creacion de socket
    //AF_UNIX
    //SOCK_STREAM
    //TCP
    if((sockfd = socket(AF_UNIX,SOCK_STREAM,0))<0)
    {
        printf("Fallo al crear socket\n");
        exit(EXIT_FAILURE);
    }
    
    //Handlear el path del archivo del server con el que se quiere conectar
    //Limpiamos la estructura y la cargamos con los valores pasados como argumentos
    memset((char*) &server_addr, '\0', sizeof(server_addr));    //Establece los primeros n bytes del área de bytes a partir de s en cero
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path,argv[1]);

    //Conexion con el server
    if(connect(sockfd, (SA*) &server_addr, sizeof(server_addr)) < 0)
    {
        printf("Falla al conectar con el servidor Unix.\nAsegurese de que el server este funcionando.\n");
        exit(EXIT_FAILURE);
    }

    //Mensaje recibido por argumentos
    strcpy(string,"SELECT * FROM Cars");

    //Contruccion del mensaje
    string[strcspn(string,"\n")] = 0;                   //Saca el \n
    strcpy(aux,string);                                 //Guarda en el aux
    strcat(string,fin_de_msg);                          //Concatena el string con fin_de_msg

    memset(env_msg,0,MAXLINE);
    strcat(env_msg,"Tipo A | ");
    strcat(env_msg,string);
    cant_bytes_env = strlen(env_msg);
    

    //Envio del mensaje
    escr_ret_val = write(sockfd,string, cant_bytes_env);    //Devuelve cantidad de bytes escritos o -1 si falla
            
    //Comprobamos que no devuelva -1 o haya llegado a la cantidad de bytes    
    if((escr_ret_val == -1) || ((long unsigned int)escr_ret_val != cant_bytes_env))
    {
        printf("Fallo al enviar mensaje\n");
            exit(EXIT_FAILURE);
    }

    memset(recvline,0,MAXLINE);
    cant_bytes_recv = 0;
    while((recvline[cant_bytes_recv - 1] != '\n') && (recvline[cant_bytes_recv - 2] != '\r'))
    {
        while(cant_bytes_recv <= 0)
        {
            cant_bytes_recv = recv(sockfd,recvline,MAXLINE-1,MSG_DONTWAIT);
        }
    }
    printf("Recibido: \n%s\n",recvline);
    close(sockfd);                                              //Cierra el socket
    exit(EXIT_SUCCESS);   
}

void verificar_argumentos_Unix(int argc, char *argv[])
{
    //La cantidad de argumentos debe ser 2
    if(argc != 2)
    {
        printf("Cantidad de argumentos invalida. Deberian ser 5\n");
        exit(EXIT_FAILURE);
    }

    //Verifica que el nombre del archivo sea correcto
    if((strlen(argv[1])>MAXLINE) || (!filename_valido(argv[1])))
    {
        printf("Nombre de archivo invalido\n");
        exit(EXIT_FAILURE);
    }
}