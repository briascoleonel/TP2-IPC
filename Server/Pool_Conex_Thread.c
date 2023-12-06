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

    while((argumentos->salir) == 0)
    {
        pthread_mutex_lock(argumentos->req_list_lock);
        vacio = isEmpty_db_request_list(argumentos->list);
        pthread_mutex_unlock(argumentos->req_list_lock);

        while(vacio)
        {
            if(((*argumentos->salir) == 0))
            {
                pthread_mutex_lock(argumentos->req_list_lock);
                vacio = isEmpty_db_request_list(argumentos->req_list_lock);
                pthread_mutex_unlock(argumentos->req_list_lock);
            }
            else
            {
                break;
            }
        }

        if((*argumentos->salir) == 0)
        {
            while(cant_conx_disp == 0)
            {
                if(((*argumentos->salir) == 0))
                {
                    cant_conx_disp = get_cant_hand_disp(cant_conx_disp, (long unsigned int)5, conx_lock);
                }
                else
                {
                    break;
                }
            }
            if(*(argumentos->salir) == 0)
            {
                sig_conx = get_prim_hand_disp(conx_disp, 5, conx_lock);
                cant_conx_disp = 0;
                while(sig_conx == -1)
                {
                    sig_conx = get_prim_hand_disp(conx_disp, 5, conx_lock);
                }
                ocupar_handler(conx_disp,sig_conx,&conx_lock[sig_conx]);
                if(sig_conx == 5)
                {
                    printf("Hubo siguiente con 5\n");
                    exit(0);
                }
            }

        }
    }

}