CC = "g++"
PROJECT = Pendulum
SRC = main.cpp

LIBS = `pkg-config --cflags --libs opencv4` 
$(PROJECT) : $(SRC)
	$(CC) $(SRC) -o $(PROJECT) $(LIBS)
	./Pendulum
	g++ -o plot metersplot.cpp
	./plot
