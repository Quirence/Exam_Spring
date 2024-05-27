#include <iostream>
#include <algorithm>
#include <cstdint>
#include <vector>
#include <limits>
#include <set>
#define INF INT_MAX

std::vector<int> Dijkstra(std::vector<std::vector<std::pair<int, int>>>& graph, int start) {
  std::vector<int> dist(graph.size(), INF);
  dist[start] = 0;
  std::vector<bool> visited(graph.size());
  for (int i = 0; i < graph.size(); ++i) {
    int nearest = -1;
    for (int vertex = 0; vertex < graph.size(); ++vertex) {
      if (!visited[vertex] && (nearest == -1 || dist[nearest] > dist[vertex])) {
        nearest = vertex;
      }
    }
    visited[nearest] = true;
    for (auto &[to, weight] : graph[nearest]) {
      if (dist[to] > dist[nearest] + weight) {
        dist[to] = dist[nearest] + weight;
      }
    }
  }
  return dist;
}

int main() {
  int nsize = 0;
  int msize = 0;
  std::cin >> nsize >> msize;
  std::vector<std::vector<std::pair<int, int>>> graph(nsize);
  for (int i = 0; i < nsize; ++i) {
    int from = 0;
    int to = 0;
    int weight = 0;
    from--;
    to--;
    graph[from].emplace_back(to, weight);
    graph[to].emplace_back(from, weight);
  }
  int start = 0;
  std::cin >> start;
  start--;
}

// Данная реализация - O(V^2 + E). Улучшим это.


std::vector<int> Fast_Dijkstra(std::vector<std::vector<std::pair<int, int>>>& graph, int start) {
  std::vector<int> dist(graph.size(), INF);
  dist[start] = 0;
  std::set<std::pair<int, int>> queue; // Расстояние до вершины + ее номер
  queue.insert({dist[start], start});
  while (!queue.empty()) {
    int nearest = queue.begin()->second;
    queue.erase(queue.begin());
    for (auto& [to, weight] : graph[nearest]) {
      if (dist[to] > dist[nearest] + weight) {
        queue.erase({dist[to], to});
        dist[to] = dist[nearest] + weight;
        queue.insert({dist[to], to});
      }
    }
  }
  return dist;
}

// Общая сложность алгоритма составляет O((V + E)logV), и первый вариант более эффективен для плотных графов, второй
// для разреженных.