OPCIONS = -O0 -g -Wextra -Wno-sign-compare -std=c++14 -Wno-deprecated -I /home/bruna/ESIN/projecte/incl -lesin

driver_easydial.exe: driver_easydial.o call_registry.o phone.o easy_dial.o dialog.o
	g++ -o driver_easydial.exe driver_easydial.o call_registry.o phone.o easy_dial.o dialog.o $(OPCIONS)

driver_easydial.o: driver_easydial.cpp 
	g++ -c driver_easydial.cpp $(OPCIONS)

easy_dial.o: easy_dial.cpp easy_dial.hpp easy_dial.rep
	g++ -c easy_dial.cpp $(OPCIONS)

phone.exe: phone.o
	g++ -o phone.exe phone.o $(OPCIONS)

phone.o: phone.cpp phone.hpp
	g++ -c phone.cpp $(OPCIONS)
	
dialog.o: dialog.cpp dialog.hpp
	g++ -c dialog.cpp $(OPCIONS)

call_registry.exe: call_registry.o phone.o
	g++ -o call_registry.exe call_registry.o phone.o $(OPCIONS)
	
call_registry.o: call_registry.cpp call_registry.hpp call_registry.rep
	g++ -c call_registry.cpp $(OPCIONS)

clean:
	rm -f *.o
	rm -f *.exe
