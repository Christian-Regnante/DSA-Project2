# DSA Project 2 Assignment

This repository contains five standalone C programs demonstrating core data
structures and graph algorithms. Each project is located in its own directory
and includes its implementation, header files, Makefile, and detailed
documentation.

## Projects

1. **Airport Baggage Handling Priority Queue**  
   Implements an array-based binary max-heap for dynamically prioritizing
   baggage containers.
2. **Hospital Emergency Triage Priority System**  
   Uses a binary max-heap to order patients by medical priority score.
3. **EV Charging Station Power Network**  
   Uses an adjacency matrix, disjoint-set union, and Kruskal's algorithm to
   calculate a minimum spanning tree.
4. **IoT Gateway Connectivity Analyzer**  
   Uses a weighted adjacency matrix and breadth-first search to analyze gateway
   connectivity and identify the slowest direct link.
5. **Cloud Service Data Routing Analyzer**  
   Uses the Bellman–Ford algorithm to calculate shortest paths in a directed
   graph containing negative edge weights and to detect negative cycles.

## Repository structure

Each project follows the same general layout:

```text
project-directory/
├── Makefile
├── README.md
├── main.c
├── algorithm implementation.c
└── algorithm interface.h
```

The `main.c` file prepares the sample data and runs the demonstration. The
implementation and header files contain the project-specific data structures,
algorithms, and reporting functions. The individual project README files
contain the complete explanations, traces, expected results, and algorithm
analysis.

The combined Word document, `Project2_Assignment.docx`, provides the project
documentation in one formatted file.

## Requirements

- GCC
- GNU Make
- A C99-compatible build environment

The Makefiles compile with strict C99 settings:

```text
-Wall -Wextra -Werror -pedantic -std=c99 -g
```

## Building and running a project

There is no root-level build target. Run Make from the directory of the
project you want to use, or pass that directory with `make -C`.

For example, to build and run Question 1 from the repository root:

```bash
make -C q1-airport_baggage_handling_priority_queue
make -C q1-airport_baggage_handling_priority_queue run
```

The equivalent commands from inside the project directory are:

```bash
make
make run
```

The project executables are:

| Project | Executable |
|---|---|
| Q1 | `baggage_priority_queue` |
| Q2 | `triage_priority_system` |
| Q3 | `ev_network_optimizer` |
| Q4 | `gateway_analyzer` |
| Q5 | `routing_analyzer` |

Question 1, Question 2, Question 3, and Question 5 run with predefined sample
inputs. Question 4 is interactive and asks for a starting gateway from `A`
through `G`. It can also receive input through a pipe:

```bash
printf 'D\n' | make -C q4-IOT_gateway_connectivity_analyzer run
```

## Cleaning build files

Each Makefile provides a `clean` target that removes generated object files
and the executable for that project:

```bash
make -C q1-airport_baggage_handling_priority_queue clean
```

Replace the directory name with the project you want to clean.

## Testing

The projects do not use a shared test framework. Their `main.c` programs
demonstrate the required algorithm operations and output. Question 5 also
provides a dedicated test suite:

```bash
make -C q5-claude_service_data_routing_analyzer test
```
