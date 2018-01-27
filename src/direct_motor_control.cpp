#include "ros/ros.h" // needed for everything
#include "motor.h"
#include <wheel/motor_velocities.h>
#include <std_msgs/String.h>
#include <string>

USBMotor m1;

void joy_to_wheel(const wheel::motor_velocities::ConstPtr& mtr){
  m1.update((int)round(mtr->motor_1_velocity));
}

int main(int argc, char **argv){
	//Initialize ROS node
	ros::init(argc,argv,"dumc");
	ros::NodeHandle nh;
  ros::Publisher wheelPub = nh.advertise<std_msgs::String>("wheel/Diagnostics",10);
  ros::Subscriber joySub = nh.subscribe("wheel/Velocity",10,joy_to_wheel);

  std::string usb_port = "/dev/ttyACM0";
  m1.init(usb_port);
  ros::Rate loop_rate(100);
  while (ros::ok()) {
    if(m1.checkData()){
      std_msgs::String newMessage;
      newMessage.data = m1.data_returned;
      wheelPub.publish(newMessage);
    }
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
