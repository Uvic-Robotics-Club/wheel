#include "motor.h"
#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "ros/ros.h" // needed for everything
#define PI 3.14159265

USBMotor::USBMotor(std::string usb_port_input = "/dev/ttyACM0"){
  usb_port = usb_port_input;
  wheel_radius = 0.0508; // 2 inch radius
  ser;
  try{
		ser.setPort(usb_port);
		ser.setBaudrate(9600);
		serial::Timeout to = serial::Timeout::simpleTimeout(1000);
		ser.setTimeout(to);
		ser.open();
    ROS_INFO_STREAM("Trying to open the motor at "<< usb_port);
    ros::Duration(0.5).sleep(); // sleep for half a second
  }
  catch(serial::IOException& e){
		throw std::invalid_argument( "Unable to open the usb port (likely its the wrong usb name or its busy)" );
	}
	if(ser.isOpen()){

	}
  else{
    ROS_ERROR("The port seems to be closed");
		throw std::invalid_argument( "Unable to open the Serial Communication port to the Motor Driver" );
	}
}

USBMotor::USBMotor(){
  wheel_radius = 0.0508; // 2 inch radius
}

void USBMotor::init(std::string usb_port_input = "/dev/ttyACM0"){
  usb_port = usb_port_input;
  try{
		ser.setPort(usb_port);
		ser.setBaudrate(9600);
		serial::Timeout to = serial::Timeout::simpleTimeout(1000);
		ser.setTimeout(to);
		ser.open();
    ROS_INFO_STREAM("Trying to open the motor at "<< usb_port);
    ros::Duration(0.5).sleep(); // sleep for half a second
  }
  catch(serial::IOException& e){
		throw std::invalid_argument( "Unable to open the usb port (likely its the wrong usb name or its busy)" );
	}
	if(ser.isOpen()){

	}
  else{
    ROS_ERROR("The port seems to be closed");
		throw std::invalid_argument( "Unable to open the Serial Communication port to the Motor Driver" );
	}
}

void USBMotor::update(int vel = 0){
  checkData();
  setVelocity(vel);
  sendData();
}

std::vector<std::string> USBMotor::split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

bool USBMotor::checkData(){
  std::vector<std::string> valuesArray;
  data_returned = "";
  int mode = 0;
  double value = 0;
  //ROS_INFO_STREAM("ser.available returns: " << ser.available());
  if(ser.available()>1){
    ser.readline(data_returned);
    valuesArray = split(data_returned,',');
    if(valuesArray.size()%2 == 0){
      while(valuesArray.size() != 0){
        mode = atoi((*valuesArray.begin()).c_str());
        valuesArray.erase (valuesArray.begin());
        value =atof((*valuesArray.begin()).c_str());
        valuesArray.erase (valuesArray.begin());
        switch (mode) {
          case 0:
            distance = value*2*PI*wheel_radius;
            break;
          case 1:
            a_input = value;
            break;
          case 2:
            a_output = value;
            break;
          case 3:
            a_setpoint = value;
            break;
          default:
            distance = value;
        };
      }
    }
    return true;
  }
  return false;
}

void USBMotor::setVelocity(int vel=0){
  if(vel >= 0){
    vel  = (vel>255 ? 255 : vel);
    data_to_write.push_back(0);
    data_to_write.push_back((uint8_t)vel);
  }
  if(vel < 0){
    vel = abs(vel);
    vel  = (vel>255 ? 255 : vel);
    data_to_write.push_back(1);
    data_to_write.push_back((uint8_t)vel);
  }
}

void USBMotor::sendData(){
  if(ser.isOpen() && !data_to_write.empty()){
    ser.write(data_to_write);
  }
}
