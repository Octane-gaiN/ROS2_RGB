#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/u_int16_multi_array.h>

#define PIN_RED   25
#define PIN_GREEN 26
#define PIN_BLUE  27

uint16_t rgb_buffer[3];

rcl_subscription_t subscriber;
std_msgs__msg__UInt16MultiArray msg;
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

void subscription_callback(const void * msgin) {
    const std_msgs__msg__UInt16MultiArray * msg_ptr = (const std_msgs__msg__UInt16MultiArray *)msgin;
    
    if (msg_ptr->data.size >= 3) {
        uint16_t r = msg_ptr->data.data[0];
        uint16_t g = msg_ptr->data.data[1];
        uint16_t b = msg_ptr->data.data[2];

        r = (r > 255) ? 255 : r;
        g = (g > 255) ? 255 : g;
        b = (b > 255) ? 255 : b;

      analogWrite(PIN_RED, r);
      analogWrite(PIN_GREEN, g);
      analogWrite(PIN_BLUE, b);

    }
}

void setup() {
    pinMode(PIN_RED, OUTPUT);
    pinMode(PIN_GREEN, OUTPUT);
    pinMode(PIN_BLUE, OUTPUT);
    
    analogWrite(PIN_RED, 0);
    analogWrite(PIN_GREEN, 0);
    analogWrite(PIN_BLUE, 0);

    set_microros_transports();

    allocator = rcl_get_default_allocator();

    rclc_support_init(&support, 0, NULL, &allocator);

    rclc_node_init_default(&node, "esp32_rgb_node", "", &support);

    msg.data.capacity = 3;
    msg.data.size = 0;
    msg.data.data = rgb_buffer;
    
    msg.layout.dim.capacity = 0;
    msg.layout.dim.size = 0;
    msg.layout.dim.data = NULL;
    msg.layout.data_offset = 0;

    rclc_subscription_init_default(
        &subscriber,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, UInt16MultiArray),
        "topic");

    rclc_executor_init(&executor, &support.context, 1, &allocator);
    rclc_executor_add_subscription(&executor, &subscriber, &msg, &subscription_callback, ON_NEW_DATA);
}

void loop() {
    rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10));
}