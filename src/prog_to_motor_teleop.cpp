#include "ros/ros.h" // needed for everything
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>
#include <wheel/motor_velocities.h>

#define PI 3.14159265

ros::Publisher wheelPub;

double mapValue(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Velocity commands callback function

void prog_to_wheel(const geometry_msgs::Twist::ConstPtr& twi){
  static double turn = 0;
  static double left = 0;
  static double right = 0;
  static double speed = 0;

  left = 0;
  right = 0;
  speed = twi->linear.x;
  turn = twi->angular.z;
  ROS_INFO("speed: %.3f, turn: %.3f",speed, turn);
  left  = turn>0 ? speed : speed*cos(turn);
  right = turn<0 ? speed : speed*cos(turn);

  wheel::motor_velocities newMessage;

  newMessage.motor_1_velocity = left;
  newMessage.motor_2_velocity = left;
  newMessage.motor_3_velocity = left;
  newMessage.motor_4_velocity = right;
  newMessage.motor_5_velocity = right;
  newMessage.motor_6_velocity = right;

  wheelPub.publish(newMessage);

}

int main(int argc, char **argv){
	//Initialize ROS node
	ros::init(argc,argv,"wheel_prog_teleop");
	ros::NodeHandle nh;
  wheelPub = nh.advertise<wheel::motor_velocities>("wheel/Velocity",10);
  ros::Subscriber joySub = nh.subscribe("cmd_vel",10,prog_to_wheel);
  //ros::Rate throttle(20);
  while(ros::ok()){
    ros::spinOnce();
    //throttle.sleep();
  }

  return 0;
}
