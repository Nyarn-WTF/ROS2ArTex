#include <Arduino.h>
#include <ros2arduino.h>
#include <ros2wr.hpp>

ROS2WR<geometry_msgs::Vector3, geometry_msgs::Twist> *node;

void setup() {
  enableCore1WDT();
  Serial.end();
  Serial.begin(115200);
  ros2::init(&Serial);
  node = new ROS2WR<geometry_msgs::Vector3, geometry_msgs::Twist>(1, "esp32/Pub", "esp32/Sub");
}

void loop() {
  ros2::spin(node);
}