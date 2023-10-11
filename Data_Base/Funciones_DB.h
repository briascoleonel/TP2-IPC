#include <sqlite3.h> 
#include <pthread.h>

#ifndef FUNCIONESDB_H
#define FUNCIONESDB_H_H

void Crear_Llenar(sqlite3 **db, char *err_msg);
void get_Query(int rc, sqlite3 **db, char *query, char *resp, pthread_mutex_t *lock);
void add_msg(sqlite3 **db, char *msg, int *lastid);

#endif