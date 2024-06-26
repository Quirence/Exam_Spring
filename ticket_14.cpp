#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

// Структура для представления ребра графа
struct Edge {
    size_t from;        // Начальная вершина ребра
    size_t to;          // Конечная вершина ребра
    int64_t capacity;   // Пропускная способность ребра
    
    // Конструктор по умолчанию
    Edge() : Edge(0, 0, 0) {
    }
    
    // Конструктор с параметрами
    Edge(size_t from, size_t to, int64_t capacity) : from(from), to(to), capacity(capacity) {
    }
};

// Функция для поиска пути с использованием поиска в глубину (DFS)
bool dfs(size_t currentNode, size_t sink, std::vector<bool>& visited, std::vector<int64_t>& parent, const std::vector<std::vector<int64_t>>& residualCapacity) {
    visited[currentNode] = true;  // Отмечаем текущий узел как посещённый
    
    // Если достигли стока, возвращаем true
    if (currentNode == sink) {
        return true;
    }

    // Проверяем всех соседей текущего узла
    for (size_t nextNode = 0; nextNode < residualCapacity.size(); ++nextNode) {
        // Если сосед не посещён и остаточная пропускная способность больше нуля
        if (!visited[nextNode] && residualCapacity[currentNode][nextNode] > 0) {
            parent[nextNode] = static_cast<int64_t>(currentNode);  // Запоминаем родителя для восстановления пути
            // Рекурсивно ищем путь через nextNode
            if (dfs(nextNode, sink, visited, parent, residualCapacity)) {
                return true;
            }
        }
    }

    return false;  // Путь не найден
}

// Алгоритм Форда-Фалкерсона для нахождения максимального потока
void FordFulkerson(const std::vector<Edge>& graph, size_t numVertices, size_t source, size_t sink) {
    // Матрица остаточной пропускной способности
    std::vector<std::vector<int64_t>> residualCapacity(numVertices, std::vector<int64_t>(numVertices, 0));

    // Инициализация остаточной пропускной способности
    for (const auto& edge : graph) {
        residualCapacity[edge.from][edge.to] = edge.capacity;
    }

    // Вектор для хранения пути, найденного DFS
    std::vector<int64_t> parent(numVertices, -1);
    int64_t maxFlow = 0;  // Инициализация максимального потока

    // Пока существует путь от истока к стоку в остаточной сети
    while (true) {
        std::vector<bool> visited(numVertices, false);  // Вектор посещённых вершин

        // Ищем путь от истока к стоку
        if (!dfs(source, sink, visited, parent, residualCapacity)) {
            break;  // Если путь не найден, выходим из цикла
        }

        // Находим минимальную пропускную способность на найденном пути (ПРОСТО ТО ЗНАЧЕНИЕ, КОТОРОЕ БУДЕМ ИЗПОЛЬЗОВАТЬ КАК МАКСИМУМ НА ПУТИ)
        int64_t pathFlow = INT64_MAX;
        for (size_t v = sink; v != source; v = parent[v]) {
            size_t u = parent[v];
            pathFlow = std::min(pathFlow, residualCapacity[u][v]);
        }

        // Тут тоже идем от конца к началу (так уж работает DFS). Обновляем все в итоговой матрице по правилу:
        // Если идем по ребру, то добавляем, если против ребра, то убираем
        for (size_t v = sink; v != source; v = parent[v]) {
            size_t u = parent[v];
            residualCapacity[u][v] -= pathFlow;
            residualCapacity[v][u] += pathFlow;
        }

        // Добавляем текущий поток к общему потоку
        maxFlow += pathFlow;
    }

    std::cout << "Maximum flow: " << maxFlow << std::endl;
}

int main() {
    size_t numVertices = 6; // Пример количества вершин
    size_t source = 0;      // Исток
    size_t sink = 5;        // Сток

    // Пример графа в виде списка ребер
    std::vector<Edge> graph = {
        Edge(0, 1, 16),
        Edge(0, 2, 13),
        Edge(1, 2, 10),
        Edge(1, 3, 12),
        Edge(2, 1, 4),
        Edge(2, 4, 14),
        Edge(3, 2, 9),
        Edge(3, 5, 20),
        Edge(4, 3, 7),
        Edge(4, 5, 4)
    };

    // Запуск алгоритма Форда-Фалкерсона
    FordFulkerson(graph, numVertices, source, sink);

    return 0;
}

// BFS Для Эдмонса-Карпа
//bool bfs(const std::vector<std::vector<int>>& capacity_matrix, std::vector<std::vector<int>>& residual_capacity, int source, int sink, std::vector<int>& parent) {
//    int number_of_vertices = capacity_matrix.size();
//    std::vector<bool> visited(number_of_vertices, false);
//    
//    std::queue<int> bfs_queue;
//    bfs_queue.push(source);
//    visited[source] = true;
//    parent[source] = -1;  // У источника нет родителя
//
//    while (!bfs_queue.empty()) {
//        int current_vertex = bfs_queue.front();
//        bfs_queue.pop();
//
//        for (int adjacent_vertex = 0; adjacent_vertex < number_of_vertices; adjacent_vertex++) {
//            if (!visited[adjacent_vertex] && residual_capacity[current_vertex][adjacent_vertex] > 0) {
//                // Если вершина ещё не посещена и есть остаточная пропускная способность
//                bfs_queue.push(adjacent_vertex);
//                parent[adjacent_vertex] = current_vertex;  // Запоминаем родителя
//                visited[adjacent_vertex] = true;
//
//                // Если мы достигли стока, то возвращаем true
//                if (adjacent_vertex == sink) {
//                    return true;
//                }
//            }
//        }
//    }
//
//    // Если не удалось найти путь до стока, возвращаем false
//    return false;
//}