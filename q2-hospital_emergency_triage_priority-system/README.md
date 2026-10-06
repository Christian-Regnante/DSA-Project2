# Question 2: Hospital Emergency Triage Priority System

## 1. Overview
In a hospital emergency department, incoming patients are continuously triaged and assigned a medical priority score based on the severity of their condition. Patients with higher priority scores must be attended to before patients with lower scores. 

To manage this dynamic queue with optimal time and space efficiency, the system is implemented in C using an **array-based Binary Max-Heap**.

---

## 2. Heap Data Structure & Mathematical Properties
- **Complete Binary Tree Representation:** The tree levels are filled from left to right and stored contiguously in an array.
- **Index Formulas (0-based Indexing):**
  - $\text{Parent}(i) = \lfloor (i - 1) / 2 \rfloor$
  - $\text{Left Child}(i) = 2i + 1$
  - $\text{Right Child}(i) = 2i + 2$
- **Max-Heap Property:**
  $$\forall i > 0, \quad \text{Priority}(\text{Parent}(i)) \ge \text{Priority}(i)$$
  The root node at index `0` always holds the patient with the highest triage priority score.
- **Patient Record Structure:**
  - `id`: Unique patient identifier (`"P01"`, `"P02"`, ..., `"P08"`)
  - `name`: Full patient name (`"Amina"`, `"Daniel"`, etc.)
  - `score`: Integer triage urgency score

---

## 3. Step-by-Step Task Operations & Traces

### Initial Input Cohort ($n = 7$)
| Index | Patient ID | Patient Name | Priority Score |
| :---: | :---: | :---: | :---: |
| **0** | `P01` | Amina | 72 |
| **1** | `P02` | Daniel | 45 |
| **2** | `P03` | Eric | 91 |
| **3** | `P04` | Grace | 63 |
| **4** | `P05` | Hassan | 88 |
| **5** | `P06` | Irene | 54 |
| **6** | `P07` | Jean | 76 |

---

### Task 1: Build the Max-Heap ($O(n)$ Bottom-Up Heapify)
Construction begins at the last non-leaf node: $\lfloor 7 / 2 \rfloor - 1 = \mathbf{2}$ down to $\mathbf{0}$.

1. **At Index 2 (`Eric: 91`):**
   - Children: Index 5 (`Irene: 54`), Index 6 (`Jean: 76`).
   - $91 \ge 54$ and $91 \ge 76$. No swap needed.
2. **At Index 1 (`Daniel: 45`):**
   - Children: Index 3 (`Grace: 63`), Index 4 (`Hassan: 88`).
   - Highest child is `Hassan: 88` at index 4.
   - Swap Index 1 and Index 4: `[72, 88, 91, 63, 45, 54, 76]`.
3. **At Index 0 (`Amina: 72`):**
   - Children: Index 1 (`Hassan: 88`), Index 2 (`Eric: 91`).
   - Highest child is `Eric: 91` at index 2.
   - Swap Index 0 and Index 2: `[91, 88, 72, 63, 45, 54, 76]`.
   - Sift-down for Index 2 (`Amina: 72`):
     - Children: Index 5 (`Irene: 54`), Index 6 (`Jean: 76`).
     - Highest child is `Jean: 76` at index 6.
     - Swap Index 2 and Index 6: `[91, 88, 76, 63, 45, 54, 72]`.

#### Resulting Max-Heap State:
- **Array:**
  ```text
  Index:     [0]       [1]        [2]       [3]        [4]         [5]        [6]
  Patient:   P03       P05        P07       P04        P02         P06        P01
  Name:      Eric      Hassan     Jean      Grace      Daniel      Irene      Amina
  Score:     91        88         76        63         45          54         72
  ```
- **Binary Tree Visualization:**
  ```text
                     [0] Eric (91)
                    /             \
         [1] Hassan (88)        [2] Jean (76)
         /             \        /           \
   [3] Grace (63) [4] Daniel (45) [5] Irene (54) [6] Amina (72)
  ```

---

### Task 2: Generate the Treatment Order
By repeatedly extracting the root patient ($O(\log n)$ per extraction), patients are attended in strict descending priority order:

