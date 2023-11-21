OPCIONS = -O0 -g -Wall -Wextra -Werror -Wno-sign-compare -std=c++14 -ansi -I /home/bruna/ESIN/projecte/incl

program.exe: main.o mcj_enters.o
	g++ -o program.exe main.o dialog.o easy_dial.o call_registry.o phone.o -L /home/bruna/ESIN/projecte/lib -lesin
	rm *.o

solution.exe: main.o mcj_enters.o
	g++ -o solution.exe main.o mcj_enters.o
	rm *.o

main.o: main.cpp mcj_enters.hpp mcj_enters.rep
	g++ -c main.cpp $(OPCIONS)

easy_dial.o: easy_dial.cpp easy_dial.hpp easy_dial.rep
	g++ -c easy_dial.cpp $(OPCIONS)
	


phone.exe: phone.o
	g++ -c phone.exe phone.o -lesin $(OPCIONS)

phone.o: phone.cpp phone.hpp
	g++ -c phone.cpp $(OPCIONS)
	
dialog.o: dialog.cpp dialog.hpp
	g++ -c dialog.cpp $(OPCIONS)
	
call_registry.o: call_registry.cpp call_registry.hpp phone.rep
	g++ -c phone.cpp $(OPCIONS)

clean:
	rm *.o
	rm *.exe
	rm *.gch
