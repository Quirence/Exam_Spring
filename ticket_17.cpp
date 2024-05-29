#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>

struct Point {
    double x, y;

    // Перегрузка оператора вычитания для удобства
    Point operator-(const Point& p) const {
        return {x - p.x, y - p.y};
    }

    // Перегрузка оператора сравнения для удобства
    bool operator==(const Point& p) const {
        return x == p.x && y == p.y;
    }

    // Векторное произведение
    double cross(const Point& p) const {
        return x * p.y - y * p.x;
    }
};

// Сравнение точек по углу относительно точки-опорной
bool compare(const Point& p1, const Point& p2, const Point& anchor) {
    Point v1 = p1 - anchor;
    Point v2 = p2 - anchor;
    double crossProduct = v1.cross(v2);
    if (crossProduct == 0) {
        // Если точки коллинеарны, выбираем ближайшую к anchor
        return std::hypot(v1.x, v1.y) < std::hypot(v2.x, v2.y);
    }
    return crossProduct > 0;
}

// Функция для нахождения выпуклой оболочки
std::vector<Point> grahamScan(std::vector<Point>& points) {
    if (points.size() < 3) return points;

    // Находим точку с минимальной y-координатой (и с минимальной x-координатой в случае равенства y)
    Point start = points[0];
    for (const Point& p : points) {
        if (p.x < start.x || (p.x == start.x && p.y < start.y)) {
            start = p;
        }
    }
    
    auto anchor = start;

    // Сортируем точки по углу относительно anchor
    std::sort(points.begin(), points.end(), [anchor](const Point& p1, const Point& p2) {
        return compare(p1, p2, anchor);
    });

    // Инициализируем стек первых трех точек
    std::vector<Point> hull = { points[0], points[1], points[2] };

    // Проходим по остальным точкам
    for (size_t i = 3; i < points.size(); ++i) {
        while (hull.size() > 1 && (hull[hull.size() - 1] - hull[hull.size() - 2]).cross(points[i] - hull[hull.size() - 2]) <= 0) {
            hull.pop_back(); // Удаляем точку, которая не является частью выпуклой оболочки
        }
        hull.push_back(points[i]);
    }

    return hull;
}

// Функция для вывода точек выпуклой оболочки
void printHull(const std::vector<Point>& hull) {
    for (const auto& p : hull) {
        std::cout << "(" << p.x << ", " << p.y << ")\n";
    }
}

int main() {
    std::vector<Point> points = {{0, 3}, {2, 2}, {1, 1}, {2, 1}, {3, 0},
                                 {0, 0}, {3, 3}};

    std::vector<Point> hull = grahamScan(points);
    std::cout << "Points in the convex hull:\n";
    printHull(hull);

    return 0;
}
