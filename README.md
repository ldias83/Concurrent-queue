# RingQueue

**RingQueue** is a minimal C++ example of a single-producer, single-consumer (SPSC) lock-free ring buffer.  
It demonstrates how to build a simple, cache-friendly, multi-threaded queue using `std::atomic` operations and memory ordering semantics.

---

## Features

- Lock-free single-producer single-consumer design
- Fixed-size circular buffer (`std::array`)
- Modern C++ atomics with explicit memory ordering

---

## How it works

The `RingQueue` uses two atomic counters (`m_head` and `m_tail`) to track where to push and pop elements. 
A producer thread pushes integers from 1 to 20 while a consumer thread pops them and prints them to `stdout`.

---

## Build & Run

This example requires a C++17 (or newer) compiler.

```bash
g++ -std=c++17 -O2 -pthread -o ringqueue main.cpp
./ringqueue
