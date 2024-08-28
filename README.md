# Graph Block Size Computation

This project provides a utility to compute the number of connected components (blocks) in a graph based on input from `stdin`. The graph is described by a sequence of integers where the first number represents the number of vertices, and each subsequent pair of numbers represents an edge between two vertices.

## Project Description

The program reads a graph from standard input and calculates the number of connected components, or "blocks," in the graph. The vertices of the graph are represented by integers from 1 to `n`.

### Input Format

- The first integer in the input is the number of vertices `n`.
- Each subsequent pair of integers represents an edge connecting two vertices.
  
For example, consider the following input:

```
4 
1 4
1 3
4 3
```


This represents a graph with four vertices, where three of them form a triangle, and the fourth vertex is isolated.

### Output

The program calculates and returns the number of connected components (blocks) in the graph.

## Installation

To compile the code, just type `make` in your terminal. You can clean up all the files just by typing `make clean` in your terminal

## Usage

Once compiled, the program can be executed by passing the path to a file containing the graph description as a command-line argument.

### Example

To run the program with an input file graph.txt that contains the following:
```
4 
1 4 
1 3
4 3
```

Use the following command:
`./findBlocks graph.txt`

This will output the number of connected components in the graph described in `graph.txt`.

## Files

- `findBlocks.c`: The main implementation of the graph block computation.
- `findBlocks.h`: Header file containing declarations used in `findBlocks.c`.

## How It Works
The program utilizes a depth-first search (DFS) to explore the graph and count the number of connected components. Each time a new vertex is found that hasn't been visited, a new DFS is initiated, representing the discovery of a new connected component.

## License
This project is open-source and available under the [MIT License](LICENSE).