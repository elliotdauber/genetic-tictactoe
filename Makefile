CC = /usr/local/opt/llvm/bin/clang++ --std=c++20
CXXFLAGS = -O0 -Wall -I/usr/local/opt/libomp/include -fopenmp -g -std=c++20 -Igenmodlib -Iincludes
LDFLAGS = -L/usr/local/opt/llvm/lib

VPATH = src
SRC = $(wildcard src/*.cc)

PLAYTARGET = play
TRAINTARGET = train

all: $(PLAYTARGET) $(TRAINTARGET)

playy: $(PLAYTARGET)

trainn: $(TRAINTARGET)

$(PLAYTARGET): $(PLAYTARGET).cc $(SRC)
			$(CC) $(CXXFLAGS) -o $(PLAYTARGET) $(PLAYTARGET).cc $(SRC)

$(TRAINTARGET): $(TRAINTARGET).cc $(SRC)
			$(CC) $(CXXFLAGS) -o $(TRAINTARGET) $(TRAINTARGET).cc $(SRC)

clean:
		$(RM) $(PLAYTARGET) && $(RM) $(TRAINTARGET)
