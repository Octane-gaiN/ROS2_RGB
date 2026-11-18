#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/u_int16_multi_array.hpp"
#include <iostream>
#include <QApplication>
#include <QColorDialog>
#include <QDebug>
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QColor color = QColorDialog::getColor(Qt::red);
    if (color.isValid()) {
        uint32_t red = color.red();
        uint32_t green = color.green();
        uint32_t blue = color.blue();
        uint32_t array[3] = {red, green, blue};
        qDebug() << array << red << green << blue; 
        qDebug() << QString::number(((red << 16) | (green << 8) | blue), 16);
        qDebug() << "Выбранный цвет:" << color.name();
    } else {
        qDebug() << "Выбор цвета отменен";
    }
    return 0;
}