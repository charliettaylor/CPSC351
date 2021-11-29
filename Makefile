
GXX49_VERSION := $(shell g++-4.9 --version 2>/dev/null)
MAKEFLAGS += -s

ifdef GXX49_VERSION
	CXX_COMMAND := g++-4.9
else
	CXX_COMMAND := g++
endif

CXX = ${CXX_COMMAND} -std=c++11 

producer: Assignment1/producer.cpp
	${CXX} Assignment1/producer.cpp -o producer -lrt
	./producer
	make clean

start: Assignment1/start.cpp
	${CXX} Assignment1/start.cpp -o start -lrt
	./start
	make clean

start2: Assignment2/main.cpp
	${CXX} Assignment2/main.cpp -o main -lpthread
	.Assignment2/main
	make clean

clean:
	rm -f start
	rm -f producer
	rm -f main