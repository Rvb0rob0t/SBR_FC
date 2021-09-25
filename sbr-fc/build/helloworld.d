build/helloworld.o: src/helloworld.cpp
	g++ -g -Wall -Werror -Wno-unused -std=c++17 -I include  -c src/helloworld.cpp -o build/helloworld.o