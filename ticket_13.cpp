#include <iostream>
#include <vector>
#include <cstring>

const int nsize = 100;
const int left_size = 4;
bool used[nsize];
int match[nsize];
std::vector<std::vector<int>> graph; // Только ребра из левой доли в правую

bool DFS(int vertex) {
  if (used[vertex]) {
    return false;
  }
  used[vertex] = true;
  for (int neighbor : graph[vertex]) {
    if (match[neighbor] == -1 || DFS(match[neighbor])) {
      match[neighbor] = vertex;
      return true;
    }
  }
  return false;
}

int main() {
  std::memset(match, -1, sizeof(match)); // Инициализируем как -1
  int counter = 0;
  for (int i = 0; i < left_size; ++i) {
    std::memset(used, 0, sizeof(used));
    if (DFS(i)) {
      counter++;
    }
  }
  std::cout << counter;
}

// Асимптотика - O(V*E)