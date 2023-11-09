#ifndef COMMON_H    // Comienzo del ifdef guard
#define COMMON_H_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/un.h>//
#include <net/if.h>  //               //Network interfaces
#include <netinet/in.h>//             //Para struct de in
#include <pthread.h>     //           //Para funciones thread
#include <errno.h>//
#include <stdatomic.h>
#include <signal.h>
#include <sys/time.h>
#include <sqlite3.h>
#include <fcntl.h> 


#include "Server_IPv4.h"
#include "Verificar_Argumentos_Server.h"
#include "Config_Socket_IPv4.h"
#include "Handler.h"
#include "Funciones_DB.h"


#define SA struct sockaddr
#define MAXLINE 4096
#define CONX_DB 5

//Struct que vamos a utilizar
struct local_threads_arg_struct {
    int id;
    int *socket_conx;
    int thread_salida;
    int *Handlers;
    pthread_mutex_t *handler_lock;
    pthread_mutex_t *db_lock;
    int *salir;
    sqlite3 **db;
};

struct IPv4_arg_struct {
    char IPV4_Server_Address[MAXLINE];
    short unsigned int IPV4_iport;
    sqlite3 **db;
    int max_clientes;
    int *salir;
};

int dir_IPv4_valida(char *ipAddr);
void ocupar_handler(int *Handlers, int i, pthread_mutex_t *lock);
void liberar_Handler(int *Handlers, int i, pthread_mutex_t *lock);
int filename_valido(char *string); 
int get_prim_hand_disp(int *Handlers, long unsigned int maxHandlers);
int get_cant_hand_disp(int *Handlers, long unsigned int maxHandlers);



#endif 