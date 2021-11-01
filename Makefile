SRCS=hdastar.cpp util.cpp
HEADERS=hdastar.hpp util.hpp

main: main.cpp $(SRCS) $(HEADERS)
	g++ main.cpp $(SRCS) -g -o $@ 

exp: experiment.cpp $(SRCS) $(HEADERS)
	g++ experiment.cpp $(SRCS) -o $@ -O3

.PHONY: clean
clean: 
	rm main exp