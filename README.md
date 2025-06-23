# 🚀 LRU-Splay Cache Framework

**LRU-Splay Cache Framework** is a C++ simulation tool for evaluating and comparing two cache strategies — **Least Recently Used (LRU)** and **Splay Tree-based** caches — in the context of shortest path queries over randomly generated graphs. It uses **Dijkstra's algorithm** for pathfinding and simulates real-world query patterns using a **Zipfian distribution**.

---

## 🔧 Features

- ✅ LRU Cache using STL (`list`, `unordered_map`)
- ✅ Splay Tree Cache with custom splay and eviction logic
- ✅ Dijkstra’s algorithm for shortest path computation
- ✅ Zipfian-distributed queries to mimic realistic access patterns
- ✅ Comparative analysis of cache hit rates

---

## 📂 File Overview

- `cache_framework.cpp`: Main simulation file implementing both cache systems, graph generation, Dijkstra's algorithm, and query handling.

---

## 🛠️ How It Works

1. **Graph Generation**: A random undirected graph with 1000 nodes and 5000 weighted edges.
2. **Query Generation**: 100,000 queries are generated using a Zipfian distribution to sample frequently accessed node pairs.
3. **Cache Comparison**:
   - Each query checks both the LRU and Splay Tree caches.
   - On **cache miss**, Dijkstra computes the path and caches the result.
   - On **cache hit**, the stored path is used directly.
4. **Output**: Hit rates for both caches are printed.

---

## 🖥️ Getting Started

### 🔧 Requirements

- C++11 or later
- A standard C++ compiler (e.g., `g++`, `clang++`)

### 🧪 Compilation

```bash
g++ -std=c++11 -O2 cache_framework.cpp -o cache_sim
