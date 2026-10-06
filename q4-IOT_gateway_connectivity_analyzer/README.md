# Question 4: IoT Gateway Connectivity Analyzer

## 1. Overview
A smart agriculture company operates several IoT gateways that collect environmental and crop data from sensors deployed across multiple farm zones. Gateways communicate directly with neighboring gateways over wireless and wired communication links to exchange sensor telemetry.

This project implements an **IoT Gateway Connectivity Analyzer** in C. The network is modeled as an **undirected, weighted graph** where:
- **Vertices (Nodes):** Represent individual IoT gateways (`A` through `G`).
- **Edges (Links):** Represent direct bidirectional communication links.
- **Edge Weights:** Represent average data-transfer times (latencies) in milliseconds (ms).

The analyzer accepts a user-selected starting gateway, traverses the network using **Breadth-First Search (BFS)** with a queue, identifies all **1-hop directly connected gateways**, and determines the direct link with the **highest data-transfer time** (the slowest bottleneck link).

---

## 2. Graph Model & Network Topology

### 2.1 Graph Definition
- **Vertices:** $V = \{A, B, C, D, E, F, G\}$, $|V| = 7$
- **Edges:** $|E| = 10$ undirected, weighted communication links:

| Gateway 1 | Gateway 2 | Data-Transfer Time (ms) |
|:---------:|:---------:|:-----------------------:|
| **A**     | **B**     | 6                       |
| **A**     | **D**     | 12                      |
| **B**     | **D**     | 5                       |
| **B**     | **C**     | 11                      |
| **C**     | **D**     | 17                      |
| **C**     | **G**     | 25                      |
| **D**     | **E**     | 22                      |
| **D**     | **F**     | 15                      |
| **E**     | **F**     | 10                      |
| **F**     | **G**     | 22                      |

### 2.2 Adjacency Matrix Representation
Because $|V| = 7$ is small and fixed, an **Adjacency Matrix** of dimensions $7 \times 7$ provides $O(1)$ edge-weight lookups while requiring minimal memory. An entry of `0` denotes the absence of a direct link. Since the network is undirected, the matrix is symmetric ($\text{adj}[u][v] = \text{adj}[v][u]$):

```text
       A    B    C    D    E    F    G
  A [  0,   6,   0,  12,   0,   0,   0 ]
  B [  6,   0,  11,   5,   0,   0,   0 ]
  C [  0,  11,   0,  17,   0,   0,  25 ]
  D [ 12,   5,  17,   0,  22,  15,   0 ]
  E [  0,   0,   0,  22,   0,  10,   0 ]
  F [  0,   0,   0,  15,  10,   0,  22 ]
  G [  0,   0,  25,   0,   0,  22,   0 ]
```

---

## 3. Algorithm & Implementation Details

### 3.1 FIFO Queue Implementation
Per Module 3 (Graph Data Structures, Unit 6), BFS requires a First-In, First-Out (FIFO) queue. A circular queue structure is implemented with the following operations:
- `queue_init(Queue *q)`: Initializes `front`, `rear`, and `count` to `0`.
- `queue_enqueue(Queue *q, int val)`: Adds a gateway index to the rear.
- `queue_dequeue(Queue *q)`: Removes and returns the gateway index from the front.
- `queue_is_empty(const Queue *q)`: Checks if unvisited discovered nodes remain.

### 3.2 BFS Traversal & Invariants
1. **Discovery & Visited Marking:** The starting gateway is marked `visited = true` upon enqueuing to prevent duplicate processing or infinite loops in cyclic topologies.
2. **Level-0 Processing:** When the starting gateway is dequeued from the queue, all its adjacent, unvisited neighbors are discovered and enqueued.
3. **1-Hop Neighbor Identification:** Every neighbor discovered directly from the starting gateway belongs to **Level 1** (1-hop away). These neighbors are recorded along with their edge weights.
4. **Traversal Continuation:** BFS continues dequeueing and visiting remaining nodes level-by-level until the queue is exhausted.

