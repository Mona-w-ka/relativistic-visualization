#include <iostream>
#include <iomanip>
#include "curve.h"
#include "exporter.h"
#include "constants.h"

int main() {
    try {
        // 1. Создание исходной кривой (лемниската Бернулли)
        Curve curve(1000);
        std::cout << std::fixed << std::setprecision(4);
        std::cout << "=== Релятивистская визуализация ===\n";
        std::cout << "Исходная кривая: лемниската Бернулли\n";

        // 2. Задание релятивистской скорости (90% от скорости света по оси X, 10% по Y)
        Vector2D velocity(-Constants::SPEED_OF_LIGHT * 0.9, 
                         -Constants::SPEED_OF_LIGHT * 0.1);
        
        double v_abs = velocity.magnitude();
        std::cout << "Абсолютная скорость: " 
                  << v_abs / Constants::SPEED_OF_LIGHT << "c ("
                  << v_abs << " м/с)\n";

        // 3. Применение релятивистского преобразования
        curve.applyRelativisticTransformation(velocity);
        std::cout << "Фактор лоренцева сокращения: " 
                  << curve.getContractionFactor() << "\n";

        // 4. Экспорт результатов
        GnuplotExporter exporter;
        const auto& points = curve.getPoints();
        
        if (!exporter.saveData(points)) {
            throw std::runtime_error("Ошибка сохранения данных");
        }

        double x_min, x_max, y_min, y_max;
        curve.getBoundingBox(x_min, x_max, y_min, y_max);
        
        if (!exporter.generateScript(x_min, x_max, y_min, y_max)) {
            throw std::runtime_error("Ошибка генерации скрипта Gnuplot");
        }

        if (!exporter.renderPlot()) {
            std::cerr << "Внимание: не удалось запустить Gnuplot. "
					<< "Скрипт сохранён в " << exporter.getScriptFilename() << "\n";
        } else {
            std::cout << "График успешно сохранён в " 
                      << exporter.getOutputImage() << "\n";
        }

        // 5. Информация о границах фигуры
        std::cout << "\nГраницы трансформированной фигуры:\n";
        std::cout << "  x ∈ [" << x_min << ", " << x_max << "]\n";
        std::cout << "  y ∈ [" << y_min << ", " << y_max << "]\n";

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}