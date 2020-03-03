#ifndef _ROS2WR_
#define _ROS2WR_

#include <Arduino.h>
#include <ros2arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

//if QUEUE_SIZE > 2, you can not use xQueueOverwrite()!!!
//also use xQueueSend()
#define QUEUE_SIZE 1

template<typename MsgTx, typename MsgRx>
class ROS2WR : public ros2::Node{
private:
  ros2::Publisher<MsgTx> *_publisher;
  QueueHandle_t command_q, status_q;
  
  static void subsclibed(MsgRx* msg, void *arg){
    (void)(arg);
    static ROS2WR<MsgTx, MsgRx> *_this = ROS2WR<MsgTx, MsgRx>::thisPtr;
    xQueueOverwriteFromISR(_this->command_q, msg, NULL);
  }

  static void publishing(MsgTx* msg, void *arg){
    (void)(arg);
    static ROS2WR<MsgTx, MsgRx> *_this = ROS2WR<MsgTx, MsgRx>::thisPtr;
    static MsgTx *qmsg;
    if(xQueueReceiveFromISR(_this->status_q, qmsg, NULL) == pdTRUE){
        msg = qmsg;
    }
  }

public:
  static ROS2WR<MsgTx, MsgRx>* thisPtr;

  ROS2WR(uint32_t pubFreq, String pubTopicName, String subTopicName):Node("esp32"){
    this->thisPtr = this;
    command_q = xQueueCreate(QUEUE_SIZE, sizeof(MsgRx));
    status_q = xQueueCreate(QUEUE_SIZE, sizeof(MsgTx));
    this->_publisher = this->createPublisher<MsgTx>(pubTopicName.c_str());
    this->createWallFreq(pubFreq, (ros2::CallbackFunc)this->publishing, nullptr, _publisher);
    this->createSubscriber<MsgRx>(subTopicName.c_str(), (ros2::CallbackFunc)this->subsclibed, nullptr);
  }

  void setPublishMsg(MsgTx *msg){
      xQueueSend(this->status_q, (*msg), 0);
  }

  MsgRx getSubscribeMsg(){
      static MsgRx *buff;
      xQueueReceive(this->command_q, buff, 0);
      return (*buff);
  }
};

template <typename MsgTx, typename MsgRx>
ROS2WR<MsgTx, MsgRx>* ROS2WR<MsgTx, MsgRx>::thisPtr;

#endif