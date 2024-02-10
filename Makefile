CC = x86_64-w64-mingw32-g++-posix

SOURCES := $(wildcard ./*.cpp)
OBJECTS := $(SOURCES:.cpp=.o)

compile-unix:
	g++ -c $(SOURCES) -I.
	g++ $(OBJECTS) -o app -lsfml-graphics -lsfml-window -lsfml-system

compile-win:
	$(CC) -c $(SOURCES) -I/mingw64/include -I/mingw64/include/SFML -I. -m64
	$(CC) $(OBJECTS) -o app.exe -L/mingw64/lib -lsfml-graphics -lsfml-window -lsfml-system -I.

run:
	./app