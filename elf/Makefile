all:	extract-sect

extract-sect: extract-sect.cpp extract-sect.hpp fmapper.hpp     
	g++ -O3 -std=c++17 -o extract-sect extract-sect.cpp

clean:
	rm -f extract-sect *~ *.o core
