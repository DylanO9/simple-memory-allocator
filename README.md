# Simple Memory Allocator (my_malloc)

A custom implementation of malloc, free, and realloc using sbrk() and a free-list.

## Memory Layout
```
+-----------+-------------+
| Header    | User Memory |
+-----------+-------------+
| size, ptr |   n bytes   |
+-----------+-------------+
```

Blocks are coalesced on free to prevent fragmentation.

## Key Features
- First-fit allocation strategy
- Block coalescing in free()
- sbrk() for heap expansion
- Spatial locality optimization

