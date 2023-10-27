#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H_H

#include <pthread.h>     //           //Para funciones thread

#define MAXLINE 4096
#define SA struct sockaddr

struct local_threads_arg_struct {
    int id;
    int segs;
    long unsigned int *total_bytes_recv_local;
    long unsigned int *ult_bytes_recv_local;
    long unsigned int *total_bytes_recv_global;
    long unsigned int *ult_bytes_recv_global;
    int *socket_conx;
    int thread_salida;
    int *Handlers;
    pthread_mutex_t *lock;
    pthread_mutex_t *global_lock;
    int *salir;
};

struct IPv4_arg_struct {
    char IPV4_Server_Address[MAXLINE];
    short unsigned int IPV4_iport;
    char IPv4_Write_File_Name[MAXLINE];
    pthread_mutex_t *global_lock;
    long unsigned int *total_bytes_recv_global;
    long unsigned int *ult_bytes_recv_global;
    int max_clientes;
    int *salir;
};

struct UNIX_arg_struct {
    char UNIX_File_Name[MAXLINE];
    char UNIX_Write_File_Name[MAXLINE];
    pthread_mutex_t *global_lock;
    long unsigned int *total_bytes_recv_global;
    long unsigned int *ult_bytes_recv_global;
    int max_clientes;
    int *salir;
};



struct IPv6_arg_struct {
    char IPV6_Server_Address[MAXLINE];
    short unsigned int IPV6_iport;
    char IPV6_Interface[MAXLINE];
    char IPv6_Write_File_Name[MAXLINE];
    pthread_mutex_t *global_lock;
    long unsigned int *total_bytes_recv_global;
    long unsigned int *ult_bytes_recv_global;
    int max_clientes;
    int *salir;
};

#endif