cc = g++
cflags = -Wall -Wextra -O2 -std=c++17 -g

main: main.cpp
	$(cc) $(cflags) $< -o $@