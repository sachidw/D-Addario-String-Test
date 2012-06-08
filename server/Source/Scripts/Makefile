all: control

control: source/control.o source/stringPCControls.o
	g++ -o tune source/control.o source/stringPCControls.o -lserial -lfftw3 -lm	

control.o: source/control.cpp
	g++ -c source/control.cpp -lserial -lfftw3 -lm

stringPCControls.o: source/stringPCControls.cpp
	g++ -c source/stringPCControls.cpp -lserial -lfftw3 -lm

clean:
	rm source/control.o 
	rm source/stringPCControls.o

