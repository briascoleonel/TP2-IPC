#include "Common.h"

//Decalaracion de funciones utilizadas
void verificar_argumentos_IPv6(int argc, char *argv[]);      //Comprobar argumentos

int main(int argc, char *argv[])
{
    //VARIABLES
    int sockfd;                                     //File descriptor de socket
    struct sockaddr_in6 server_addr;                //Struct para especificar server address
    short unsigned int iport;                       //Para verificar numero de puerto

    //Para obtener mensaje por stdin que sera enviado al server
    char string[MAXLINE];                           //Ingresado en stdin
    char aux[MAXLINE];                              //Auxiliar utilizado para guardar sin salto de linea "/n"
    char fin_de_msg[7] = "\n";                      //Fin de mensaje agregado para simular HTTP
    char env_msg[MAXLINE];
    char recvline[MAXLINE];

    //Variables para enviar
    long unsigned int cant_bytes_env;
    long int cant_bytes_recv;
    long int escr_ret_val;


    //Llamamos a la verificadora de argumentos
    verificar_argumentos_IPv6(argc,argv);
    iport = (short unsigned int)atoi(argv[2]);      //Le pasamos el valor por stdin
    
    //Creacion de socket
    //AF_INET6
    //SOCK_STREAM
    //TCP
    if((sockfd = socket(AF_INET6,SOCK_STREAM,0))<0)      
    {
        printf("Fallo al crear socket\n");
        exit(EXIT_FAILURE);
    }

    //Handlear el path del archivo del server con el que se quiere conectar
    //Limpiamos la estructura y la cargamos con los valores pasados como argumentos
    bzero(&server_addr, sizeof(server_addr));       //Establece los primeros n bytes del área de bytes a partir de s en cero
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(iport);           //Convierte los bytes a estandar de bytes de red

    //Crea una estructura de dirección de red
    if(inet_pton(AF_INET6, argv[1], &server_addr.sin6_addr) <= 0)
    {
        printf("Fallo al convertir direccion IP  a binario\n");
        exit(EXIT_FAILURE);
    }

    server_addr.sin6_scope_id = if_nametoindex(argv[3]); //Devuelve el nombre de la interfaz

    //Conexion con el server
    if(connect(sockfd, (SA*) &server_addr, sizeof(server_addr)) < 0)
    {
        printf("Falla al conectar con el servidor IPv6.\nAsegurese de que el server este funcionando.\n");
        exit(EXIT_FAILURE);
    }

    while(strcmp(aux,"salir") != 0)
    {
        memset(string,0,MAXLINE);
        memset(env_msg,0,MAXLINE);
        strcat(env_msg,"Tipo B | ");
        printf("Ingrese el mensaje que se desea enviar: ");
        //Funcion para ingreso de mensaje
    }

    /*
    while(1)
    {
        if(cont != veces_enviado)   //Controlamos que se envie las veces requeridas
        {
            //Contruccion del mensaje
            string[strcspn(string,"\n")] = 0;                   //Saca el \n
            strcpy(aux,string);                                 //Guarda en el aux
            strcat(string,fin_de_msg);                          //Concatena el string con fin_de_msg

            //Envio del mensaje
            cant_bytes = strlen(string);                        //Guarda la cantidad de bytes
            escr_ret_val = write(sockfd,string, cant_bytes);    //Devuelve cantidad de bytes escritos o -1 si falla

            //Comprobamos que no devuelva -1 o haya llegado a la cantidad de bytes    
            if((escr_ret_val == -1) || ((long unsigned int)escr_ret_val != cant_bytes))
            {
                printf("Fallo al enviar mensaje\n");
                exit(EXIT_FAILURE);
            }
            cont++;                                             //Aumenta el contador
        }
        else
        {
            break;
        }
    }
*/
    close(sockfd);                                              //Cierra el socket
    exit(EXIT_SUCCESS);   
}

void verificar_argumentos_IPv6(int argc, char *argv[])
{
    //La cantidad de argumentos debe ser 7
    if(argc != 7)
    {
        printf("Cantidad de argumentos invalida. Deberian ser 5\n");
        exit(EXIT_FAILURE);
    }

    //Verifica que el numero de puerto ingresado sea correcto
    //Que sea un numero y este entre 0 y 65535
    for(unsigned int i=0; i < strlen(argv[2]); i++)
    {
        if((isdigit(argv[2][i]) == 0) || (atoi(argv[2]) <= 0) || (atoi(argv[2]) > 65535))
        {
            printf("Debe ingresar un numero de puerto correcto\n");
            exit(EXIT_FAILURE);
        }
    }

    //Verifica que la interfaz conste de letras y digitos
    for(unsigned int i = 0;i < strlen(argv[3]); i++)
    {
        if(((isdigit(argv[3][i]) == 0) && (isalpha(argv[3][i]) == 0)) || strlen(argv[3]) > MAXLINE)
        {
            printf("Debe ingresar un mensaje que solo conste de letras y numeros\n");
            exit(EXIT_FAILURE);
        }
    }

    //Verifica que el mensaje solo tenga letras y numeros
    for(unsigned int i = 0; i < strlen(argv[4]); i++)
    {
        if(((isdigit(argv[4][i]) == 0) && (isalpha(argv[4][i]) == 0)) || strlen(argv[4]) > MAXLINE)
        {
            printf("Debe ingresar un mensaje correcto\n");
            exit(EXIT_FAILURE);
        }
    }

    //Verifica que la cantidad de veces que se quiere enviar el mensaje sea correct
    for(unsigned int i = 0;i < strlen(argv[5]); i++)
    {
        if((isdigit(argv[5][i]) == 0) || (atoi(argv[5]) <= 0))
        {
            printf("Debe ingresar una cantidad de veces correcta\n");
            exit(EXIT_FAILURE);
        }
    }

    //Verifica que la cantidad de microsegundos a esperar antes de enviar sea correcta
    for(unsigned int i = 0;i < strlen(argv[6]); i++)
    {
        if((isdigit(argv[6][i]) == 0) || (atoi(argv[6]) <= 0))
        {
            printf("Debe ingresar una cantidad correcta\n");
            exit(EXIT_FAILURE);
        }
    }

}