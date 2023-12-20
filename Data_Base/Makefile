HEADERS = Common.h Funciones_DB.h Server_IPv4.h Handler.h Config_Socket_IPv4.h Verificar_Argumentos_Server.h
OBJFILES = Common.o Funciones_DB.o Server_IPv4.o Handler.o Config_Socket_IPv4.o Verificar_Argumentos_Server.o

all: Data_Base.o $(OBJFILES) $(HEADERS)
	gcc -o Data_Base Data_Base.o $(OBJFILES) -lpthread -lrt -lsqlite3 -std=c99

Data_Base.o: Data_Base.c 
	gcc -c -Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11 -lpthread -lrt -lsqlite Data_Base.c

Common.o: Common.c 
	gcc -c -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 -lsqlite3 -lrt Common.c 

Funciones_DB.o: Funciones_DB.c 
	gcc -c -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -lsqlite3 -std=c99 -lrt Funciones_DB.c 

Handler.o: Handler.c 
	gcc -c -g -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Handler.c

Config_Socket_IPv4.o: Config_Socket_IPv4.c 
	gcc -c -g -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Config_Socket_IPv4.c

Verificar_Argumentos_Server.o: Verificar_Argumentos_Server.c
	gcc -c -g -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Verificar_Argumentos_Server.c

test:
	./Data_Base 127.0.0.1 1800

clean:
	rm -f data_base test.db *.o *.txt
