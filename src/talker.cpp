#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/u_int16_multi_array.hpp"
#include <iostream>
#include <QApplication>
#include <QColorDialog>
#include <QDebug>
#include <vector>

class PickerColor
{
  public:
    std::vector<uint16_t> Color(){
      QColor color = QColorDialog::getColor(Qt::red);
      if (color.isValid()) {
        uint16_t red = static_cast<uint16_t>(color.red());
        uint16_t green = static_cast<uint16_t>(color.green());
        uint16_t blue = static_cast<uint16_t>(color.blue());
        return {red, green, blue};
      }
      else{
        qDebug() << "Цвет не выбран";
        return {0, 0, 0};
      }
    }
};

class Publisher : public rclcpp::Node
{
  public:
    Publisher()
    : Node("publisher")
    {
      publisher_ = this->create_publisher<std_msgs::msg::UInt16MultiArray>("topic", 10);
    }
    void makeMessage(const std::vector<uint16_t>& array) {
      auto message = std_msgs::msg::UInt16MultiArray();
      message.data = array;
      RCLCPP_INFO(this->get_logger(), "Цвет [%u, %u, %u]", array[0], array[1], array[2]);
      this->publisher_->publish(message);
    }
  private:
    rclcpp::Publisher<std_msgs::msg::UInt16MultiArray>::SharedPtr publisher_;
};


int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  rclcpp::init(argc, argv);
  auto node = std::make_shared<Publisher>();
  PickerColor picker;

  node->makeMessage(picker.Color());
  
  rclcpp::sleep_for(std::chrono::milliseconds(500));
  rclcpp::shutdown();
  return 0;
}