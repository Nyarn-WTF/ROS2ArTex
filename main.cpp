#include <Arduino.h>
#include <ros2arduino.h>
#include <ros2wr.hpp>

ROS2WR<geometry_msgs::Twist, geometry_msgs::Twist> *node;

void MainThread(void*);

void setup() {
  enableCore1WDT();
  Serial.end();
  Serial.begin(2000000);
  ros2::init(&Serial);
  node = new ROS2WR<geometry_msgs::Twist, geometry_msgs::Twist>(100, "esp32/Pub", "esp32/Sub");
  xTaskCreatePinnedToCore(MainThread, "MainTheread", 1024*16, NULL, 2, nullptr, 1);
}

void loop() {
  ros2::spin(node);
}

void MainThread(void *pvParameters){
  //Setup
  uint64_t n = 0;
  while(1){
    //Main
    geometry_msgs::Twist msg;
    node->getSubscribeMsg(&msg);
    msg.linear.x = n;
    node->setPublishMsg(&msg);
    delay(1);
  }
}