### 3.3 Communication Analysis
The 1-hop neighbors are scanned to evaluate their link latencies:
$$\text{Max Transfer Time} = \max_{v \in \text{Neighbors}(s)} \text{adj}[s][v]$$
The gateway corresponding to this maximum latency is flagged as the slowest direct link.

---

## 4. Step-by-Step Traces

### Trace 1: Starting Gateway `D` (Prompt Example)
- **Direct 1-Hop Neighbors of D:**
  - $D \leftrightarrow A$: $12\text{ ms}$
  - $D \leftrightarrow B$: $5\text{ ms}$
  - $D \leftrightarrow C$: $17\text{ ms}$
  - $D \leftrightarrow E$: $22\text{ ms}$
  - $D \leftrightarrow F$: $15\text{ ms}$
- **Queue Trace:**
  1. Initial: Enqueue `D` $\rightarrow$ Queue: `[D]`, Visited: `{D}`
  2. Dequeue `D`: Discover direct neighbors `A, B, C, E, F` $\rightarrow$ Queue: `[A, B, C, E, F]`, Visited: `{D, A, B, C, E, F}`
  3. Dequeue `A`: No new unvisited neighbors.
  4. Dequeue `B`: No new unvisited neighbors.
  5. Dequeue `C`: Discovers `G` (weight $25$) $\rightarrow$ Queue: `[E, F, G]`, Visited: `{D, A, B, C, E, F, G}`
  6. Dequeue `E`: No new unvisited neighbors.
  7. Dequeue `F`: `G` already visited.
  8. Dequeue `G`: No new unvisited neighbors. Queue empty.
- **BFS Traversal Order:** `D -> A -> B -> C -> E -> F -> G`
- **Highest Transfer Time:** **Gateway E** ($22\text{ ms}$)

---

### Trace 2: Starting Gateway `A`
- **Direct 1-Hop Neighbors of A:**
  - $A \leftrightarrow B$: $6\text{ ms}$
  - $A \leftrightarrow D$: $12\text{ ms}$
- **BFS Traversal Order:** `A -> B -> D -> C -> E -> F -> G`
- **Highest Transfer Time:** **Gateway D** ($12\text{ ms}$)

---

### Trace 3: Starting Gateway `G`
- **Direct 1-Hop Neighbors of G:**
  - $G \leftrightarrow C$: $25\text{ ms}$
  - $G \leftrightarrow F$: $22\text{ ms}$
- **BFS Traversal Order:** `G -> C -> F -> B -> D -> E -> A`
- **Highest Transfer Time:** **Gateway C** ($25\text{ ms}$)

---

## 5. Compilation and Execution

### Build
Compile the program using `make` (enforcing strict C99 standards: `-Wall -Wextra -Werror -pedantic -std=c99 -g`):
```bash
make
```

### Run
Execute the compiled binary:
```bash
make run
# or directly:
./gateway_analyzer
```

### Sample Interactive Session
```text
========================================================
      SMART AGRICULTURE: IoT GATEWAY ANALYZER           
========================================================
--------------------------------------------------------
Available Gateways in Network: A, B, C, D, E, F, G
--------------------------------------------------------

Enter starting gateway: D
Selected Gateway: D (Validated successfully)

========================================================
     IoT GATEWAY CONNECTIVITY ANALYSIS REPORT           
========================================================
Starting Gateway: D

--- Task 2: BFS Connectivity Analysis ---
BFS Traversal Order: D -> A -> B -> C -> E -> F -> G
Directly Connected Gateways (1-hop): A B C E F 

--- Task 3: Communication Analysis ---
+-----------------------+--------------------------+
| Communication Link    | Data-Transfer Time (ms)  |
+-----------------------+--------------------------+
| Gateway D <-> A       |               12 ms     |
| Gateway D <-> B       |                5 ms     |
| Gateway D <-> C       |               17 ms     |
| Gateway D <-> E       |               22 ms     |
| Gateway D <-> F       |               15 ms     |
+-----------------------+--------------------------+

Directly connected gateway with HIGHEST transfer time:
  >> Gateway E (Transfer Time: 22 ms)
========================================================
```

### Clean
Remove compiled object files and binaries:
```bash
make clean
```
