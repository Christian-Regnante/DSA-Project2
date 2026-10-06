# Question 3: EV Charging Station Power Network

## 1. Overview
A city is installing a network of electric vehicle (EV) charging stations. The stations must be connected to the city's main power distribution grid so that every station can receive electricity either directly or through another station. 

This project designs and implements the **EV Charging Station Power Network Optimizer** in C. The network is modeled as an **undirected, weighted graph** where:
- **Vertices (Nodes):** Represent individual EV charging stations (`A` through `G`).
- **Edges (Links):** Represent candidate underground power-cable connections.
- **Edge Weights:** Represent cable installation costs in thousands of dollars ($1,000s USD).

The objective is to find a **Minimum Spanning Tree (MST)** using **Kruskal's Algorithm** to connect all charging stations into a unified electrical grid with the **minimum possible total cable installation cost**, ensuring full connectivity without redundant or expensive cyclic connections.

---

## 2. Graph Model & Network Topology

### 2.1 Graph Definition
- **Vertices:** $V = \{A, B, C, D, E, F, G\}$, $|V| = 7$ stations.
- **Edges:** $|E| = 10$ available underground cable connections:

| Connection | Station 1 | Station 2 | Installation Cost ($1,000s) |
|:----------:|:---------:|:---------:|:----------------------------:|
| **A — B**  | A         | B         | 6                            |
| **A — D**  | A         | D         | 12                           |
| **B — D**  | B         | D         | 5                            |
| **B — C**  | B         | C         | 11                           |
| **C — D**  | C         | D         | 17                           |
| **C — G**  | C         | G         | 25                           |
| **D — E**  | D         | E         | 22                           |
| **D — F**  | D         | F         | 15                           |
| **E — F**  | E         | F         | 10                           |
| **F — G**  | F         | G         | 22                           |

### 2.2 Task 1: Adjacency Matrix Representation
Because $|V| = 7$ is small and fixed, an **Adjacency Matrix** of dimensions $7 \times 7$ provides $O(1)$ edge-weight lookups while requiring minimal memory. An entry of `0` denotes the absence of a direct connection (and self-loops). Since the network is undirected, the matrix is symmetric ($\text{adj}[u][v] = \text{adj}[v][u]$):

```text
         [A]   [B]   [C]   [D]   [E]   [F]   [G]
   +----------------------------------------------
[A] |     0     6     0    12     0     0     0 
[B] |     6     0    11     5     0     0     0 
[C] |     0    11     0    17     0     0    25 
[D] |    12     5    17     0    22    15     0 
[E] |     0     0     0    22     0    10     0 
[F] |     0     0     0    15    10     0    22 
[G] |     0     0    25     0     0    22     0 
   +----------------------------------------------
```

---

## 3. Algorithm & Theoretical Foundation (Module 3)

### 3.1 Minimum Spanning Tree Properties
Per Module 3 (Graph Data Structures, Unit 7):
1. **Spanning Tree Definition:** A subgraph that contains all $|V|$ vertices, is connected, and contains no cycles.
2. **Edge Count:** Any valid tree connecting $|V|$ vertices contains exactly **$|V| - 1$ edges** ($7 - 1 = 6$ connections).
3. **Weight Minimization:** The MST minimizes the sum of selected edge weights:
   $$\text{Cost}(\text{MST}) = \sum_{e \in \text{MST}} w(e)$$

### 3.2 Kruskal's Algorithm
Kruskal's algorithm operates on a **global greedy strategy**:
1. **Global Sort:** Sort all candidate edges by increasing weight.
2. **Iterative Evaluation:** Consider each edge in ascending order.
3. **Cycle Avoidance:** Add an edge if and only if its endpoints belong to separate connected components. Otherwise, reject it.
4. **Termination:** Stop when exactly $|V| - 1 = 6$ edges have been selected.

### 3.3 Disjoint Set Union (DSU / Union-Find)
Per Module 3 (Unit 7, Section 27), cycles are detected efficiently using a **Disjoint Set Union (DSU)** structure:
- `dsu_find(i)`: Identifies the root representative of the set containing element `i` using **path compression** ($O(\alpha(V))$).
- `dsu_union(i, j)`: Unifies the components containing `i` and `j` using **union-by-rank** ($O(\alpha(V))$).
- **Cycle Condition:** If $\text{find}(u) == \text{find}(v)$, vertices $u$ and $v$ already share an electrical path; selecting edge $(u, v)$ would introduce a cycle.

---

## 4. Task 2: Step-by-Step Kruskal's Algorithm Trace

