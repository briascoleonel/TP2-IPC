#include "Common.h"
#include "Funciones_Server.h"
#include "Estructuras.h"

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
}
