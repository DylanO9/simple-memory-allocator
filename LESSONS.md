# Mini Malloc

## Overview
This repository contains a custom implementation of **`malloc`** and **`free`**, designed to simulate how dynamic memory allocation works at the system level.  
My implementation uses a **free list** built on a **linked list** structure to manage memory blocks on the heap.

## What I Learned
Building `mini_malloc` taught me how **heap memory** is structured and managed. I implemented:
- A **free list** that tracks allocated and freed memory blocks.
- A **header-based layout**, where each block includes metadata for size and linkage.
- A **`free()`** routine that reinserts released blocks into the free list for reuse.
- **Memory alignment**, ensuring data within allocated blocks meets platform alignment requirements by using the most restrictive data type.

## Takeaways
This project helped me:
- Understand **low-level memory management** and **heap internals**.  
- Practice using **linked lists** in a practical, system-level context.  
- Appreciate the tradeoffs between **simplicity, fragmentation, and performance** in real allocators.  

Overall, `mini_malloc` gave me hands-on insight into how the C runtime manages dynamic memory under the hood.

