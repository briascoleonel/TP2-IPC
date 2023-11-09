#include "Common.h"

void *Server_IPv4_codigo(void *arg)
{
    //VARIABLES
    //File descriptors
    int listenfd;                                           //Utilizado para escuchar conexiones
    int *connfd;                                            //Utilizado una vez ocurra el accept y haya conexion ya establecida
    
    //Estructuras
    struct IPv4_arg_struct *argumentos = arg;               //Recuperar los argumentos
    struct sockaddr_in serv_addr;                           //Para pasar la direccion del server
    struct local_threads_arg_struct *handler_thread_args;   //Para handlear las conexiones

    //Asignacion de memoria utilizando max_clientes como referencia
    connfd = malloc((unsigned long int) argumentos->max_clientes * sizeof(int));
    for(int i = 0; i < argumentos->max_clientes; i++)
    {
        connfd[i] = -1;
    }

    //Variable para el siguiente id obtenido
    int ult_id = 0;

    //HILOS
    pthread_t *task_thread;                                 //Tarea en si
    pthread_mutex_t handler_lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t db_lock = PTHREAD_MUTEX_INITIALIZER;       //Para la exclusion mutua
 
    //Asignacion de memoria para los hilos utilizando cantidad maxima de clientes como referencia
    task_thread = malloc((unsigned long int) argumentos->max_clientes *sizeof(pthread_t));
    handler_thread_args = malloc((unsigned long int) argumentos->max_clientes * sizeof(struct local_threads_arg_struct));

    //VARIABLES DE HANDLERS
    long unsigned int cant_handlers_disp = 0;               //Cantidad de handlers disponible
    int *handlers_disp;                                     //Array para saber que handler esta disponible
    int sig_handler;                                        //Utiliza el primer handler que se encuentra disponible

    //Asignacion de memoria utilizando max_clientes como referencia
    handlers_disp = malloc((unsigned long int)argumentos->max_clientes *sizeof(int));
    
    //Para configurar el socket se utiliza esta funcion
    server_conf_socket_IPv4(&listenfd, &serv_addr, argumentos->IPV4_iport, (unsigned long int)argumentos->max_clientes, argumentos->IPV4_Server_Address);

    //Iniciamos todos los handlers como disponibles
    for(int i=0; i<argumentos->max_clientes; i++)
    {
        handlers_disp[i] = 1;
    }

    //Esperando conexiones y lanzando hilos hasta que salir deje de ser 0
    while(*(argumentos->salir) == 0)
    {
        //En exclusion mutua usando el lock
        pthread_mutex_lock(&handler_lock);
        //Usa la funcion get_cant_hand_disp
        cant_handlers_disp = (long unsigned int) get_cant_hand_disp(handlers_disp,(unsigned long int) argumentos->max_clientes);
        pthread_mutex_unlock(&handler_lock);                        //Libero el lock

        //Compruebo que queden handlers disponibles
        //Usa get_prim_hand_disp para comprobar tambien que haya disponibles
        if(cant_handlers_disp == 0 || get_prim_hand_disp(handlers_disp, (unsigned long int)argumentos->max_clientes) < 0)
        {
            while(cant_handlers_disp == 0)
            {
                if(*(argumentos->salir) == 0)
                {
                    pthread_mutex_lock(&handler_lock);
                    cant_handlers_disp = (long unsigned int)get_cant_hand_disp(handlers_disp, (unsigned long int)argumentos->max_clientes);
                    pthread_mutex_unlock(&handler_lock);
                }
                else
                {
                    break;
                }
            }
        }

        if(*(argumentos->salir) == 0)
        {
            //Pasando la prueba de disponibilidad, agarra el primero disponible
            //Tambien en exclusion mutua
            pthread_mutex_lock(&handler_lock);
            sig_handler = get_prim_hand_disp(handlers_disp, (unsigned long int)argumentos->max_clientes);
            pthread_mutex_unlock(&handler_lock);

            int flags = fcntl(listenfd, F_GETFL, 0);
            fcntl(listenfd, F_SETFL, flags | O_NONBLOCK);

            while(connfd[sig_handler] < 0)
            {
                if(*(argumentos->salir) == 0)
                {
                    //Espera y acepta una conexion en el handler disponible
                    connfd[sig_handler] = accept(listenfd, (SA*) NULL, NULL);
                }
                else
                {
                    break;
                }
            }

            //Ya habiendo conexion, carga los argumentos de la estructura
            handler_thread_args[sig_handler].id = sig_handler;
            handler_thread_args[sig_handler].socket_conx= &(connfd[sig_handler]);
            handler_thread_args[sig_handler].thread_salida = 0;
            handler_thread_args[sig_handler].Handlers = handlers_disp;
            handler_thread_args[sig_handler].handler_lock = &handler_lock;
            handler_thread_args[sig_handler].db_lock = &db_lock;
            handler_thread_args[sig_handler].salir = argumentos->salir;
            handler_thread_args[sig_handler].db = argumentos->db;

            //Se lanzan los hilos de la tarea en si y el contador
            pthread_create(&(task_thread[sig_handler]), NULL, Task, &(handler_thread_args[sig_handler]));
            //Se modifican los handlers disponibles en exclusion mutua
            ocupar_handler(handlers_disp,sig_handler,&handler_lock);
        }
        else
        {
            break;
        }
    }

    //En caso de que se introduzca "salir", con join esperamos a terminen los hilos que estan corriendo
    if(*(argumentos->salir) == 1)
    {
        for(int i=0; i<argumentos->max_clientes;i++)
        {
            pthread_join(task_thread[i],NULL);
        }
    }

    //Recorremos y cerramos todos los file descriptors
    //CONEXION
    for(int i=0;i<argumentos->max_clientes;i++)
    {
        if((close(connfd[i])<0))
        {
            printf("Erro al cerrar conn %d\n",i);
            exit(EXIT_FAILURE);
        }
    }

    //ESCUCHA
    if((close(listenfd)<0)) 
    {
        printf("Error al cerrar listenfd\n");
        exit(EXIT_FAILURE); 
    }

    //Liberamos la memoria previamente asignada
    free(handlers_disp);   
    free(task_thread);          
    free(connfd);              
    return NULL;

}









