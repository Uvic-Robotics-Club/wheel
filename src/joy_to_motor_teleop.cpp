#include "ros/ros.h" // needed for everything
#include <sensor_msgs/Joy.h>
#include <wheel/motor_velocities.h>

#define PI 3.14159265

// array locations of AXES
int left_right_left = 0;
int up_down_left = 1;
int trigger_left  = 2;
int left_right_right = 3;
int up_down_right = 4;
int trigger_right = 5;
int left_right_hat = 6;
int up_down_hat = 7;

// array locations of button
int a_button = 0;
int b_button = 1;
int x_button = 2;
int y_button = 3;
int lb_button = 4;
int rb_button = 5;
int back_button = 6;
int start_button = 7;
int power_button = 8;
int left_stick_button = 9;
int right_stick_button = 10;

ros::Publisher wheelPub;

double mapValue(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Velocity commands callback function

void joy_to_wheel(const sensor_msgs::Joy::ConstPtr& joy){
  static double turn = 0;
  static double forward = 0;
  static double reverse = 0;

  forward = mapValue(joy->axes[trigger_right],1,-1,0,1);
  reverse = mapValue(joy->axes[trigger_left],1,-1,0,1);
  turn = cos(mapValue(joy->axes[left_right_left],-1,1,-PI,PI));

  wheel::motor_velocities newMessage;

  newMessage.motor_1_velocity = (forward-reverse)*(1+turn);
  newMessage.motor_2_velocity = (forward-reverse)*(1+turn);
  newMessage.motor_3_velocity = (forward-reverse)*(1+turn);
  newMessage.motor_4_velocity = (forward-reverse)*(1+turn);
  newMessage.motor_5_velocity = (forward-reverse)*(1+turn);
  newMessage.motor_6_velocity = (forward-reverse)*(1+turn);

  wheelPub.publish(newMessage);

}

int main(int argc, char **argv){
	//Initialize ROS node
	ros::init(argc,argv,"wheel_joy_teleop");
	ros::NodeHandle nh;
  wheelPub = nh.advertise<wheel::motor_velocities>("wheel/Velocity",10);
  ros::Subscriber joySub = nh.subscribe("wheel_joy",10,joy_to_wheel);
  ros::Rate throttle(20);
  while(ros::ok()){
    ros::spinOnce();
    throttle.sleep();
  }

  return 0;
}
