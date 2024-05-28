#include <iostream>
#include <vector>
#include <utility>
#include <limits.h> // Для INT_MAX

const int MAX_NODES = 1e5;
const int INFINITY = INT_MAX; // Максимальное значение для целого числа

bool used[MAX_NODES]; // Массив для отметки посещенных вершин
std::vector<std::vector<std::pair<int, int>>> graph(MAX_NODES); // Список смежности для графа
std::vector<int> min_edge(MAX_NODES, INFINITY); // Массив минимальных весов для ребер
std::vector<int> best_edge(MAX_NODES); // Массив для хранения вершины, из которой ведет минимальное ребро

int Prim() {
  min_edge[0] = 0; // Минимальный вес ребра для начальной вершины (предположим, что это 0)
  
  for (int i = 0; i < graph.size(); i++) {
    int current_vertex = -1;
    // Поиск неиспользованной вершины с минимальным значением в массиве min_edge
    for (int u = 0; u < graph.size(); u++) {
      if (!used[u] && (current_vertex == -1 || min_edge[u] < min_edge[current_vertex])) {
        current_vertex = u;
      }
    }

    used[current_vertex] = true; // Отмечаем вершину как использованную

    // Если вершина не начальная, выводим ребро, связывающее её с MST
    if (current_vertex != 0) {
      std::cout << current_vertex << " " << best_edge[current_vertex] << std::endl;
    }

    // Обновляем минимальные веса ребер для смежных вершин
    for (auto edge : graph[current_vertex]) {
      int adjacent_vertex = edge.first;
      int weight = edge.second;
      // Проверяем, можно ли улучшить минимальный вес для смежной вершины
      if (weight < min_edge[adjacent_vertex]) {
        min_edge[adjacent_vertex] = weight; // Обновляем минимальный вес ребра для смежной вершины
        best_edge[adjacent_vertex] = current_vertex; // Запоминаем, из какой вершины ведет это ребро
      }
    }
  }
}

// Асимтпотика - O (N^2). Также, как в алгоритме Дейкстры, можно не делать линейный поиск оптимальной вершины
// а поддерживать его в приоритетной очереди. Получается реализация за O(M*logN)
