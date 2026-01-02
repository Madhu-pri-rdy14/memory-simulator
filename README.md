1.OVERVIEW

This project is a comprehensive simulation of an Operating System’s Memory Management Unit (MMU).
Unlike simple memory allocators, the simulator models the complete interaction between the CPU, Virtual Memory (Page Tables), Multilevel Cache Hierarchy, and Physical RAM.

The objective of this project is to demonstrate how modern operating systems manage memory using well-defined algorithms and abstractions.

2.FEATURES

Physical Memory Allocation
The simulator implements dynamic memory allocation strategies including First Fit, Best Fit, and Worst Fit.
Adjacent free memory blocks are automatically merged during deallocation to reduce external fragmentation.
Memory utilization and external fragmentation percentages are computed and reported.

Multilevel Cache Simulation
An L1 cache is simulated as a small, low-latency, direct-mapped cache.
An L2 cache is simulated as a larger, higher-latency, set-associative cache using the Least Recently Used (LRU) replacement policy.
Cache hits and misses are tracked and displayed.

Virtual Memory and Paging

The simulator models a page table that maps Virtual Page Numbers to Physical Frame Numbers.
Page faults are handled when unmapped virtual addresses are accessed.
The complete address translation flow is simulated as:
Virtual Address → MMU → Physical Address → Cache → RAM

3.PROJECT STRUCTURE

memory-simulator
├── docs 
├── include 
├── src
│ ├── allocator
│ ├── buddy 
│ ├── cache
│ ├── virtual_memory 
│ └── main.cpp
├── tests/workloads 
├── screenshots 
├── Makefile
How to Build and Run

Prerequisites

A C++ compiler compatible with the C++17 standard is required.

Build
Run the following command in the project root directory:
make
Run
Execute the simulator using:
./memsim

5.Sample Execution

After running the simulator, enter the following commands in sequence to observe allocator behavior, cache accesses, and statistics:

strategy best
init 1024
malloc 100
malloc 200
access 100
access 100
stats

These commands demonstrate memory allocation, cache hits on repeated accesses, and the final memory and cache performance statistics.
