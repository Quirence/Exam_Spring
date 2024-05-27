#include <iostream>
#include <vector>
#include <cstdint>

class AdjList {
 public:
  int n_vertex_{};
  int n_edges_{};
  std::vector<std::vector<int>> adj_list_;
  AdjList() {
    std::cin >> n_vertex_ >> n_edges_;
    adj_list_.resize(n_vertex_);
    for (int i = 0; i < n_edges_; ++i) {
      int a, b;
      std::cin >> a >> b;
      a--;
      b--;
      adj_list_[a].push_back(b);
      adj_list_[b].push_back(a);
    }
  }
};

class DFS {
 public:
  bool Cycle_ = false;

  void operator()(AdjList& graph, std::vector<int>& visited, int64_t start) {
    visited[start] = 1; // Помечаем вершину как посещенную (в процессе)
    for (int to : graph.adj_list_[start]) {
      if (visited[to] == 0) {
        // Если вершина не была посещена, продолжаем DFS
        operator()(graph, visited, to);
      } else if (visited[to] == 1) {
        // Если вершина посещена и не завершена, значит найден цикл
        Cycle_ = true;
      }
    }
    visited[start] = 2; // Помечаем вершину как полностью обработанную
  }
};

// Для поиска цикла в неориентированном графе можно добавить continue и передавать parent(from), чтобы DFS не нашел цикл из двух вершин.


class Connection {
 public:
  DFS DepthFirstSearch;
  bool operator()(AdjList& graph) {
    int nsize = graph.n_vertex_;
    std::vector<int> visited(nsize, 0); // Инициализаруем нулями!
    DepthFirstSearch(graph, visited, 0);
    for (int i : visited) {
      if (i == 0) {
        return false; // Нашли необработанную вершину
      }
    }
    return true; // Граф связный
  }
};

class Greed {
 public:
  int colors_ = 0;
  void operator()(AdjList& graph, std::vector<int>& result) {
    result.assign(graph.n_vertex_, -1);
    result[0] = 0;
    colors_ = 1;
    std::vector<bool> available(graph.n_vertex_, false);
    for (int start = 1; start < graph.n_vertex_; ++start) {
      for (int neighbor : graph.adj_list_[start]) {
        if (result[neighbor] != -1) {
          available[result[neighbor]] = true;
        }
      }
      int cr;
      for (cr = 0; cr < graph.n_vertex_; ++cr) {
        if (!available[cr]) {
          break;
        }
      }
      result[start] = cr;
      if (cr + 1 > colors_) {
        colors_ = cr + 1;
      }
      for (int neighbor : graph.adj_list_[start]) {
        if (result[neighbor] != -1) {
          available[result[neighbor]] = false;
        }
      }
    }
  }
};

// Эйлеров цикл - проверка на четность всех степеней вершин. Копируем чужой код:

/*
std::set<int> g_euler[maxn];

bool IsEulerian() {
  for (int i = 0; i < maxn; ++i) {
    if (!g_euler[i].empty() && (g_euler[i].size() % 2 != 0)) {
      return false; // Возвращаем false, если нашли вершину с нечетной степенью
    }
  }
  return true; // Все вершины с рёбрами имеют четные степени
}

void Euler(int v) {
  while (!g_euler[v].empty()) {
    auto u = *g_euler[v].begin();
    g_euler[v].erase(u);
    g_euler[u].erase(v); // если граф ориентированный, обратное ребро удалять не надо
    Euler(u);
  }
  std::cout << v <<  " ";
}

*/