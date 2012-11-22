TARGET = example
all: $(TARGET)

$(TARGET): $(TARGET).o
	clang -o $@ $? -ldirutils
clean:
	rm -f *.o $(TARGET)

.PHONY: all clean
