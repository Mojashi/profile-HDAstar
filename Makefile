main: main.cpp hdastar.cpp hdastar.hpp
	g++ main.cpp hdastar.cpp -g -o $@ 

.PHONY: clean
clean: 
	rm main