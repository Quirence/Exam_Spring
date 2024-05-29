#include <iostream>
#include <cmath>

struct Point {
    double x, y;
    
    Point(double _x = 0, double _y = 0) : x(_x), y(_y) {}
};

struct Vector {
    double x, y;
    
    Vector(double _x = 0, double _y = 0) : x(_x), y(_y) {}
    Vector(const Point& A, const Point& B) : x(B.x - A.x), y(B.y - A.y) {}
    
    double length() const {
        return std::sqrt(x * x + y * y);
    }
    
    Vector operator+(const Vector& other) const {
        return Vector(x + other.x, y + other.y);
    }
    
    Vector operator-(const Vector& other) const {
        return Vector(x - other.x, y - other.y);
    }
    
    double dot(const Vector& other) const {
        return x * other.x + y * other.y;
    }
    
    double cross(const Vector& other) const {
        return x * other.y - y * other.x;
    }
};

enum Orientation {
    COLLINEAR,
    CLOCKWISE,
    COUNTERCLOCKWISE
};

// Функция для определения ориентации тройки точек (p, q, r)
Orientation getOrientation(const Point& p, const Point& q, const Point& r) {
    double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return COLLINEAR;
    return (val > 0) ? CLOCKWISE : COUNTERCLOCKWISE;
}

// Проверка пересечения двух отрезков
bool doIntersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2) {
    Orientation o1 = getOrientation(p1, q1, p2);
    Orientation o2 = getOrientation(p1, q1, q2);
    Orientation o3 = getOrientation(p2, q2, p1);
    Orientation o4 = getOrientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4) return true;
    return false;
}

#include <vector>

double polygonArea(const std::vector<Point>& polygon) {
    int n = polygon.size();
    double area = 0.0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area += (polygon[i].x * polygon[j].y) - (polygon[j].x * polygon[i].y);
    }
    return std::abs(area) / 2.0;
}

bool onSegment(const Point& p, const Point& q, const Point& r) {
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
        return true;
    return false;
}

bool isPointInPolygon(const std::vector<Point>& polygon, const Point& p) {
    int n = polygon.size();
    if (n < 3) return false;

    Point extreme(1e10, p.y);
    int count = 0, i = 0;
    do {
        int next = (i + 1) % n;
        if (doIntersect(polygon[i], polygon[next], p, extreme)) {
            if (getOrientation(polygon[i], p, polygon[next]) == COLLINEAR)
                return onSegment(polygon[i], p, polygon[next]);
            count++;
        }
        i = next;
    } while (i != 0);

    return (count % 2 == 1);
}

int main() {
    Point p1(0, 0), p2(10, 0), p3(10, 10), p4(0, 10);
    std::vector<Point> polygon = {p1, p2, p3, p4};
    
    Point testPoint(5, 5);

    std::cout << "Area of polygon: " << polygonArea(polygon) << std::endl;
    std::cout << "Point (5,5) is " << (isPointInPolygon(polygon, testPoint) ? "inside" : "outside") << " the polygon" << std::endl;
    
    return 0;
}
