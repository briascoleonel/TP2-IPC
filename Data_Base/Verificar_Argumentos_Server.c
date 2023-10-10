#include "Common.h"

/*Se usan 2 argumentos
- Direccion IPv4
- Puerto
*/
void verificar_argumentos(int argc, char *argv[])
{
    //Cantidad de argumentos a pasar tiene que ser igual a 3
    if(argc!=3)
    {
        printf("Cantidad de argumentos invalida\n");
        exit(EXIT_FAILURE);
    }

    //Verificacion de IPv4
    Verificar_Argumentos_IPv4(argv);
}

void  Verificar_Argumentos_IPv4(char *argv[])
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
}

