HEADERS = Common.h Handler.h Contador.h Config_Socket_Unix.h Config_Socket_IPv4.h Config_Socket_IPv6.h Server_Unix.h Server_IPv4.h Server_IPv6.h Verificar_Argumentos_Server.h Thread_Conexion.h Funciones_Server.h Estructuras.h Pool_Conex_Thread.h
OBJFILES = Common.o Handler.o Contador.o Config_Socket_Unix.o Config_Socket_IPv4.o Config_Socket_IPv6.o Server_Unix.o Server_IPv4.o Server_IPv6.o Verificar_Argumentos_Server.o Thread_Conexion.o Funciones_Server.o Estructuras.o Pool_Conex_Thread.o

all: Server.o $(OBJFILES) $(HEADERS)
	gcc -o Server Server.o $(OBJFILES) -lpthread -lrt -lsqlite3 -std=c99

Server.o: Server.c
	gcc -c -Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11 -lpthread -lrt -lsqlite Server.c

Common.o: Common.c 
	gcc -c -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 -lrt Common.c 

Handler.o: Handler.c 
	gcc -c -g -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Handler.c 

Contador.o: Contador.c 
	gcc -c -g -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Contador.c 

Config_Socket_Unix.o: Config_Socket_Unix.c 
	gcc -c -g -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Config_Socket_Unix.c

Config_Socket_IPv4.o: Config_Socket_IPv4.c 
	gcc -c -g -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Config_Socket_IPv4.c

Config_Socket_IPv6.o: Config_Socket_IPv6.c 
	gcc -c -g -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Config_Socket_IPv6.c

Server_Unix.o: Server_Unix.c
	gcc -c -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Server_Unix.c

Server_IPv4.o: Server_IPv4.c
	gcc -c -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Server_IPv4.c

Server_IPv6.o: Server_IPv6.c
	gcc -c -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Server_IPv6.c

Verificar_Argumentos_Server.o: Verificar_Argumentos_Server.c
	gcc -c -g -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 Verificar_Argumentos_Server.c

Funciones_Server.o: Funciones_Server.c
	gcc -c -Wall -lpthread -pedantic -Werror -Wextra -Wconversion -std=gnu11 -lsqlite3 -lrt Funciones_Server.c

Pool_Conex_Thread.o: Pool_Conex_Thread.c
	gcc -c -Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11 -lpthread -lrt -lsqlite Pool_Conex_Thread.c
	
Thread_Conexion.o: Thread_Conexion.c
	gcc -c -Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11 -lpthread -lrt -lsqlite Thread_Conexion.c

Estructuras.o: Estructuras.c
	gcc -c -Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11 -lpthread -lrt -lsqlite Estructuras.c

clean:
	rm -f Server arch *.o *.txt

testServer: 
	./Server 192.168.122.1 1900 /home/$(USER)/UNIX_ICP/  ::1 1800 enp2s0 127.0.0.1 1800 12
