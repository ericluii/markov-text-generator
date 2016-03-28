CC = g++
CFLAGS = -c -Wall -I.
OUTPUT = markovTextGenerator

SRC= $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

all: $(OUTPUT)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(OUTPUT): $(OBJ)
	$(CC) -o $@ $^

clean:
	rm $(OBJ) $(OUTPUT)
