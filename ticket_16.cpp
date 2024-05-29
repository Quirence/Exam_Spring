#include <vector>
#include <cmath>
#include <iostream>

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

std::vector<Point> jarvis(std::vector<Point> points) {
    if (points.size() < 3) return points;  // В выпуклой оболочке не может быть менее 3 точек

    // Инициализируем точку p0 самой левой (с минимальной x) точкой
    Point start = points[0];
    for (const Point& p : points) {
        if (p.x < start.x || (p.x == start.x && p.y < start.y)) {
            start = p;
        }
    }

    // Инициализируем оболочку первой точкой
    std::vector<Point> hull = {start};

    Point current = start;
    while (true) {
        Point next_point_candidate = points[0];
        for (const Point& p : points) {
            if (p == current) continue;
            double cross_product = (p - current).cross(next_point_candidate - current);
            if (cross_product > 0 || (cross_product == 0 && std::hypot(p.x - current.x, p.y - current.y) > std::hypot(next_point_candidate.x - current.x, next_point_candidate.y - current.y))) {
                next_point_candidate = p;
            }
        }

        // Если следующая точка кандидата совпадает с начальной точкой, это означает,
        // что мы обошли все точки и вернулись в начальную
        if (next_point_candidate == start) {
            break;
        } else {
            hull.push_back(next_point_candidate);
            current = next_point_candidate;
        }
    }

    return hull;
}

int main() {
    // Задаем набор точек
    std::vector<Point> points = {{0, 3}, {2, 2}, {1, 1}, {2, 1}, {3, 0},
                                 {0, 0}, {3, 3}};
    // Вызываем функцию для построения выпуклой оболочки
    std::vector<Point> hull = jarvis(points);
    for (int i = 0; i < hull.size(); ++i) {
        std::cout << hull[i].x << " " << hull[i].y << "\n";
    }
    return 0;
}
