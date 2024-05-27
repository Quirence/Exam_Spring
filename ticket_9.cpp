#include <algorithm>
#include <vector>
#include <iostream>
#define INF INT_MAX
// Флойд-Уоршелл

int main() {
  int nsize = 0;
  int msize = 0;
  std::cin >> nsize >> msize;
  std::vector<std::vector<int>> dist(nsize, std::vector<int>(nsize, INF));
  for (int v = 0; v < nsize; ++v) {
    dist[v][v] = 0;
  }
  for (int i = 0; i < msize; ++i) {
    int from = 0;
    int to = 0;
    int weight = 0;
    std::cin >> from >> to >> weight;
    dist[from][to] = std::min(dist[from][to], weight);
  }
  for (int v = 0; v < nsize; ++v) {
    for (int from = 0; from < nsize; ++from) {
      for (int to = 0; to < nsize; ++to) {
        if (dist[from][v] != INF && dist[v][to] != INF &&
        dist[from][to] > dist[from][v] + dist[v][to]) {
          dist[from][to] = dist[from][v] + dist[v][to];
        }
      }
    }
  }
}