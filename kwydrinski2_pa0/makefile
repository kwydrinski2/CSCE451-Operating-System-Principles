EXECUTABLES = newecho newcat

all: $(EXECUTABLES)

newecho : newecho.o
	gcc -o newecho newecho.o

newcat : newcat.o
	gcc -o newcat newcat.o

newcat.o : newcat.c 
	gcc -c newcat.c

newecho.o : newecho.c
	gcc -c newecho.c

clean :
	rm -f $(EXECUTABLES) *.o


