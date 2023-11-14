#include "Common.h"

int main(int argc, char *argv[])
{
    //VARIABLES
    sqlite3 *db;                //Base de datos
    char *err_msg = 0;          //Mensaje de error

    Crear_Llenar(&db, err_msg); //Llamado a funcion para crear y llenar la DB

    //Variables de Server de DB    
    pthread_t IPv4_Server_Thread;
    struct IPv4_arg_struct IPv4_argumentos;

    //Mecanismo de salida
    char salir[MAXLINE];
    int salir_todos = 0;

    verificar_argumentos(argc,argv);

    //Asignacion de argumentos de acuerdo a la estructura de cada hilo
    
    //IPv4
    strcpy(IPv4_argumentos.IPV4_Server_Address,argv[1]);
    IPv4_argumentos.IPV4_iport = (short unsigned int)atoi(argv[2]);
    IPv4_argumentos.db = &db;
    IPv4_argumentos.salir = &salir_todos;
    IPv4_argumentos.max_clientes = CONX_DB;

    //Lanza el hilo del Server
    pthread_create(&IPv4_Server_Thread,NULL,Server_IPv4_codigo, &IPv4_argumentos);

    //Mecanismo de salida
    while (!salir_todos) 
    {
        printf("Ingrese -salir- para cerrar el server\n");
        fgets(salir, sizeof(salir), stdin);
        if (strcmp(salir, "salir\n") == 0) 
        {
            salir_todos = 1;
        } 
        else 
        {
            printf("No ha ingresado salir. Ha ingresado: %s\n", salir);
        }
    }
}