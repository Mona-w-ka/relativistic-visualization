#pragma once
#include <cmath>

/** Класс для работы с 2D-векторами */
class Vector2D {
private:
    double x_, y_;

public:
    Vector2D(double x = 0.0, double y = 0.0) : x_(x), y_(y) {}

    // Геттеры
    double x() const { return x_; }
    double y() const { return y_; }

    // Операции
    double magnitude() const { return std::sqrt(x_ * x_ + y_ * y_); }
    double dot(const Vector2D& other) const { 
        return x_ * other.x_ + y_ * other.y_; 
    }
    Vector2D normalized() const {
        double m = magnitude();
        return (m > 1e-12) ? Vector2D(x_ / m, y_ / m) : Vector2D();
    }

    // Поворот на угол (радианы)
    Vector2D rotated(double angle) const {
        double cos_a = std::cos(angle);
        double sin_a = std::sin(angle);
        return Vector2D(
            x_ * cos_a - y_ * sin_a,
            x_ * sin_a + y_ * cos_a
        );
    }

    // Угол вектора относительно оси X
    double angle() const { 
        return std::atan2(y_, x_); 
    }
};