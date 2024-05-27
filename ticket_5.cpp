#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

class AdjList {
 public:
  int n_vertex_{};
  int n_edges_{};
  std::vector<std::vector<int>> adj_list_;
  std::vector<std::vector<int>> rev_adj_list_;
  AdjList() {
    std::cin >> n_vertex_ >> n_edges_;
    adj_list_.resize(n_vertex_);
    rev_adj_list_.resize(n_vertex_);
    for (int i = 0; i < n_edges_; ++i) {
      int a, b;
      std::cin >> a >> b;
      a--;
      b--;
      adj_list_[a].push_back(b);
      rev_adj_list_[b].push_back(a);
      // Убираем строчку, которая добавляет ребро в обратном направлении,
      // чтобы граф стал направленным
      // adj_list_[b].push_back(a);
      // adj_list_[a].push_back(b);
    }
  }
};

class DFS {
 public:
  void operator()(AdjList& graph, std::vector<int>& visited, int64_t start, std::vector<int64_t>& order) {
    visited[start] = 1;
    for (int to : graph.adj_list_[start]) {
      if (visited[to] == 0) {
        operator()(graph, visited, to, order);
      }
    }
    order.push_back(start);
  }
};

// Функтор для топологической сортировки, который принимает только ациклический граф (DAG)
class TopologicalSort {
 public:
  std::vector<int64_t> operator()(AdjList& graph) {
    std::vector<int> visited(graph.n_vertex_, 0);
    std::vector<int64_t> order;

    DFS dfs;
    for (int i = 0; i < graph.n_vertex_; ++i) {
      if (visited[i] == 0) {
        dfs(graph, visited, i, order);
      }
    }

    std::reverse(order.begin(), order.end());
    return order;
  }
};

// Алгоритм Косараджу

class Graph {
  std::vector<std::vector<int>> g, gr;
  std::vector<int> visited, order;
  void dfs1(int vertex) {
    visited[vertex] = 1;
    for (int i : gr[vertex]) {
      if (!visited[i]) {
        dfs1(i);
      }
    }
    order.push_back(vertex);
  }

  void dfs2(int vertex, int component) {
    visited[vertex] = component;
    for (int i : g[vertex]) {
      if (!visited[i]) {
        dfs2(i, component);
      }
    }
  }
 public:
  Graph(int nsize) {
    g.resize(nsize);
    gr.resize(nsize);
  }

  void AddEdge(int from, int to) {
    g[from].push_back(to);
    gr[to].push_back(from);
  }

  std::vector<int> FindScc() {
    visited.assign(g.size(), 0);
    for (int v = 0; v < g.size(); ++v) {
      if (!visited[v]) {
        dfs1(v);
      }
    }
    std::reverse(order.begin(), order.end());
    visited.assign(g.size(), 0);
    int counter = 0;
    for (int v : order) {
      if (!visited[v]) {
        dfs2(v, ++counter);
      }
    }
    return visited;
  }
};