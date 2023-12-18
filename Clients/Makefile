HEADERS = Common.h 
OBJFILES = Common.o 

all: Client_A.o Client_B.o Client_C.o $(OBJFILES) $(HEADERS)
	gcc -o Client_A Client_A.o $(OBJFILES)
	gcc -o Client_B Client_B.o $(OBJFILES)
	gcc -o Client_C Client_C.o $(OBJFILES) -lsqlite3 -lrt

Common.o: Common.c 
	gcc -c -Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11 -lsqlite3 -lrt Common.c 

Client_A.o: Client_A.c 
	gcc -c -Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11 Client_A.c

Client_B.o: Client_B.c 
	gcc -c -Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11 Client_B.c

Client_C.o: Client_C.c 
	gcc -c -Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11 -lsqlite3 -lrt Client_C.c