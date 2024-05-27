#include <iostream>
#include <vector>
#include <queue>
#include <climits>

#define INF INT_MAX

std::vector<int> BFS(std::vector<std::vector<int>> &graph, int start) {
  std::vector<int> dist(graph.size(), INF);
  std::queue<int> queue;

  dist[start] = 0;
  queue.push(start);

  while (!queue.empty()) {
    int vertex = queue.front();
    queue.pop();
    for (int to : graph[vertex]) {
      if (dist[to] == INF) { // Только если вершина не посещена
        dist[to] = dist[vertex] + 1;
        queue.push(to);
      }
    }
  }
  return dist;
}

int main() {
  int nsize = 0;
  int msize = 0;
  std::cin >> nsize >> msize;

  std::vector<std::vector<int>> graph(nsize);
  for (int i = 0; i < msize; ++i) {
    int from = 0;
    int to = 0;
    std::cin >> from >> to;
    from--;
    to--;

    graph[from].push_back(to);
    graph[to].push_back(from); // Удалить для ориентированного графа
  }
  int start = 0;
  std::cin >> start;

  std::vector<int> dist = BFS(graph, start);
  for (int distance : dist) {
    if (distance != INF) {
      std::cout << distance << " ";
    } else {
      std::cout << "X ";
    }
  }
  std::cout << std::endl;
}

// kBFS - для вычисления кратчайшего расстония для взвешенного графа
const int n = 100; // количество вершин
const int k = 50; // максимальный вес ребра
struct Edge {
  int vertex;
  int to;
  int weight;
  Edge(): vertex(0), to(0), weight(0) {
  }
  Edge(int vertex, int to, int weight): vertex(vertex), to(to), weight(weight) {
  }
};

std::vector<int> kBFS(const std::vector<std::vector<Edge>>& graph, int start) {
  std::vector<int> dist(n, INF);
  std::vector<std::queue<int>> queues; // в очереди queues[d] храню вершины с длиной d
  queues[0].push(start);
  dist[start] = 0;
  for (int d = 0; d < n * k; ++d) {
    while (!queues[d].empty()) {
      auto v = queues[d].front();
      queues[d].pop();
      if (dist[v] < d) {
        continue;
      }
      for (auto ch : graph[v]) {
        if (dist[ch.vertex] > dist[v] + ch.weight) {
          dist[ch.vertex] = dist[v] + ch.weight;
          queues[dist[ch.vertex]].push(ch.vertex);
        }
      }
    }
  }
  return dist;
}
// Асимптотика O(N * K + E)