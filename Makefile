CC = "g++"
PROJECT = PENDULUM
SRC = main.cpp

LIBS = `pkg-config --cflags --libs opencv4` 
$(PROJECT) : $(SRC)
	$(CC) $(SRC) -o $(PROJECT) $(LIBS)
	./PENDULUM
	g++ metersplot.cpp && ./a.out