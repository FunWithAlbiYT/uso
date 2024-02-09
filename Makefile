SOURCES := $(wildcard ./*.cpp)
OBJECTS := $(SOURCES:.cpp=.o)

compile:
	g++ -c $(SOURCES) -I.
	g++ $(OBJECTS) -o app -lsfml-graphics -lsfml-window -lsfml-system
	
compilewin:
	g++ -c $(SOURCES) -o $(OBJECTS) -I/mingw64/include -I/mingw64/include/SFML -L/mingw64/lib -lsfml-graphics -lsfml-window -lsfml-system
	g++ $(OBJECTS) -o app.exe -L/mingw64/lib -lsfml-graphics -lsfml-window -lsfml-system

run:
	./app