#include "Common.h"
#define FILEPATH "test.db"


//Decalaracion de funciones utilizadas
void verificar_argumentos_IPv4(int argc, char *argv[]);      //Comprobar argumentos
void descargar_archivo(int *socket, unsigned long int tam_file);
void prueba_query();

int main(int argc, char *argv[])
{
    //VARIABLES
    int sockfd;                                     //File descriptor de socket
    struct sockaddr_in server_addr;                 //Struct para especificar server address
    short unsigned int iport;                       //Para verificar numero de puerto

    //Para obtener mensaje por stdin que sera enviado al server
    char recvline[MAXLINE];                           //Ingresado en stdin
    char env_msg[MAXLINE];                              //Auxiliar utilizado para guardar sin salto de linea "/n"

    //Variables para enviar
    long unsigned int cant_bytes_env;
    long int cant_bytes_recv;
    long int escr_ret_val;

    //Llamamos a la verificadora de argumentos
    verificar_argumentos_IPv4(argc,argv);
    iport = (short unsigned int)atoi(argv[2]);      //Le pasamos el valor por stdin
    
    //Creacion de socket
    //AF_INET
    //SOCK_STREAM
    //TCP
    if((sockfd = socket(AF_INET,SOCK_STREAM,0))<0)    
    {
        printf("Fallo al crear socket\n");
        exit(EXIT_FAILURE);
    }

    //Handlear el path del archivo del server con el que se quiere conectar
    //Limpiamos la estructura y la cargamos con los valores pasados como argumentos
    bzero(&server_addr, sizeof(server_addr));       //Establece los primeros n bytes del área de bytes a partir de s en cero
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(iport);            //Convierte los bytes a estandar de bytes de red

    //Crea una estructura de dirección de red
    if(inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0)
    {
        printf("Fallo al convertir direccion IP  a binario\n");
        exit(EXIT_FAILURE);
    }

    //Conexion con el server
    if(connect(sockfd, (SA*) &server_addr, sizeof(server_addr)) < 0)
    {
        printf("Falla al conectar con el servidor IPv4.\nAsegurese de que el server este funcionando.\n");
        exit(EXIT_FAILURE);
    }

    memset(env_msg,0,MAXLINE);
    strcat(env_msg,"Tipo C | Descargar archivo\n");

    cant_bytes_env = strlen(env_msg);
    escr_ret_val = write(sockfd,env_msg,cant_bytes_env);
    if((escr_ret_val == -1) || ((long unsigned int)escr_ret_val != cant_bytes_env))
    {
        printf("Error al enviar mensaje.\n");
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

    if(cant_bytes_recv < 0)
    {
        printf("Error de lectura.\n");
        exit(EXIT_FAILURE);
    }

    memset(env_msg,0,MAXLINE);
    strcat(env_msg,"Tam recibido\n");
    cant_bytes_env = strlen(env_msg);
    escr_ret_val = write(sockfd,env_msg,cant_bytes_env);
    if((escr_ret_val == -1) || ((long unsigned int)escr_ret_val != cant_bytes_env))
    {
        printf("Error al enviar mensaje.\n");
        exit(EXIT_FAILURE);
    }

    char tam_file[MAXLINE];
    strcpy(tam_file,recvline);
    tam_file[strcspn(tam_file,"\n")] = 0;
    long unsigned int TAM = (long unsigned int)atoi(tam_file);
    //Implementar funcion para descargar el file

    descargar_archivo(&sockfd,TAM);
    prueba_query();

    close(sockfd);                                              //Cierra el socket
    exit(EXIT_SUCCESS);   
}

void verificar_argumentos_IPv4(int argc, char *argv[])
{
    //La cantidad de argumentos debe ser 3
    if(argc != 3)
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

}

void descargar_archivo(int *socket, unsigned long int tam_file)
{
    char recvline[MAXLINE];
    long int recv_bytes = 0;
    long int total_bytes_recv = 0;
    long int bytes_escr = 0;
    unsigned long int TAM_file = tam_file;

    int archivo = open(FILEPATH,O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(archivo == -1)
    {
        printf("Error al crear/leer archivo.\n");
        exit(EXIT_FAILURE);
    }

    while(TAM_file != 0)
    {
        memset(recvline,0,MAXLINE);
        recv_bytes = read(*(socket),recvline,MAXLINE);
        TAM_file -= (unsigned long int)recv_bytes;
        total_bytes_recv += recv_bytes;
        bytes_escr += write(archivo,recvline,(unsigned long int)recv_bytes);
    }

    if((close(archivo) < 0))
    {
        printf("Error al cerrar conexion.\n");
        exit(EXIT_FAILURE);
    }

}

void prueba_query()
{
    sqlite3 *db;    
    sqlite3_stmt *res;
    
    int rc = sqlite3_open("test.db", &db); 
    
    if (rc != SQLITE_OK)  
    {
        printf("No se puede abrir la db. El error es %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    rc = sqlite3_prepare_v2(db, "SELECT Name FROM Cars", -1, &res, 0);

    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    printf("Display resultados:\n");

    while(sqlite3_step(res) != SQLITE_DONE)
    {
        int i;
        int num_cols = sqlite3_column_count(res);
        for (i = 0; i < num_cols; i++)
		{
			switch (sqlite3_column_type(res, i))
			{
			case (SQLITE3_TEXT):
				printf("%s, ", sqlite3_column_text(res, i));
				break;
			case (SQLITE_INTEGER):
				printf("%d, ", sqlite3_column_int(res, i));
				break;
			case (SQLITE_FLOAT):
				printf("%g, ", sqlite3_column_double(res, i));
				break;
			default:
				break;
			}
		}
		printf("\n");
    }
}

