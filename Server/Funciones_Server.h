#ifndef FUNCIONESSERVER_H
#define FUNCIONESSERVER_H_H

#include <pthread.h>

int dir_IPv4_valida(char *ipAddr);
int dir_IPv6_valida(char *ipAddr);
void ocupar_handler(int *Handlers, int i, pthread_mutex_t *lock);
void liberar_Handler(int *Handlers, int i, pthread_mutex_t *lock);
int filename_valido(char *string);
int get_prim_hand_disp(int *Handlers, long unsigned int maxHandlers);
int get_cant_hand_disp(int *Handlers, long unsigned int maxHandlers);

#endif
