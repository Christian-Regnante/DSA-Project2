# Question 5: Cloud Service Data Routing Analyzer

## 1. Overview
A cloud services company operates a distributed multi-region infrastructure consisting of several data centers that exchange application telemetry, customer data, and scheduled backups. A routing analyzer determines the minimum cumulative cost of transferring data from the **Primary Data Center (A)** to every other data center across the network.

The network is modeled as a **weighted directed graph** where:
- **Vertices (Nodes):** Represent data centers (`A` through `J`).
- **Directed Edges (Links):** Represent one-way data-transfer routes between data centers.
- **Edge Weights:** Represent the net transfer cost of utilizing a specific route.
- **Negative Edge Weights:** Represent promotional transfer credits, compression optimizations, or off-peak routing discounts that decrease the overall routing cost.

This project implements the **Bellman–Ford algorithm** in C to:
1. Support networks containing both positive and negative edge weights.
2. Find the minimum cumulative cost from Primary Data Center `A` to all reachable nodes.
3. Reconstruct and display the exact hop-by-hop sequence of data centers for each route.
4. Detect and report any reachable negative-weight cycles that would render shortest-path routing undefined.

---

## 2. Graph Model & Network Topology

### 2.1 Graph Definition
- **Vertices:** $V = \{A, B, C, D, E, F, G, H, I, J\}$, $|V| = 10$
- **Source:** Data Center `A`
- **Edges:** $|E| = 15$ directed weighted routes:

| Route # | From (Source) | To (Destination) | Net Transfer Cost | Note |
|:-------:|:-------------:|:----------------:|:-----------------:|:-----|
| 1       | **A**         | **B**            | 6                 | Direct route |
| 2       | **A**         | **D**            | 16                | Direct route |
| 3       | **B**         | **C**            | 6                 | Intermediate transfer |
| 4       | **B**         | **D**            | 6                 | Alternate path to D |
| 5       | **B**         | **J**            | 7                 | Branch to J |
| 6       | **C**         | **G**            | -9                | Optimization credit |
| 7       | **D**         | **E**            | 7                 | Branch to E |
| 8       | **D**         | **J**            | 8                 | Alternate route to J |
| 9       | **E**         | **F**            | 10                | High-cost route |
| 10      | **E**         | **I**            | -2                | Optimization credit |
| 11      | **F**         | **G**            | 4                 | Alternate route to G |
| 12      | **F**         | **I**            | 2                 | Cycle edge with I |
| 13      | **G**         | **H**            | 13                | Terminal route to H |
| 14      | **I**         | **F**            | 2                 | Cycle edge with F |
| 15      | **J**         | **E**            | 3                 | Optimized path to E |

### 2.2 Cycle Analysis
- The only directed cycle in the network exists between **F** and **I**:
  - $F \rightarrow I$ with cost $+2$
  - $I \rightarrow F$ with cost $+2$
  - Cumulative cycle cost: $2 + 2 = +4 > 0$
- Because this cycle has a positive total weight, **no negative-weight cycles exist** in the network. The shortest paths are well-defined.

---

## 3. Theoretical Background & Algorithm Design

### 3.1 Why Dijkstra's Algorithm Fails with Negative Weights
Per **Module 5 (Pathfinding, Section 14)**:
* Dijkstra's algorithm relies on a greedy strategy that assumes once an unvisited vertex with the minimum tentative distance is extracted from the priority queue, its distance is finalized and cannot decrease further.
* When negative edges exist (such as $C \rightarrow G$ with weight $-9$ and $E \rightarrow I$ with weight $-2$), later transitions can reduce costs to previously finalized nodes, causing Dijkstra's algorithm to yield suboptimal paths.

### 3.2 The Bellman–Ford Algorithm
Bellman–Ford uses dynamic programming and edge relaxation:
1. **Initialization:**
   $$\text{dist}[A] = 0, \quad \text{dist}[v] = \infty \quad (\forall v \neq A), \quad \text{parent}[v] = -1 \quad (\forall v)$$
2. **Relaxation ($|V| - 1$ passes):**
   In a graph with $|V|$ vertices, any simple path has at most $|V| - 1$ edges. Every edge $(u, v)$ with weight $w$ is relaxed across $|V| - 1$ rounds:
   $$\text{if } \text{dist}[u] \neq \infty \text{ and } \text{dist}[u] + w < \text{dist}[v] \implies \text{dist}[v] = \text{dist}[u] + w, \quad \text{parent}[v] = u$$
3. **Negative-Cycle Detection ($|V|$-th pass):**
   A final pass examines all edges. If any edge can still be relaxed:
   $$\text{dist}[u] + w < \text{dist}[v]$$
   Then a reachable negative-weight cycle exists, and the algorithm reports:
   > `"Negative-weight cycle detected. Shortest-path results may be undefined."`
   Otherwise, it reports:
   > `"No negative-weight cycle detected."`

### 3.3 Path Reconstruction
By storing the predecessor node in `parent[v]`, any shortest path from source $A$ to target $v$ can be reconstructed recursively by following parent pointers backward and reversing the sequence.

---

## 4. Source Code Architecture

| File | Description |
|:-----|:------------|
| `routing_analyzer.h` | Defines data structures (`Edge`, `Graph`, `RoutingResult`) and public API prototypes. |
| `routing_analyzer.c` | Implements graph creation, edge registration, Bellman-Ford algorithm, negative cycle detection, and routing table printing. |
| `main.c` | Program entry point; sets up the network, executes analysis, and tests destination queries and edge cases. |
| `Makefile` | Automates build, execution, and cleanup with strict flags (`-Wall -Wextra -Werror -pedantic -std=c99 -g`). |

---

## 5. Build and Execution Instructions

### 5.1 Compilation
Compile the project using `make`:
```bash
make
```

### 5.2 Running the Program
Run the compiled binary:
```bash
make run
```
or directly:
```bash
./routing_analyzer
```

### 5.3 Cleaning Build Artifacts
Remove compiled objects and binaries:
```bash
make clean
```

---

## 6. Expected Output

```text
Initializing Cloud Service Data Routing Network...
Successfully registered 10 Data Centers and 15 directed routes.

======================================================================
              CLOUD SERVICE DATA ROUTING ANALYZER                    
======================================================================
Status: No negative-weight cycle detected.
Source: A

Destination     Shortest Cost   Path                               
----------------------------------------------------------------------
B               6               A → B                              
D               12              A → B → D                          
C               12              A → B → C                          
J               13              A → B → J                          
G               3               A → B → C → G                      
E               16              A → B → J → E                      
F               16              A → B → J → E → I → F              
I               14              A → B → J → E → I                  
H               16              A → B → C → G → H                  
======================================================================

--- Specific Destination Query Example (Task 2) ---
Destination: G
Path: A → B → C → G
Cost: 3

Destination: H
Path: A → B → C → G → H
Cost: 16

Destination: F
Path: A → B → J → E → I → F
Cost: 16

--- Robustness & Edge-Case Validation ---
Testing query for invalid data center 'Z':
Destination: Z (Invalid data center)
Path: None
Cost: N/A
```
