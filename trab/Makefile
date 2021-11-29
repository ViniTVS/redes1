editor: rawsockets.o main.o list.o mensagem.o directory.o
	g++ -o editor main.o rawsockets.o list.o mensagem.o directory.o

main.o: main.cpp
	g++ -c -o main.o main.cpp

mensagem.o: mensagem.cpp mensagem.h
	g++ -c -o mensagem.o mensagem.cpp

list.o: list.cpp list.h
	g++ -c -o list.o list.cpp

directory.o: directory.cpp directory.h
	g++ -c -o directory.o directory.cpp

rawsockets.o: rawsockets.c rawsockets.h
	gcc -c -o rawsockets.o rawsockets.c

clean:
	rm -rf *.o editor 
	clear