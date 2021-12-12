editor: rawsockets.o main_client.o main_server.o list.o mensagem.o directory.o error.o ver.o linha.o linhas.o edit.o compilar.o
	g++ -o client main_client.o rawsockets.o list.o mensagem.o directory.o error.o ver.o linha.o edit.o linhas.o compilar.o
	g++ -o server main_server.o rawsockets.o list.o mensagem.o directory.o error.o ver.o linha.o edit.o linhas.o

main_client.o: main_client.cpp rawsockets.o
	g++ -c -o main_client.o main_client.cpp

main_server.o: main_server.cpp rawsockets.o
	g++ -c -o main_server.o main_server.cpp

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

linhas.o: linhas.cpp linhas.h
	g++ -c -o linhas.o linhas.cpp

edit.o: edit.cpp edit.h
	g++ -c -o edit.o edit.cpp

compilar.o: compilar.cpp compilar.h
	g++ -c -o compilar.o compilar.cpp

rawsockets.o: rawsockets.c rawsockets.h
	gcc -c -o rawsockets.o rawsockets.c

clean:
	rm -rf *.o server client
	clear