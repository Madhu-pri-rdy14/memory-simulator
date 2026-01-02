# Memory Management Simulator - Design Document

## 1. Overview
This project simulates an Operating System's memory management unit (MMU) in user space. It models Physical Memory allocation, a Multilevel Cache hierarchy, and Virtual Memory translation using Paging. The goal is to demonstrate understanding of OS-level abstractions, fragmentation, and memory access latency.

## 2. Memory Layout and Assumptions
* **Physical Memory:** Modeled as a contiguous array of bytes (simulated by a `total_size` variable).
* **Block Structure:** Memory is managed using a `Block` linked list. Each block contains metadata:
    * `id`: Unique identifier for the allocation.
    * `size`: Size of the block in bytes.
    * `start_addr`: Physical starting address.
    * `is_free`: Boolean flag indicating status.
* **Addressing:** Physical addresses range from `0` to `total_size - 1`. Virtual addresses are mapped via a Page Table.

## 3. Allocation Strategies
We implemented three standard dynamic allocation algorithms using a singly linked list:
1.  **First Fit:** Scans the list from the beginning and selects the first free block that is large enough.
2.  **Best Fit:** Scans the entire list to find the smallest free block that fits the request (minimizing wasted space).
3.  **Worst Fit:** Scans the entire list to select the largest available free block (leaving large holes for future allocations).
4.  **Coalescing:** On `free()`, the allocator checks if adjacent blocks are free. If so, they are merged into a single larger block to reduce external fragmentation.

## 4. Buddy System Design (Advanced Extension)
* **Mechanism:** Memory is treated as a single block of size $2^N$.
* **Splitting:** When a request arrives, the block is recursively split into two equal "buddies" until the smallest block capable of fitting the data is found.
* **Merging:** When a block is freed, its "buddy" is checked. If the buddy is also free, they merge back up the tree.
* **Implementation:** We use an array of free lists (`std::vector<std::vector<Block*>>`), where index `i` stores free blocks of size $2^i$.

## 5. Multilevel Cache Hierarchy
The system simulates a two-level inclusive cache hierarchy:
* **L1 Cache:** Small, fast, Direct Mapped.
* **L2 Cache:** Larger, slower, using **LRU (Least Recently Used)** replacement policy.
* **Flow:**
    1.  CPU requests Physical Address $P$.
    2.  Check L1. If Hit $\to$ Return.
    3.  If Miss $\to$ Check L2.
    4.  If L2 Hit $\to$ Load into L1.
    5.  If L2 Miss $\to$ Fetch from Main Memory $\to$ Load into L2 $\to$ Load into L1.

## 6. Virtual Memory Model
* **Paging:** We simulate a paging system with a fixed Page Size (e.g., 64 bytes).
* **Page Table:** Implemented as a `std::map<int, PageTableEntry>`, mapping a Virtual Page Number (VPN) to a Physical Frame Number (PFN).
* **Address Translation Flow:**
    $$\text{Virtual Address} \xrightarrow{\text{div PageSize}} \text{VPN} \xrightarrow{\text{Lookup}} \text{PFN} \to \text{Physical Address}$$
* **Page Faults:** If a VPN is not found in the map, the system raises a "PAGE FAULT" error (symbolic representation of a disk fetch requirement).

## 7. Limitations and Simplifications
* **Disk Latency:** We simulate disk fetches for page faults symbolically; no actual disk I/O is performed.
* **Single Process:** The simulation currently assumes a single active process context (one page table).
* **Memory Data:** We simulate the *allocation* of memory, but we do not store actual user data inside the allocated blocks (only metadata is tracked).
* **Concurrency:** The simulator is single-threaded and does not handle race conditions.