CC=g++ -std=c++11 -g -O3
CFLAGS=-Wall 

all: util.o hash.o lsh_func.o hypercube_func.o kmeans_func.o lsh_main.o hypercube_main.o kmeans_main.o lsh cube kmeans
util.o: util.cpp
	$(CC) $(CFLAGS) -c util.cpp -o util.o
hash.o: hash.cpp
	$(CC) $(CLFAGS) -c hash.cpp -o hash.o
	
lsh_func.o: lsh_func.cpp
	$(CC) $(CLFAGS) -c lsh_func.cpp -o lsh_func.o
hypercube_func.o: hypercube_func.cpp
	$(CC) $(CLFAGS) -c hypercube_func.cpp -o hypercube_func.o
kmeans_func.o: kmeans_func.cpp
	$(CC) $(CLFAGS) -c kmeans_func.cpp -o kmeans_func.o
	
lsh_main.o: lsh_main.cpp
	$(CC) $(CLFAGS) -c lsh_main.cpp -o lsh_main.o
hypercube_main.o: hypercube_main.cpp
	$(CC) $(CLFAGS) -c hypercube_main.cpp -o hypercube_main.o
kmeans_main.o: kmeans_main.cpp
	$(CC) $(CLFAGS) -c kmeans_main.cpp -o kmeans_main.o
	
lsh: util.o hash.o lsh_func.o lsh_main.o 
	$(CC) $(CFLAGS) -o lsh util.o hash.o lsh_func.o lsh_main.o 
cube: util.o hash.o hypercube_func.o hypercube_main.o 
	$(CC) $(CFLAGS) -o cube util.o hash.o hypercube_func.o hypercube_main.o 
kmeans: util.o hash.o kmeans_func.o kmeans_main.o 
	$(CC) $(CFLAGS) -o kmeans util.o hash.o kmeans_func.o kmeans_main.o 


.PHONY: clean

clean:
	rm -f util.o hash.o lsh_func.o hypercube_func.o kmeans_func.o lsh_main.o hypercube_main.o kmeans_main.o lsh cube kmeans

