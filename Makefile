default: bin/main.exe
	echo "make successfully!"
clean:
	rm bin/*
run: bin/main.exe
	bin/main.exe

bin/main.exe: bin/main.o bin/SimJoiner.o
	g++ bin/main.o bin/SimJoiner.o -o bin/main.exe -O3 -Wall
bin/main.o: src/main.cpp src/SimJoiner.h
	g++ src/main.cpp -c -o bin/main.o -O3 -Wall
bin/SimJoiner.o: src/SimJoiner.cpp src/SimJoiner.h
	g++ src/SimJoiner.cpp -c -o bin/SimJoiner.o -O3 -Wall

gen2: bin/GenData2.exe
	bin/GenData2.exe
gen3: bin/GenData3.exe
	bin/GenData3.exe
gen4: bin/GenData4.exe
	bin/GenData4.exe

bin/GenData2.exe: gen/GenData2.cpp
	g++ gen/GenData2.cpp -o bin/GenData2.exe -O3 -Wall
bin/GenData3.exe: gen/GenData3.cpp
	g++ gen/GenData3.cpp -o bin/GenData3.exe -O3 -Wall
bin/GenData4.exe: gen/GenData4.cpp
	g++ gen/GenData4.cpp -o bin/GenData4.exe -O3 -Wall
