
objects = main.o
cc = g++

httpserver : $(objects)
	g++ -o client $(objects) -lpthread
#main.o : main.cpp
#	cc -c main.cpp
	
.PHONY : clean
clean :
	rm edit $(objects)
