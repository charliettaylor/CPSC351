
GXX49_VERSION := $(shell g++-4.9 --version 2>/dev/null)

ifdef GXX49_VERSION
	CXX_COMMAND := g++-4.9
else
	CXX_COMMAND := g++
endif

CXX = ${CXX_COMMAND} -std=c++11 

run_start: start
	./start

run_producer: producer
	./producer

start: Assignment1/start.cpp
	${CXX} Assignment1/start.cpp -o start -lrt

producer: Assignment1/producer.cpp
	${CXX} Assignment1/producer.cpp -o producer -lrt

clean:
	rm -f start