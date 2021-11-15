editor: rawsockets.o main.o ls.o mensagem.o
	g++ -o editor main.o rawsockets.o ls.o mensagem.o

main.o: main.cpp
	g++ -c -o main.o main.cpp

mensagem.o: mensagem.cpp
	g++ -c -o mensagem.o mensagem.cpp

ls.o: ls.cpp
	g++ -c -o ls.o ls.cpp

rawsockets.o: rawsockets.c
	gcc -c -o rawsockets.o rawsockets.c

clean:
	rm -rf *.o editor 
	clear