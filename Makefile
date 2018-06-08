default: bin/main.exe
	echo "make successfully!"

clean:
	rm bin/*

run:
	bin/main.exe

bin/main.exe: bin/main.o bin/SimJoiner.o
	g++ bin/main.o bin/SimJoiner.o -o main.exe -O3

bin/main.o: src/main.cpp src/SimJoiner.h
	g++ src/main.cpp -c -o bin/main.o

bin/SimJoiner.o: src/SimJoiner.cpp src/SimJoiner.h
	g++ src/SimJoiner.cpp -c -o bin/SimJoiner.o
