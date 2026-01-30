#pragma once
#include <string>
#include <vector>
#include "vector2d.h"

/** Класс для экспорта данных и генерации скриптов Gnuplot */
class GnuplotExporter {
private:
    std::string data_filename_;
    std::string script_filename_;
    std::string output_image_;

public:
    GnuplotExporter(const std::string& data_file = "figure.dat",
                    const std::string& script_file = "plot_figure.gnu",
                    const std::string& image_file = "figure.png")
        : data_filename_(data_file), 
          script_filename_(script_file), 
          output_image_(image_file) {}

    // Сохранение точек кривой в файл
    bool saveData(const std::vector<Vector2D>& points) const;
    
    // Генерация скрипта Gnuplot с фиксированной сеткой координат
    bool generateScript(double x_min, double x_max, 
                        double y_min, double y_max) const;
    
    // Выполнение Gnuplot (требует установленной утилиты)
    bool renderPlot() const;

    std::string getOutputImage() const { return output_image_; }

    std::string getDataFilename() const { return data_filename_; }
    std::string getScriptFilename() const { return script_filename_; }
    
	
	};