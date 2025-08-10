CC 	?= cc
PREFIX  ?= /usr

NAME	 = musical-donut
TARGET	 = donut
VERSION	 = 0.0.3
SRC 	 = $(wildcard src/*.c)
OBJ	 = $(SRC:.c=.o)
CFLAGS	 = -Iinclude -DVERSION=\"$(VERSION)\"
LDLIBS   = -lm

all: $(TARGET)


$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(OBJ) $(TARGET)

install:
	install $(TARGET) -Dm 755 -v $(DESTDIR)$(PREFIX)/bin/$(TARGET)
