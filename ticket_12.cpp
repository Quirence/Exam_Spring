#include <iostream>
#include <vector>
#include <algorithm> // для std::sort

class DSU {
  int size_;
  std::vector<int> parents_;
  std::vector<int> rank_;
 public:
  explicit DSU(int size) : size_(size), parents_(size), rank_(size, 0) {
    for (int i = 0; i < size; ++i) {
      parents_[i] = i;
    }
  }
  
  int Leader(int node) {
    if (node != parents_[node]) {
      parents_[node] = Leader(parents_[node]); // Эвристика пути
    }
    return parents_[node];
  }
  
  void Unite(int first, int second) {
    first = Leader(first);
    second = Leader(second);
    if (first != second) { // Объединяем ТОЛЬКО если они в разных множествах
      if (rank_[first] > rank_[second]) {
        std::swap(first, second);
      }
      parents_[first] = second;
      if (rank_[first] == rank_[second]) {
        rank_[second]++;
      }
    }
  }
};
// Граф лучше всего хранить как список рёбер
struct Edge {
  int from, to, weight;
  
  bool operator<(const Edge& other) const {
    return weight < other.weight;
  } // Изнасилование С++
};

int Kruskal(int n, std::vector<Edge>& edges) {
  // Сортируем ребра по весу
  std::sort(edges.begin(), edges.end()); // Гениально...
  
  DSU dsu(n); // Создаем систему непересекающихся множеств размера n_vertices
  int mst_weight = 0; // Общий вес MST
  
  for (const auto& edge : edges) {
    if (dsu.Leader(edge.from) != dsu.Leader(edge.to)) {
      dsu.Unite(edge.from, edge.to);
      mst_weight += edge.weight;
      // std::cout << "Edge (" << edge.from << ", " << edge.to << ") with weight " << edge.weight << " added to MST.\n";
    }
  }
  
  return mst_weight;
}

// Пример работы.
int main() {
  int n = 5; // количество вершин
  std::vector<Edge> edges = {
    {0, 1, 10},
    {0, 2, 6},
    {0, 3, 5},
    {1, 3, 15},
    {2, 3, 4}
  };
  
  int mst_weight = Kruskal(n, edges);
  std::cout << "Total weight of MST: " << mst_weight << std::endl;
  
  return 0;
}
// Асимптотика - O(M*LogM) - скорость сортировки рёбер