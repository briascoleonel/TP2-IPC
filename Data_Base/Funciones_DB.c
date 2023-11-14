#include "Common.h"

//Para crear y llenar la Base de Datos
void Crear_Llenar(sqlite3 **db, char *err_msg)
{
    int rc;                             //Para recibir success o error de las funciones sqlite3
    rc = sqlite3_open("test.db",db);    //Abre la DB, si no existe, la crea

    if(rc != SQLITE_OK)             //Handlea error
    {
        fprintf(stderr,"No se puede abrir Base de Datos: %s\n", sqlite3_errmsg(*db));   //Imprime el error usando sqlite3_errmsg
        sqlite3_close(*db);         //Cierra la DB
        exit(EXIT_FAILURE);
    }

    //LLENADO DE LA DB (Se uso lo de clase)
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

    rc = sqlite3_exec(*db, sql, 0, 0, &err_msg);        //Ejecuta la consulta, guardando asi la data en el DB

    if(rc != SQLITE_OK)         //Handlea error
    {
        fprintf(stderr, "SQL Error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(*db);
        exit(EXIT_FAILURE);
    }

}

//Para obternes la respuesta de la query
void get_Query(int rc, sqlite3 **db, char *query, char *resp, pthread_mutex_t *lock)
{
    sqlite3_stmt *res;      //Respuesta
    char aux[MAXLINE];

    memset(resp, 0, MAXLINE);       //Limpiamos buffer que contiene la respuesta

    //Realizar query con lo recibido, en ex mutua
    pthread_mutex_lock(lock);
    rc = sqlite3_prepare_v2(*db, query, -1, &res, 0);   //Prepara la query 
    pthread_mutex_unlock(lock);

    if(rc != SQLITE_OK)         //Handlea errores
    {
        sprintf(resp, "Fallo al buscar data: %s\n", sqlite3_errmsg(*db));
    }
    else            //Si los pasa...
    {
        while(sqlite3_step(res) != SQLITE_DONE)         //Espera que se haya terminado la consulta
        {
            int i;
            int cant_col = sqlite3_column_count(res);   //Cantidad de columnas 
            for (i = 0; i < cant_col; i++)              //Recorre todas las columnas
            {
                switch (sqlite3_column_type(res, i))    //Determinamos el tipo de datos de la columna actual
                {
                case (SQLITE_TEXT):
                    sprintf(aux, "%s, ", sqlite3_column_text(res, i));  //Formatea y almacena en aux el valor de la columna de texto
                    strcat(resp,aux);
                    break;
                case(SQLITE_INTEGER):
                    sprintf(aux, "%d, ", sqlite3_column_int(res, i));   //Formatea y almacena en aux el valor de la columna de enteros
                    strcat(resp,aux);
                    break;
                case(SQLITE_FLOAT):
                    sprintf(aux, "%g, ", sqlite3_column_double(res, i)); //Formatea y almacena en aux el valor de la columna de float
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

//Para agregar msg a la DB
void add_msg(sqlite3 **db, char *msg, int *lastid)
{
    int rc;                     //Para recibir success o error de las funciones sqlite3
    char comando[MAXLINE];      //Comando para la DB
    char *err_msg = 0;          //Mensaje de error

    rc = sqlite3_open("test.db", db);   //Abre la DB, si no existe, la crea

    if(rc != SQLITE_OK)         //Handlea error
    {
        fprintf(stderr, "No se puede abrir base de datos: %s\n", sqlite3_errmsg(*db));
        sqlite3_close(*db);
        exit(EXIT_FAILURE);
    }

    //Formatea la cadena y la almacena en comando. comando tiene la instruccion SQL entera
    sprintf(comando,"INSERT INTO Mensajes VALUES(%d, '%s');",  (*(lastid) + 1), msg);
    
    rc = sqlite3_exec(*db, comando, 0, 0, &err_msg);    //Ejecuta la consulta, guardando asi la data en el DB

    if(rc != SQLITE_OK)         //Handlea error
    {
        fprintf(stderr, "SQL Error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(*db);
        exit(EXIT_FAILURE);
    }

    *(lastid) = *(lastid) + 1;
}