### 4.1 Sorted Connections (Ascending Cost)
| Priority / Order | Candidate Edge | Installation Cost ($1,000s) |
|:----------------:|:--------------:|:----------------------------:|
| 1                | **B — D**      | 5                            |
| 2                | **A — B**      | 6                            |
| 3                | **E — F**      | 10                           |
| 4                | **B — C**      | 11                           |
| 5                | **A — D**      | 12                           |
| 6                | **D — F**      | 15                           |
| 7                | **C — D**      | 17                           |
| 8                | **D — E**      | 22                           |
| 9                | **F — G**      | 22                           |
| 10               | **C — G**      | 25                           |

### 4.2 Disjoint Sets Evolution & Decision Log
- **Initial Disjoint Sets:**  
  `{ {A}, {B}, {C}, {D}, {E}, {F}, {G} }`

| Step | Candidate Edge | Cost | Component Status | Decision | Resulting Component Sets |
|:----:|:--------------:|:----:|:-----------------|:--------:|:-------------------------|
| 1 | **B — D** | 5 | $\text{find}(B) \ne \text{find}(D)$ | **ACCEPTED (1/6)** | `{ {B, D}, {A}, {C}, {E}, {F}, {G} }` |
| 2 | **A — B** | 6 | $\text{find}(A) \ne \text{find}(B)$ | **ACCEPTED (2/6)** | `{ {A, B, D}, {C}, {E}, {F}, {G} }` |
| 3 | **E — F** | 10 | $\text{find}(E) \ne \text{find}(F)$ | **ACCEPTED (3/6)** | `{ {A, B, D}, {E, F}, {C}, {G} }` |
| 4 | **B — C** | 11 | $\text{find}(B) \ne \text{find}(C)$ | **ACCEPTED (4/6)** | `{ {A, B, C, D}, {E, F}, {G} }` |
| 5 | **A — D** | 12 | $\text{find}(A) == \text{find}(D)$ | **REJECTED (Cycle)** | Cycle: `A — B — D — A`. Sets unchanged. |
| 6 | **D — F** | 15 | $\text{find}(D) \ne \text{find}(F)$ | **ACCEPTED (5/6)** | `{ {A, B, C, D, E, F}, {G} }` |
| 7 | **C — D** | 17 | $\text{find}(C) == \text{find}(D)$ | **REJECTED (Cycle)** | Cycle: `C — B — D — C`. Sets unchanged. |
| 8 | **D — E** | 22 | $\text{find}(D) == \text{find}(E)$ | **REJECTED (Cycle)** | Cycle: `D — F — E — D`. Sets unchanged. |
| 9 | **F — G** | 22 | $\text{find}(F) \ne \text{find}(G)$ | **ACCEPTED (6/6)** | `{ {A, B, C, D, E, F, G} }` |

*Target reached:* Exactly $V - 1 = 6$ connections selected. Edge `C — G : 25` is bypassed. All stations form a single connected component.

---

## 5. Task 3: Identified Selected Connections

The 6 underground cable connections selected for the optimal power grid:
- **Station B — Station D : 5**
- **Station A — Station B : 6**
- **Station E — Station F : 10**
- **Station B — Station C : 11**
- **Station D — Station F : 15**
- **Station F — Station G : 22**

### Specification Checklist:
- [x] **Connects all charging stations:** All 7 stations $\{A, B, C, D, E, F, G\}$ are in a single component.
- [x] **Contains exactly $V - 1$ connections:** $7 - 1 = 6$ edges.
- [x] **Contains no cycles:** Verified by DSU disjoint-set invariants.
- [x] **Minimum possible total installation cost:** Guaranteed by Kruskal greedy cut optimality.

---

## 6. Task 4: Total Installation Cost Calculation

$$\text{Total Cost} = 5 + 6 + 10 + 11 + 15 + 22 = \mathbf{69}$$

```text
Selected Connections:

Station B — Station D : 5
Station A — Station B : 6
Station E — Station F : 10
Station B — Station C : 11
Station D — Station F : 15
Station F — Station G : 22

Total Installation Cost: 69 thousand dollars
```
*(Equivalent to **$69,000 USD**).*

---

## 7. Project Structure & Build Instructions

### 7.1 Files
- `ev_network.h`: Data structures (`EVGraph`, `Edge`, `DSU`, `MSTResult`) and function prototypes.
- `ev_network.c`: Matrix construction, DSU operations, Kruskal's algorithm, and report generators.
- `main.c`: Driver program presenting all tasks systematically.
- `Makefile`: Build configuration using strict compiler flags (`-Wall -Wextra -Werror -pedantic -std=c99 -g`).
- `README.md`: Formal assignment documentation and theoretical analysis.

### 7.2 Compilation & Execution
To compile and link the application:
```bash
make
```

To run the program:
```bash
make run
```

To clean build artifacts:
```bash
make clean
```
