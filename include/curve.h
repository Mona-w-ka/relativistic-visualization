#pragma once
#include <vector>
#include <stdexcept>
#include "vector2d.h"
#include "constants.h"

/** Класс для параметрической кривой и релятивистских преобразований */
class Curve {
private:
    std::vector<Vector2D> points_;
    double contraction_factor_; // sqrt(1 - v²/c²)

    // Генерация лемнискаты Бернулли
    void generateLemniscate(int n_points) {
        points_.clear();
        for (int i = 0; i < n_points; ++i) {
            double t = 4.0 * Constants::PI * i / (n_points - 1);
            double denom = 1.0 + std::sin(t) * std::sin(t);
            double x = std::sqrt(2.0) * std::cos(t) / denom;
            double y = std::sqrt(2.0) * std::cos(t) * std::sin(t) / denom;
            points_.emplace_back(x, y);
        }
    }

public:
    Curve(int n_points = 1000) {
        if (n_points < 2) 
            throw std::invalid_argument("Количество точек должно быть >= 2");
        generateLemniscate(n_points);
        contraction_factor_ = 1.0; // Начальное состояние без сокращения
    }

    // Применение релятивистского преобразования
    void applyRelativisticTransformation(const Vector2D& velocity) {
        double v_abs = velocity.magnitude();
        
        // Валидация скорости
        if (v_abs >= Constants::SPEED_OF_LIGHT * Constants::MAX_SPEED_RATIO) {
            throw std::invalid_argument(
                "Скорость превышает допустимый предел (должна быть < 0.999c)"
            );
        }

        // Вычисление фактора лоренцева сокращения
        contraction_factor_ = std::sqrt(1.0 - (v_abs * v_abs) / 
                                       (Constants::SPEED_OF_LIGHT * Constants::SPEED_OF_LIGHT));

        // 1. Поворот системы так, чтобы вектор скорости совпал с осью X
        double rotation_angle = -velocity.angle();
        for (auto& p : points_) {
            p = p.rotated(rotation_angle);
        }

        // 2. Лоренцево сокращение ТОЛЬКО по оси X (вдоль движения)
        for (auto& p : points_) {
            p = Vector2D(p.x() * contraction_factor_, p.y());
        }

        // 3. Обратный поворот в исходную систему координат
        rotation_angle = -rotation_angle;
        for (auto& p : points_) {
            p = p.rotated(rotation_angle);
        }
    }

    // Получение точек для экспорта
    const std::vector<Vector2D>& getPoints() const { return points_; }

    // Диапазоны координат с отступами
    void getBoundingBox(double& x_min, double& x_max, double& y_min, double& y_max, 
                        double padding_ratio = 0.1) const {
        x_min = x_max = points_[0].x();
        y_min = y_max = points_[0].y();

        for (const auto& p : points_) {
            if (p.x() < x_min) x_min = p.x();
            if (p.x() > x_max) x_max = p.x();
            if (p.y() < y_min) y_min = p.y();
            if (p.y() > y_max) y_max = p.y();
        }

        double x_pad = (x_max - x_min) * padding_ratio;
        double y_pad = (y_max - y_min) * padding_ratio;
        x_min -= x_pad; x_max += x_pad;
        y_min -= y_pad; y_max += y_pad;
    }

    double getContractionFactor() const { return contraction_factor_; }
};