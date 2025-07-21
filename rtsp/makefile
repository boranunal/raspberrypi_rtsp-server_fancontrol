# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall

# GStreamer and RTSP server pkg-config flags
PKGFLAGS = $(shell pkg-config --cflags --libs gstreamer-1.0 gstreamer-rtsp-server-1.0)

# Source and target
SRC = rtspstream.c
TARGET = rtspstream

# Default target
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(PKGFLAGS)

# Clean build files
clean:
	rm -f $(TARGET)

.PHONY: all clean
