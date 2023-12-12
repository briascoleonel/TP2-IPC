#include "Common.h"
#include "Funciones_Server.h"
#include "Estructuras.h"
#include "Pool_Conex_Thread.h"

int main(int argc, char *argv[])
{
    int conx_socket[5];
    int ack[5];
    int salir_todos = 0;

    pthread_t Unix_Thread_Server;
    pthread_t IPv4_Thread_Server;
    pthread_t IPv6_Thread_Server;
    pthread_t pool_conx;

    pthread_mutex_t ack_lock[5];
    pthread_mutex_t req_list_lock;

    db_request_list *l = new_db_request_list();
    
    struct UNIX_arg_struct Unix_argumentos;
    struct IPv4_arg_struct IPv4_argumentos;
    struct IPv6_arg_struct IPv6_argumentos;
    struct ack_arg_struct  ack_argumentos[5];
    struct pool_arg_struct pool_conx_argumentos;

    char file_path[MAXLINE];
    char file_name[MAXLINE];
    char salir[MAXLINE];

    for(int i = 0; i < 5; i++)
    {
        ack[i] = 1;
        ack_argumentos[i].ack = &(ack[i]);
        ack_argumentos[i].ack_lock = &(ack_lock[i]);
        ack_argumentos[i].conx_socket = &(conx_socket[i]);
    }

    verificarArgumentos(argc,argv);
    strcpy(pool_conx_argumentos.IPV4_Server_Address,argv[7]);
    pool_conx_argumentos.IPV4_iport = (unsigned short int)atoi(argv[8]);
    pool_conx_argumentos.salir = &salir_todos;
    pool_conx_argumentos.list = l;
    pool_conx_argumentos.req_list_lock = &req_list_lock;
    pool_conx_argumentos.ack_arg = ack_argumentos;

    pthread_create(&pool_conx,NULL,Pool_Conex_Thread_Codigo,&pool_conx_argumentos);
    strcpy(IPv4_argumentos.IPV4_Server_Address,argv[1]);
    IPv4_argumentos.IPV4_iport = (short unsigned int)atoi(argv[2]);
    IPv4_argumentos.req_list_lock = &req_list_lock;
    IPv4_argumentos.list = l;
    IPv4_argumentos.max_clientes = atoi(argv[9]);
    IPv4_argumentos.salir = &salir_todos;
    IPv4_argumentos.ack_arg = ack_argumentos;
}