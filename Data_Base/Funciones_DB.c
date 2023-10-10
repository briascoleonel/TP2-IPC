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