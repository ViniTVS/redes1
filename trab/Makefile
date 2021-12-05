editor: rawsockets.o main.o list.o mensagem.o directory.o error.o ver.o linha.o edit.o
	g++ -o editor main.o rawsockets.o list.o mensagem.o directory.o error.o ver.o linha.o edit.o

main.o: main.cpp rawsockets.o
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

linha.o: linha.cpp linha.h
	g++ -c -o linha.o linha.cpp

edit.o: edit.cpp edit.h
	g++ -c -o edit.o edit.cpp

rawsockets.o: rawsockets.c rawsockets.h
	gcc -c -o rawsockets.o rawsockets.c

clean:
	rm -rf *.o editor 
	clear