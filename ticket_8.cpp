#include <algorithm>
#include <vector>
#define INF INT_MAX
// Простой Форд-Беллман

struct Edge {
  int from_;
  int to_;
  int weight_;
  Edge(int from, int to, int weight) : from_(from), to_(to), weight_(weight) {
  };
};

std::vector<int> FordBellman(std::vector<Edge> edges, int start, int size) {
  std::vector<int> dist(size, INF);
  dist[start] = 0;
  for (int v = 0; v < size - 1; ++v) {
    for (auto& [from, to, weight] : edges) {
      if (dist[from] != INF && dist[to] > dist[from] + weight) {
        dist[to] = dist[from] + weight;
      }
    }
  }
  return dist;
}

// Асимптотика - O(V * E)