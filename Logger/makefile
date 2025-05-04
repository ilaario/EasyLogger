CC      := gcc
CFLAGS  := -Wall -O2 -fPIC -dLINUX -Iinclude
TARGET  := logger.so      # cambia in .a se vuoi statica
OBJ     := logger.o

$(TARGET): $(OBJ)
	$(CC) -shared -o $@ $^

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)