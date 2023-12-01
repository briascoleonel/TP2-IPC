#include "Common.h"
#include "Funciones_Server.h"
#include "Thread_Conexion.h"

void* Pool_Conex_Thread_Codigo(void *arg)
{
    int conx_disp[5];
    struct pool_arg_struct *argumentos = arg;
    struct conx_arg_struct *conx_args;
    pthread_t *conx_threads;
    pthread_mutex_t *conx_lock;
    pthread_mutex_t *conx_list_lock;
    db_request_list **l;

    conx_threads = malloc((5 * sizeof(pthread_t)));
    conx_args = malloc((5 * sizeof(struct conx_arg_struct)));
    conx_lock = malloc((5 * sizeof(pthread_mutex_t)));
    conx_list_lock = malloc((5 * sizeof(pthread_mutex_t)));
    l = malloc((5 * sizeof(db_request_list)));

    for(int i = 0; i < 5; i++)
    {
        l[i] = new_db_request_list();
    }
    for(int i = 0; i < 5; i++)
    {
        //Esta bien?
        pthread_mutex_lock(&conx_list_lock[i]);
    }
    for(int i = 0; i < 5; i++)
    {
        pthread_mutex_lock(&conx_lock);
        conx_disp[i] = 1;
        pthread_mutex_unlock(&conx_lock);
    }

    for(int i = 0; i < 5; i++)
    {
        conx_args[i].id = i;
        strcpy(conx_args[i].IPV4_Server_Address,argumentos->IPV4_Server_Address);
        conx_args[i].IPV4_iport = argumentos->IPV4_iport;
        conx_args[i].conx_disp = conx_disp;
        conx_args[i].conx_lock = &conx_lock[i];
        conx_args[i].list_lock = &conx_list_lock;
        conx_args[i].ack_arg = argumentos->ack_arg;
        conx_args[i].list = l[i];
        conx_args[i].salir = argumentos->salir;
    }

    for(int i = 0; i < 5; i++)
    {
        pthread_create(&conx_threads[i],NULL,Thread_Conex_Codigo,&conx_args[i]);
    }

    int vacio = 1;
    int cant_conx_disp = 0;
    int sig_conx = -1;
}