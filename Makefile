CC = gcc

CFLAGS  = -g -Wall
TARGET = tsp-3510

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c -lm

valgrind:
		valgrind ./$(TARGET)

clean:
	$(RM) $(TARGET)
