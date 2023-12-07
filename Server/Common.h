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

struct conx_arg_struct{
    char IPV4_Server_Address[MAXLINE];
    short unsigned int IPV4_iport;
    int *salir;
    int id;
    int *conx_disp;
    pthread_mutex_t *conx_lock;
    pthread_mutex_t *list_lock;
    db_request_list *list;
    struct ack_arg_struct *ack_arg;
};

struct ack_arg_struct{
    int *conx_socket;
    int *ack;
    pthread_mutex_t *ack_lock;
};

struct pool_arg_struct{
    char IPV4_Server_Address[MAXLINE];
    short unsigned int IPV4_iport;
    int *salir;
    pthread_mutex_t *req_list_lock;
    db_request_list *list;
    struct ack_arg_struct *ack_arg;
};

int isEmpty_db_request_list(db_request_list *self);
db_request *get_db_request(db_request_list *pl, int id);
void remove_req_list_head(db_request_list *self);
db_request_list *new_db_request_list();
void add_db_request(db_request_list *self, int id, int *conn, char *sendmsg);
void remove_db_request(db_request_list *self, int id);
int db_request_list_getNextID(db_request_list *self);






#endif 