```text
Patient P03 (Eric) — Priority 91
Patient P05 (Hassan) — Priority 88
Patient P07 (Jean) — Priority 76
Patient P01 (Amina) — Priority 72
Patient P04 (Grace) — Priority 63
Patient P06 (Irene) — Priority 54
Patient P02 (Daniel) — Priority 45
```

---

### Task 3: New Emergency Patient (`P08: Kofi`, Score: 98)
1. Patient `P08` is inserted at the next array leaf position (Index 7).
2. **Sift-Up Operations:**
   - Parent of Index 7 is Index 3 (`Grace: 63`): $98 > 63 \implies$ Swap Index 7 & 3.
   - Parent of Index 3 is Index 1 (`Hassan: 88`): $98 > 88 \implies$ Swap Index 3 & 1.
   - Parent of Index 1 is Index 0 (`Eric: 91`): $98 > 91 \implies$ Swap Index 1 & 0.
   - Index 0 is the root node; termination condition met.

#### Resulting Heap After Insertion:
```text
Index:     [0]       [1]        [2]       [3]        [4]         [5]        [6]        [7]
Patient:   P08       P03        P07       P05        P02         P06        P01        P04
Name:      Kofi      Eric       Jean      Hassan     Daniel      Irene      Amina      Grace
Score:     98        91         76        88         45          54         72         63
```

---

### Task 4: Patient Cleared (Removing `P08`)
1. Remove `P08` from root and replace with last element (`P04: Grace, 63` at Index 7).
2. Decrement heap size from 8 to 7.
3. **Sift-Down Operations:**
   - At Index 0 (`Grace: 63`): Children are Index 1 (`Eric: 91`) and Index 2 (`Jean: 76`).
     Highest is `Eric: 91` $\implies$ Swap Index 0 & 1.
   - At Index 1 (`Grace: 63`): Children are Index 3 (`Hassan: 88`) and Index 4 (`Daniel: 45`).
     Highest is `Hassan: 88` $\implies$ Swap Index 1 & 3.
   - Index 3 has no valid children in range $\implies$ Sift-down terminates.

#### Resulting Heap After Clearing `P08`:
```text
Index:     [0]       [1]        [2]       [3]        [4]         [5]        [6]
Patient:   P03       P05        P07       P04        P02         P06        P01
Name:      Eric      Hassan     Jean      Grace      Daniel      Irene      Amina
Score:     91        88         76        63         45          54         72
```

---

## 4. Compilation and Execution

### Build Using Makefile:
```bash
make
```

### Run Program:
```bash
make run
```

### Clean Build Artifacts:
```bash
make clean
```

---

## 5. Sample Console Output

