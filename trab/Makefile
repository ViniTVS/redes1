editor: rawsockets.o main.o list.o mensagem.o directory.o error.o ver.o
	g++ -o editor main.o rawsockets.o list.o mensagem.o directory.o error.o ver.o

main.o: main.cpp
	g++ -c -o main.o main.cpp

mensagem.o: mensagem.cpp mensagem.h
	g++ -c -o mensagem.o mensagem.cpp

list.o: list.cpp list.h
	g++ -c -o list.o list.cpp

error.o: error.cpp error.h
	g++ -c -o error.o error.cpp

directory.o: directory.cpp directory.h
	g++ -c -o directory.o directory.cpp

ver.o: ver.cpp ver.h
	g++ -c -o ver.o ver.cpp

rawsockets.o: rawsockets.c rawsockets.h
	gcc -c -o rawsockets.o rawsockets.c

clean:
	rm -rf *.o editor 
	clear