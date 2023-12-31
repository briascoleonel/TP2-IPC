#include "Common.h"
#include "Funciones_Server.h"
#include "Estructuras.h"
#include "Verificar_Argumentos_Server.h"
/*
argv[1]  = Dir IPv4
argv[2]  = Puerto IPv4
argv[3]  = File path
argv[4]  = Dir IPv6
argv[5]  = Puerto IPv6
argv[6]  = Interfaz IPv6
argv[7]  = Dir IPv4 CP
argv[8]  = Puerto IPv4 CP 
argv[9]  = Cantidad de Clientes
*/

void verificar_argumentos(int argc, char *argv[])
{
    //Cantidad de argumentos a pasar tiene que ser igual a 10
    if(argc!=10)
    {
        printf("Cantidad de argumentos invalida\n");
        exit(EXIT_FAILURE);
    }

    //Verificacion de cada protocolo
    Verificar_Argumentos_UNIX(argv);
    Verificar_Argumentos_IPv4(argv);
    Verificar_Argumentos_IPv6(argv);

    //Cantidad de clientes tiene que ser un numero positivo, no letra y menor al maximo establecido
    for(unsigned int i = 0; i < strlen(argv[9]); i ++)
    {
        if((isdigit(argv[9][i]) == 0)|| (atoi(argv[9]) < 0) || (atoi(argv[9]) > 10000) || strlen(argv[9]) > sizeof(int)) // Verificar que no se hayan ingresado letras, 
        {
            printf("Debe ingresar una cantidad de clientes correcta\n");
            exit(EXIT_FAILURE);
        }
    }
}

void Verificar_Argumentos_UNIX(char *argv[])
{
    //El nombre del archivo UNIX tiene que ser correcto
    if((strlen(argv[3])>MAXLINE) || (!filename_valido(argv[3])))
    {
        printf("Nombre de archivo invalido\n");
        exit(EXIT_FAILURE);
    } 
}

void Verificar_Argumentos_IPv4(char *argv[])
{
    //La direccion IP debe ser correcta
    if(!dir_IPv4_valida(argv[1]))
    {
        printf("Debe ingresar una direccion IPv4 correcta\n");
        exit(EXIT_FAILURE);
    }

    //El puerto tiene que ser digitos y estar entre 0 y 65535
    for(unsigned int i = 0; i < strlen(argv[2]); i++)
    {
        if((isdigit(argv[2][i]) == 0) || (atoi(argv[2]) <= 0) || (atoi(argv[2])>65535))
        {
            printf("Debe ingresar un puerto correcto\n");
            exit(EXIT_FAILURE);
        }
    }

    //La direccion IP debe ser correcta (CP)
    if(!dir_IPv4_valida(argv[7]))
    {
        printf("Debe ingresar una direccion IPv4 correcta\n");
        exit(EXIT_FAILURE);
    }

    //El puerto tiene que ser digitos y estar entre 0 y 65535
    for(unsigned int i = 0; i < strlen(argv[8]); i++)
    {
        if((isdigit(argv[8][i]) == 0) || (atoi(argv[8]) <= 0) || (atoi(argv[8])>65535))
        {
            printf("Debe ingresar un puerto correcto\n");
            exit(EXIT_FAILURE);
        }
    }

}

void Verificar_Argumentos_IPv6(char *argv[])
{
    //La direccion IP debe ser correcta
    if(!dir_IPv6_valida(argv[4]))
    {
        printf("Debe ingresar una direccion IPv6 valida\n");
        exit(EXIT_FAILURE);
    }

    //El puerto tiene que ser digitos y estar entre 0 y 65535
    for(unsigned int i = 0; i < strlen(argv[5]); i++)
    {
        if((isdigit(argv[5][i]) == 0) || (atoi(argv[5]) <= 0) || (atoi(argv[5]) > 65535))
        {
            printf("Debe ingresar un puerto correcto\n");
            exit(EXIT_FAILURE);
        }
    }

    //La interfaz tiene que estar compuesta por numero y letras
    for(unsigned int i = 0; i < strlen(argv[6]); i ++)
    {
        if((isdigit((argv[6][i]) == 0) && (isalpha(argv[6][i]) == 0)) || strlen(argv[6])>MAXLINE) 
        {                                                               
            printf("Debe ingresar una interfaz correcta\n");               
            exit(EXIT_FAILURE);
        }
    }
}

