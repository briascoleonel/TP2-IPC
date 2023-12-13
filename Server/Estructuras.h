#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H_H

#include <pthread.h>     //           //Para funciones thread

#define MAXLINE 4096
#define SA struct sockaddr

struct db_req
{
    int id;
    int *conn;
    char *env_msg;
    struct db_req * next;
};

typedef struct db_req db_request;

typedef struct db_req_list db_request_list;
struct db_req_list
{
    db_request *node;
};

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
    int max_clientes;
    int *salir;
    pthread_mutex_t *req_list_lock;
    db_request_list *list;
    struct ack_arg_struct *ack_arg;

};

struct UNIX_arg_struct {
    char UNIX_File_Name[MAXLINE];
    pthread_mutex_t *req_list_lock;
    int max_clientes;
    int *salir;
    db_request_list *list;
};



struct IPv6_arg_struct {
    char IPV6_Server_Address[MAXLINE];
    short unsigned int IPV6_iport;
    char IPV6_Interface[MAXLINE];
    pthread_mutex_t *req_list_lock;
    int max_clientes;
    int *salir;
    db_request_list *list;
};

struct Pool_arg_struct {
    char IPV4_Server_Address[MAXLINE];
    short unsigned int IPV4_iport;
    struct ack_arg_struct *ack_arg;
    pthread_mutex_t *req_list_lock;
    db_request_list *list;
    int *salir;
};

#endif