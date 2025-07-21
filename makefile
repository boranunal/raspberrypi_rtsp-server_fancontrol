# Makefile for Raspberry Pi PWM Fan Control

# Compiler and flags
CC = gcc
CFLAGS = -Wall -pthread -lpigpio -lrt

# Target binary
TARGET = rpipwmfan

# Source files
SRCS = rpipwmfan.c
OBJS = $(SRCS:.c=.o)

# Build target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

%.o: %.c rpipwmfan.h
	$(CC) -c $< $(CFLAGS)

# Clean target
clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
