lab5: lab5.o hugeint.o
	g++ -std=c++11 lab5.o hugeint.o -o lab5
lab5.o: lab5.cpp
	g++ -std=c++11 -c lab5.cpp -o lab5.o
hugeint.o: hugeint.h hugeint.cpp
	g++ -std=c++11 -c hugeint.cpp -o hugeint.o
clean:
	rm lab5.o hugeint.o
