#include <Arduino.h>
#include <ros2arduino.h>
#include <ros2wr.hpp>

ROS2WR<geometry_msgs::Twist, geometry_msgs::Twist> *node;

void MainThread(void*);

void setup() {
  Serial.end();
  Serial.begin(1500000);
  ros2::init(&Serial);
  node = new ROS2WR<geometry_msgs::Twist, geometry_msgs::Twist>(100, "esp32/Pub", "esp32/Sub");
  xTaskCreatePinnedToCore(MainThread, "MainTheread", 1024*16, NULL, 2, nullptr, 0);
}

void loop() {
  ros2::spin(node);
}

void MainThread(void *pvParameters){
  //Setup
  
  while(1){
    //Main
    geometry_msgs::Twist msg;
    node->getSubscribeMsg(&msg);
    node->setPublishMsg(&msg);
    delay(2);
  }
}