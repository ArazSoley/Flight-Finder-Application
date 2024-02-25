CC=g++
LFLAGS=
CFLAGS=-c -g -std=c++11 -Wall -Wconversion -pedantic-errors -O0

.PHONY: all clean

all: TC_HashTable TC_CENGFlight TC_MultiGraph

clean:
	rm *o
	rm TC_CENGFlight TC_MultiGraph TC_HashTable

# Hash Table
# Hash Table is header only (due to templates)
HASH_T_FILES= HashTable.h HashTableImpl.h IntPair.h Exceptions.h

# Executables
TC_CENGFlight: mainFlight.o MultiGraph.o CENGFlight.o
	$(CC) $(LFLAGS) mainFlight.o MultiGraph.o CENGFlight.o -o TC_CENGFlight

TC_MultiGraph: mainGraph.o MultiGraph.o
	$(CC) $(LFLAGS) mainGraph.o MultiGraph.o -o TC_MultiGraph

TC_HashTable: mainHTable.o
	$(CC) $(LFLAGS) mainHTable.o -o TC_HashTable

# Objects
# Mains
mainFlight.o: mainFlight.cpp $(HASH_T_FILES)
	$(CC) $(CFLAGS) -o mainFlight.o mainFlight.cpp

mainHTable.o: mainHTable.cpp $(HASH_T_FILES)
	$(CC) $(CFLAGS) -o mainHTable.o mainHTable.cpp

mainGraph.o: mainGraph.cpp
	$(CC) $(CFLAGS) -o mainGraph.o mainGraph.cpp

# Objects
MultiGraph.o: MultiGraph.cpp MultiGraph.h Exceptions.h
	$(CC) $(CFLAGS) -o MultiGraph.o MultiGraph.cpp

CENGFlight.o: CENGFlight.cpp CENGFlight.h $(HASH_T_FILES) MultiGraph.h
	$(CC) $(CFLAGS) -o CENGFlight.o CENGFlight.cpp
