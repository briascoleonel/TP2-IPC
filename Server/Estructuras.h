#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H_H

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
    int *socket_conx;
    int thread_salida;
    int *Handlers;
    pthread_mutex_t *handler_lock;
    int *salir;
    int segs;
    struct ack_arg_struct *ack_arg;
    pthread_mutex_t *req_list_lock;
    pthread_mutex_t *lock;
    pthread_mutex_t *list;
    
};

struct IPv4_arg_struct {
    char IPV4_Server_Address[MAXLINE];
    short unsigned int IPV4_iport;
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

struct pool_arg_struct{
    char IPV4_Server_Address[MAXLINE];
    short unsigned int IPV4_iport;
    int *salir;
    pthread_mutex_t *req_list_lock;
    db_request_list *list;
    struct ack_arg_struct *ack_arg;
};

struct ack_arg_struct{
    int *conx_socket;
    int *ack;
    pthread_mutex_t *ack_lock;
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

int isEmpty_db_request_list(db_request_list *self);
db_request *get_db_request(db_request_list *pl, int id);
void remove_req_list_head(db_request_list *self);
void add_db_request(db_request_list *self, int id, int *conn, char *sendmsg);
void remove_db_request(db_request_list *self, int id);
int db_request_list_getNextID(db_request_list *self);
db_request_list *new_db_request_list();


#endif