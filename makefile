make all: compila

compila:
	g++ main.cpp AppSettings.cpp Circle.cpp Rectangle.cpp tinyxml2.cpp -o trabalhocg -lglut -lGL -lGLU

clean:
	rm *.o trabalhocg