```text
********************************************************************************
     HOSPITAL EMERGENCY DEPARTMENT - TRIAGE PRIORITY SYSTEM (MAX-HEAP)
********************************************************************************

>>> [Input Received]: 7 waiting patients for assessment:
    [P01] Amina    => Priority Score: 72
    [P02] Daniel   => Priority Score: 45
    [P03] Eric     => Priority Score: 91
    [P04] Grace    => Priority Score: 63
    [P05] Hassan   => Priority Score: 88
    [P06] Irene    => Priority Score: 54
    [P07] Jean     => Priority Score: 76

>>> [TASK 1]: Building array-based Max-Heap from patient cohort...
    Executing bottom-up heap construction in O(n) time...
================================================================================
 TASK 1: Resulting Initial Max-Heap
 (Queue Size: 7, Max-Heap Invariant: SATISFIED [OK])
--------------------------------------------------------------------------------
 Index  | ID     | Patient Name     | Priority | Child Nodes         
--------------------------------------------------------------------------------
 [ 0]   | P03    | Eric             | 91       | L:P05(88), R:P07(76)
 [ 1]   | P05    | Hassan           | 88       | L:P04(63), R:P02(45)
 [ 2]   | P07    | Jean             | 76       | L:P06(54), R:P01(72)
 [ 3]   | P04    | Grace            | 63       | None (Leaf)
 [ 4]   | P02    | Daniel           | 45       | None (Leaf)
 [ 5]   | P06    | Irene            | 54       | None (Leaf)
 [ 6]   | P01    | Amina            | 72       | None (Leaf)
================================================================================

--- Binary Tree Representation (Level-by-Level) ---
 Level 0: [P03: Eric (91)] 
 Level 1: [P05: Hassan (88)] [P07: Jean (76)] 
 Level 2: [P04: Grace (63)] [P02: Daniel (45)] [P06: Irene (54)] [P01: Amina (72)] 
---------------------------------------------------

>>> [TASK 2]: Generating Treatment Order (Repeated Max-Extraction)...
    Extracting patients in descending order of triage priority score:

Patient P03 (Eric) — Priority 91
Patient P05 (Hassan) — Priority 88
Patient P07 (Jean) — Priority 76
Patient P01 (Amina) — Priority 72
Patient P04 (Grace) — Priority 63
Patient P06 (Irene) — Priority 54
Patient P02 (Daniel) — Priority 45

    (Queue Size: 0 — All initial patients scheduled in priority order)

>>> [TASK 3]: New Critical Emergency Patient Arrives!
    Patient ID: P08 | Name: Kofi | Priority Score: 98
    Inserting into the existing Max-Heap and restoring heap property via sift_up()...
================================================================================
 TASK 3: Max-Heap After Inserting Kofi (P08: 98)
 (Queue Size: 8, Max-Heap Invariant: SATISFIED [OK])
--------------------------------------------------------------------------------
 Index  | ID     | Patient Name     | Priority | Child Nodes         
--------------------------------------------------------------------------------
 [ 0]   | P08    | Kofi             | 98       | L:P03(91), R:P07(76)
 [ 1]   | P03    | Eric             | 91       | L:P05(88), R:P02(45)
 [ 2]   | P07    | Jean             | 76       | L:P06(54), R:P01(72)
 [ 3]   | P05    | Hassan           | 88       | L:P04(63)
 [ 4]   | P02    | Daniel           | 45       | None (Leaf)
 [ 5]   | P06    | Irene            | 54       | None (Leaf)
 [ 6]   | P01    | Amina            | 72       | None (Leaf)
 [ 7]   | P04    | Grace            | 63       | None (Leaf)
================================================================================

--- Binary Tree Representation (Level-by-Level) ---
 Level 0: [P08: Kofi (98)] 
 Level 1: [P03: Eric (91)] [P07: Jean (76)] 
 Level 2: [P05: Hassan (88)] [P02: Daniel (45)] [P06: Irene (54)] [P01: Amina (72)] 
 Level 3: [P04: Grace (63)] 
---------------------------------------------------

>>> [TASK 4]: Patient P08 has been treated and cleared from the emergency queue...
    Removing P08 and restoring Max-Heap property via sift_down()...
    Successfully cleared: [P08] Kofi (Priority: 98)

================================================================================
 TASK 4: Max-Heap After Clearing P08
 (Queue Size: 7, Max-Heap Invariant: SATISFIED [OK])
--------------------------------------------------------------------------------
 Index  | ID     | Patient Name     | Priority | Child Nodes         
--------------------------------------------------------------------------------
 [ 0]   | P03    | Eric             | 91       | L:P05(88), R:P07(76)
 [ 1]   | P05    | Hassan           | 88       | L:P04(63), R:P02(45)
 [ 2]   | P07    | Jean             | 76       | L:P06(54), R:P01(72)
 [ 3]   | P04    | Grace            | 63       | None (Leaf)
 [ 4]   | P02    | Daniel           | 45       | None (Leaf)
 [ 5]   | P06    | Irene            | 54       | None (Leaf)
 [ 6]   | P01    | Amina            | 72       | None (Leaf)
================================================================================

--- Binary Tree Representation (Level-by-Level) ---
 Level 0: [P03: Eric (91)] 
 Level 1: [P05: Hassan (88)] [P07: Jean (76)] 
 Level 2: [P04: Grace (63)] [P02: Daniel (45)] [P06: Irene (54)] [P01: Amina (72)] 
---------------------------------------------------
```
