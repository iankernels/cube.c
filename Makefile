APP := cube
SRC := cube.c
CC  := gcc
CFLAGS := -std=c11 -Wall -Wextra -Wconversion -pedantic -O2 $(shell pkg-config --cflags sdl2)
LDFLAGS := $(shell pkg-config --libs sdl2) -lm

.PHONY: all run clean cmake cmake-build cmake-run

all: $(APP)

$(APP): $(SRC)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

run: $(APP)
	./$(APP)

clean:
	rm -f $(APP)
	rm -rf build

# CMake helpers
cmake:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

cmake-build: cmake
	cmake --build build --config Release -j

cmake-run: cmake-build
	./build/$(APP)


