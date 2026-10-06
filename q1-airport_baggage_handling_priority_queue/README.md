# Question 1: Airport Baggage Handling Priority Queue

## 1. Overview
At an international airport, an automated baggage handling system manages baggage containers dynamically. Containers with higher priority scores must be processed before containers with lower scores. This system is implemented in C using an **array-based Binary Max-Heap**.

---

## 2. Heap Data Structure & Mathematical Properties
- **Complete Binary Tree Representation:** Stored contiguously in a dynamically-sized array.
- **Index Formulas (0-based):**
  - $\text{Parent}(i) = \lfloor (i - 1) / 2 \rfloor$
  - $\text{Left Child}(i) = 2i + 1$
  - $\text{Right Child}(i) = 2i + 2$
- **Max-Heap Property:** $\forall i > 0, \quad \text{Priority}(\text{Parent}(i)) \ge \text{Priority}(i)$.
- **Container Record:**
  - `id`: Unique identifier (e.g., `'A'`, `'B'`, ..., `'X'`)
  - `name`: Descriptive flight/cargo label
  - `priority`: Integer priority score

---

## 3. Operations & Step-by-Step Traces

### Initial Input
$P = \{56, 23, 91, 34, 72, 48, 85, 17, 63, 79, 42\}$ assigned to IDs $A$ through $K$.

### Task 1: Max-Heap Construction ($O(n)$ Bottom-Up Heapify)
- Starts from the last internal node: $\lfloor (11 - 1) / 2 \rfloor = 4$ down to $0$.
- Sift-down operations ensure child-parent ordering is preserved while maintaining ID-name-priority associations during every swap.
- **Resulting Heap Array:**
  ```text
  Index:     [0]   [1]   [2]   [3]   [4]   [5]   [6]   [7]   [8]   [9]   [10]
  Container: C:91  J:79  G:85  I:63  E:72  F:48  A:56  H:17  D:34  B:23  K:42
  ```

### Task 2: Urgent Baggage Container Arrival ($O(\log n)$ Sift-Up)
- Container `X` with Priority `100` is appended at index `11`.
- Sifts up through parent nodes:
  1. Compares with index 5 (`F:48`): $100 > 48 \implies$ Swap.
  2. Compares with index 2 (`G:85`): $100 > 85 \implies$ Swap.
  3. Compares with index 0 (`C:91`): $100 > 91 \implies$ Swap.
- `X:100` becomes the new root.
- **Resulting Heap Array:**
  ```text
  Index:     [0]    [1]   [2]   [3]   [4]   [5]   [6]   [7]   [8]   [9]   [10]  [11]
  Container: X:100  J:79  C:91  I:63  E:72  G:85  A:56  H:17  D:34  B:23  K:42  F:48
  ```

### Task 3: Cancelled Container Removal ($O(\log n)$ Sift-Down)
- Container `X:100` is removed from the root.
- The last container `F:48` (index 11) moves to the root, reducing heap size to 11.
- Sifts down from root:
  1. Children are index 1 (`J:79`) and index 2 (`C:91`). Larger is `C:91` ($91 > 48$) $\implies$ Swap with index 2.
  2. Children of index 2 are index 5 (`G:85`) and index 6 (`A:56`). Larger is `G:85` ($85 > 48$) $\implies$ Swap with index 5.
  3. Index 5 has no children within bounds. Sift-down terminates.
- **Resulting Heap Array:**
  ```text
  Index:     [0]   [1]   [2]   [3]   [4]   [5]   [6]   [7]   [8]   [9]   [10]
  Container: C:91  J:79  G:85  I:63  E:72  F:48  A:56  H:17  D:34  B:23  K:42
  ```

### Rubric Requirement: Extraction in Descending Priority Order
- Demonstrates repeated extraction of the maximum element until empty:
  `C (91) -> G (85) -> J (79) -> E (72) -> I (63) -> A (56) -> F (48) -> K (42) -> D (34) -> B (23) -> H (17)`

---

## 4. Compilation and Execution
To compile and run using the provided Makefile:

```bash
make
./baggage_priority_queue
```

Or run directly:
```bash
make run
```
