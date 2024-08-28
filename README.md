## Index
1. [Graph Block Size Computation](#graph-block-size-computation)
2. [Project Description](#project-description)
   - [Input Format](#input-format)
   - [Output](#output)
3. [Installation](#installation)
4. [Usage](#usage)
   - [Example](#example)
5. [Files](#files)
6. [How It Works](#how-it-works)
   - [1. Graph Representation](#1-graph-representation)
   - [2. Adding Edges](#2-adding-edges)
   - [3. Depth-First Search (DFS) and Articulation Points](#3-depth-first-search-dfs-and-articulation-points)
   - [4. Block Analysis](#4-block-analysis)
   - [5. Output](#5-output)
   - [Example Walkthrough](#example-walkthrough)
7. [License](#license)


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

The program produces several outputs:
1. **Graph Adjacency List:** Prints the adjacency list of the graph.
2. **Articulation Points:** Identifies and prints the articulation points in the graph.
3. **Block Analysis:** Provides detailed information about each block, including the number of vertices and edges.
4. **Total Number of Blocks:** Outputs the total number of blocks in the graph.

Example output for a graph might look like:

```
:: GRAPH READ ::
Adjacency list of vertex 1: (head) -> 3 -> 2 
Adjacency list of vertex 2: (head) -> 4 -> 1 ...

:: ARTICULATION POINTS :: 
Articulation points in the graph: 4 7

:: BLOCKS ANALYSIS :: 
Vertex 1 added to the block 
... 
Found Block 1: Number of Vertexes 6, Number of Edges: 6 ...

TOTAL NUMBER OF BLOCKS IN THE GRAPH: 4
```

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

The program is designed to read a graph, identify articulation points, and analyze the blocks (connected components) within the graph. Here's a detailed breakdown of how it works:

### 1. Graph Representation

The graph is represented using an adjacency list. The structure `Graph` contains an array of adjacency lists, where each list represents a vertex and its connected edges.

- **Adjacency List Nodes:** Each vertex in the graph has a list of adjacent vertices (neighbors), represented by `AdjListNode`.
- **Graph Structure:** The graph itself is encapsulated in the `Graph` structure, which holds the number of vertices and an array of adjacency lists.

### 2. Adding Edges

The function `addEdge` adds an edge to the graph. Since the graph is undirected, an edge between vertex `u` and vertex `v` results in `v` being added to the adjacency list of `u` and `u` being added to the adjacency list of `v`.

### 3. Depth-First Search (DFS) and Articulation Points

The core of the graph analysis involves a Depth-First Search (DFS) traversal. The function `DFSarticulationPoints` is responsible for this traversal, during which it also identifies articulation points in the graph. Articulation points are vertices that, when removed, increase the number of connected components in the graph.

- **Discovery and Low Times:** During the DFS, each vertex is assigned a discovery time (when it is first visited) and a low value, which is the earliest visited vertex reachable from the subtree rooted at that vertex.
- **Articulation Point Criteria:** A vertex is an articulation point if:
  - It is the root of the DFS tree and has two or more children.
  - It is not the root, and there is no back edge from any descendant of this vertex to its ancestors.

### 4. Block Analysis

After identifying the articulation points, the program proceeds to analyze the blocks (biconnected components) in the graph. This is handled by a block analysis function that traverses the graph to determine the structure and composition of each block.

- **Block Construction:** The function iterates over the vertices, grouping them into blocks based on connectivity and articulation points.
- **Block Details:** For each block, the program counts the number of vertices and edges, providing a detailed output for each block.

### 5. Output

The program provides several layers of output:
- **Graph Adjacency List:** Visual representation of the graph's structure.
- **Articulation Points:** Lists the critical vertices whose removal would split the graph.
- **Block Analysis:** Detailed information about each block, including the vertices and edges it contains.
- **Total Number of Blocks:** The final count of distinct blocks within the graph.

### Example Walkthrough

For a given graph, the program:
 1. Prints the adjacency list;
 2. Identifies and lists the articulation points;
 3. Followed by a detailed analysis of each block in the graph. 
 4. Finally, it summarizes the total number of blocks.

This combination of DFS, articulation point identification, and block analysis allows for a comprehensive understanding of the graph's structure, particularly in terms of its connectivity and resilience.

## License
This project is open-source and available under the [MIT License](LICENSE).