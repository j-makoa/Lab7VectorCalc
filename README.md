# Vector Calculator

## Description
A vector calculator written in C. The user enters commands at a
prompt to create, store, and operate on named 3D vectors. This works in a matlab esque way where you are able to create them in the command line and interact using commands.

## Building
Requires `gcc` and `make`.

1. Clone the repository
2. Run `make` in the project directory
3. Run `make clean` to remove build files

### Command line options
- `-h` : display the help message

## Supported Commands
- `a = 1 2 3` : create/assign vector `a`
- `a + b` : add two vectors
- `a - b` : subtract two vectors
- `a * 2` : scalar multiply
- `list` : show all stored vectors
- `clear` : remove all vectors
- `load <file>` : load vectors from a CSV file
- `save <file>` : save vectors to a CSV file
- `quit` : exit the program

## Dynamic Memory Usage
- Vectors are stored in an array allocated with `malloc()` on the heap
- When the array fills up, it is grown with `realloc()` so there is no fixed limit
- `clear` frees the storage and resets the count
- All memory is released with `free()` before the program exits