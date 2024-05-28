#include <iostream>
#include <algorithm>
#include <vector>
const int MAX_NODES = 1e5;
bool visited[MAX_NODES];
int height[MAX_NODES], lowestReachable[MAX_NODES];
std::vector<std::vector<int>> graph(MAX_NODES);

void DFS(int currentNode, int parentNode = -1) {
  visited[currentNode] = true;
  height[currentNode] = lowestReachable[currentNode] = (parentNode == -1 ? 0 : height[parentNode] + 1);
  // parentNode == -1 означает, что мы в корне
  for (int neighbor : graph[currentNode]) {
    if (neighbor != parentNode) {
      if (visited[neighbor]) { // Если ребро обратное, т.е. DFS уже был в данной вершине
        lowestReachable[currentNode] = std::min(lowestReachable[currentNode], height[neighbor]);
      } else { // если ребро прямое, т.е. DFS встречает его в первый раз
        DFS(neighbor, currentNode); // Запускаем от этого ребра DFS
        lowestReachable[currentNode] = std::min(lowestReachable[currentNode], lowestReachable[neighbor]); // Обновляем lowestReachable

        if (height[currentNode] < lowestReachable[neighbor]) {
          // Выполнено условие на мост.
        }
      }
    }
  }
}

// Асимптотики - O(n + m)

// Глобальные переменные
std::vector<int> used; // массив для отметки посещенных вершин
std::vector<int> articulationPoints; // массив для хранения точек сочленения

void dfs(int vertex, int parent = -1) {
  used[vertex] = 1; // отмечаем вершину как посещенную
  height[vertex] = lowestReachable[vertex] = (parent == -1 ? 0 : height[parent] + 1); // устанавливаем высоту обнаружения и минимальную достижимую высоту
  int children = 0; // счетчик для количества дочерних вершин

  for (int neighbor : graph[vertex]) { // проходим по всем соседям вершины
    if (neighbor != parent) { // если сосед не является родительской вершиной
      if (used[neighbor]) {
        // если сосед уже был посещен, обновляем минимальную высоту для текущей вершины
        lowestReachable[vertex] = std::min(lowestReachable[vertex], height[neighbor]);
      } else {
        // если сосед не был посещен, продолжаем DFS
        dfs(neighbor, vertex);
        lowestReachable[vertex] = std::min(lowestReachable[vertex], lowestReachable[neighbor]); // и тоже обновляем высоту, СТРОГО ПОСЛЕ DFS

        // проверяем, является ли текущая вершина точкой сочленения
        if (lowestReachable[neighbor] >= height[vertex] && parent != -1) {
          articulationPoints.push_back(vertex);
        }
        children++;
      }
    }
  }

  // специальный случай для корня: если у корня больше одного ребенка, он является точкой сочленения
  if (parent == -1 && children > 1) {
    articulationPoints.push_back(vertex);
  }
}
