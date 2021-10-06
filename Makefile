
GXX49_VERSION := $(shell g++-4.9 --version 2>/dev/null)

ifdef GXX49_VERSION
	CXX_COMMAND := g++-4.9
else
	CXX_COMMAND := g++
endif

CXX = ${CXX_COMMAND} -std=c++11 

run_start: start
	./start


start: Assignment1/start.cpp
	${CXX} Assignment1/start.cpp -o start -lrt

clean:
	rm -f start