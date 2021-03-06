all: tune tighten loosen readFrequency reset tension bringtension inittune

tune: source/tune.o source/stringPCControls.o
	g++ -o bin/tune source/tune.o source/stringPCControls.o -lserial -lfftw3 -lm	

inittune: source/inittune.o source/stringPCControls.o
	g++ -o bin/inittune source/inittune.o source/stringPCControls.o -lserial -lfftw3 -lm

bringtension: source/bringtension.o source/stringPCControls.o
	g++ -o bin/bringtension source/bringtension.o source/stringPCControls.o -lserial -lfftw3 -lm	

tighten: source/tighten.o source/stringPCControls.o
	g++ -o bin/tighten source/tighten.o source/stringPCControls.o -lserial -lfftw3 -lm


readFrequency: source/readFrequency.o source/stringPCControls.o
	g++ -o bin/readFrequency source/readFrequency.o source/stringPCControls.o -lserial -lfftw3 -lm

loosen: source/loosen.o source/stringPCControls.o
	g++ -o bin/loosen source/loosen.o source/stringPCControls.o -lserial -lfftw3 -lm


reset: source/reset.o source/stringPCControls.o
	g++ -o bin/reset source/reset.o source/stringPCControls.o -lserial -lfftw3 -lm
	
tension: source/tension.o source/stringPCControls.o
	g++ -o bin/tension source/tension.o source/stringPCControls.o -lserial -lfftw3 -lm	


tune.o: source/tune.cpp
	g++ -c source/tune.cpp -lserial -lfftw3 -lm

inittune.o: source/inittune.cpp
	g++ -c source/inittune.cpp -lserial -lfftw3 -lm

tighten.o: source/tighten.cpp
	g++ -c source/tighten.cpp -lserial -lfftw3 -lm

loosen.o: source/loosen.cpp
	g++ -c source/loosen.cpp -lserial -lfftw3 -lm

readFrequency.o: source/readFrequency.cpp
	g++ -c source/readFrequency.cpp -lserial -lfftw3 -lm

stringPCControls.o: source/stringPCControls.cpp
	g++ -c source/stringPCControls.cpp -lserial -lfftw3 -lm

bringtension.o: source/bringtension.cpp
	g++ -c source/bringtension.cpp -lserial -lfftw3 -lm

reset.o: source/reset.cpp
	g++ -c source/reset.cpp 

tension.o: source/tension.cpp
	g++ -c source/tension.cpp 

clean:
	rm source/tune.o 
	rm source/stringPCControls.o
	rm source/readFrequency.o
	rm source/tighten.o
	rm source/loosen.o
	rm source/reset.o
	rm source/tension.o
	rm source/bringtension.o
	rm source/inittune.o
