C     = g++
CFLAGS = -Wall `pkg-config --cflags --libs opencv4` -lwiringPi 

all: checkers

#mv.o: mv.cpp mv.h
#	$(C) $(CFLAGS) -c mv.cpp -o mv.o

#rules.o: rules.cpp rules.h
#	$(C) $(CFLAGS) -c rules.cpp -o rules.o

%.o: %.cpp
	$(C) $(CFLAGS) -c $^ -o $@

checkers:  main.o mv.o rules.o servo.o
	$(C) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o
	rm -f checkers
