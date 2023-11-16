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

#define SA struct sockaddr
#define MAXLINE 4096

int dir_IPv4_valida(char *ipAddr);
int dir_IPv6_valida(char *ipAddr);
int filename_valido(char *string);
int get_input(char *string, int max);


#endif