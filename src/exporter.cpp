#include "exporter.h"
#include <fstream>
#include <cstdlib>
#include <iostream>

bool GnuplotExporter::saveData(const std::vector<Vector2D>& points) const {
    std::ofstream file(data_filename_);
    if (!file.is_open()) return false;
    
    for (const auto& p : points) {
        file << p.x() << " " << p.y() << "\n";
    }
    file.close();
    return true;
}

bool GnuplotExporter::generateScript(double x_min, double x_max,
                                     double y_min, double y_max) const {
    std::ofstream script(script_filename_);
    if (!script.is_open()) return false;

    script << "set terminal pngcairo size 800,800 enhanced font 'Verdana,12'\n";
    script << "set output '" << output_image_ << "'\n";
    script << "set size ratio -1\n";
    script << "set grid lc rgb '#cccccc'\n";
    script << "set border lc rgb '#444444'\n";
    script << "set key top right\n";
    script << "set xlabel 'x' font 'Verdana,11'\n";
    script << "set ylabel 'y' font 'Verdana,11'\n";
    script << "set title 'Релятивистская трансформация фигуры' font 'Verdana,14'\n";
    script << "set xrange [" << x_min << ":" << x_max << "]\n";
    script << "set yrange [" << y_min << ":" << y_max << "]\n";
    script << "plot '" << data_filename_ << "' with lines lw 2 lc rgb '#1f77b4' title 'Трансформированная фигура'\n";
    
    script.close();
    return true;
}

bool GnuplotExporter::renderPlot() const {
    int result = std::system(("gnuplot " + script_filename_).c_str());
    return (result == 0);
}