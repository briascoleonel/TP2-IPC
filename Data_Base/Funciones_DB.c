#include "Common.h"

void Crear_Llenar(sqlite3 **db, char *err_msg)
{
    int rc;
    rc = sqlite3_open("test.db",db);

    if(rc != SQLITE_OK)
    {
        fprintf(stderr,"No se puede abrir Base de Datos: %s\n", sqlite3_errmsg(*db));
        sqlite3_close(*db);
        exit(EXIT_FAILURE);
    }

    char *sql = "DROP TABLE IF EXISTS Cars;" 
                "CREATE TABLE Cars(Id INT, Name TEXT, Price INT);" 
                "INSERT INTO Cars VALUES(1, 'Audi', 52642);"  
                "INSERT INTO Cars VALUES(2, 'Mercedes', 57127);" 
                "INSERT INTO Cars VALUES(3, 'Skoda', 9000);" 
                "INSERT INTO Cars VALUES(4, 'Volvo', 29000);" 
                "INSERT INTO Cars VALUES(5, 'Bentley', 350000);" 
                "INSERT INTO Cars VALUES(6, 'Citroen', 21000);" 
                "INSERT INTO Cars VALUES(7, 'Hummer', 41400);" 
                "INSERT INTO Cars VALUES(8, 'Volkswagen', 21600);"
                "INSERT INTO Cars VALUES(9, 'Ford', 12342);"
                "DROP TABLE IF EXISTS Mensajes;" 
                "CREATE TABLE Mensajes(Id INT, Msg TEXT);" ;

    printf("Llenando base de datos\n");

    rc = sqlite3_exec(*db, sql, 0, 0, &err_msg);

    if(rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL Error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(*db);
        exit(EXIT_FAILURE);
    }

}

void get_Query(int rc, sqlite3 **db, char *query, char *resp, pthread_mutex_t *lock)
{
    sqlite3_stmt *res;
    char aux[MAXLINE];

    memset(resp, 0, MAXLINE);

    pthread_mutex_lock(lock);
    rc = sqlite3_prepare_v2(*db, query, -1, &res, 0);
    pthread_mutex_unlock(lock);

    if(rc != SQLITE_OK)
    {
        sprintf(resp, "Fallo al buscar data: %s\n", sqlite3_errmsg(*db));
    }
    else
    {
        while(sqlite3_step(res) != SQLITE_DONE)
        {
            int i;
            int cant_col = sqlite3_column_count(res);
            for (i = 0; i < cant_col; i++)
            {
                switch (sqlite3_column_type(res, i))
                {
                case (SQLITE_TEXT):
                    sprintf(aux, "%s, ", sqlite3_column_text(res, i));
                    strcat(resp,aux);
                    break;
                case(SQLITE_INTEGER):
                    sprintf(aux, "%d, ", sqlite3_column_int(res, i));
                    strcat(resp,aux);
                    break;
                case(SQLITE_FLOAT):
                    sprintf(aux, "%g, ", sqlite3_column_double(res, i));
                    strcat(resp, aux);
                    break;
                default:
                    break;
                }
            }
            sprintf(aux, "\n");
            strcat(resp, aux);
        }
    }

}

void add_msg(sqlite3 **db, char *msg, int *lastid)
{
    int rc;
    char comando[MAXLINE];
    char *err_msg = 0;

    rc = sqlite3_open("test.db", db);

    if(rc != SQLITE_OK)
    {
        fprintf(stderr, "No se puede abrir base de datos: %s\n", sqlite3_errmsg(*db));
        sqlite3_close(*db);
        exit(EXIT_FAILURE);
    }
    
    sprintf(comando,"INSERT INTO Mensajes VALUES(%d, '%s');",  (*(lastid) + 1), msg);
    
    rc = sqlite3_exec(*db, comando, 0, 0, &err_msg);

    if(rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL Error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(*db);
        exit(EXIT_FAILURE);
    }

    *(lastid) = *(lastid) + 1;
}