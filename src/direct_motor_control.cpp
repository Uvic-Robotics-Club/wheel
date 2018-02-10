#include "ros/ros.h" // needed for everything
#include "motor.h"
#include <wheel/motor_velocities.h>
#include <std_msgs/String.h>
#include <string>

USBMotor m1; bool m1_here = false;
USBMotor m2; bool m2_here = false;
USBMotor m3; bool m3_here = false;
USBMotor m4; bool m4_here = false;
USBMotor m5; bool m5_here = false;
USBMotor m6; bool m6_here = false;

void joy_to_wheel(const wheel::motor_velocities::ConstPtr& mtr){
	if(m1_here){m1.update((int)round(mtr->motor_1_velocity));}
	if(m2_here){m2.update((int)round(mtr->motor_2_velocity));}
	if(m3_here){m3.update((int)round(mtr->motor_3_velocity));}
	if(m4_here){m4.update((int)round(mtr->motor_4_velocity));}
	if(m5_here){m5.update((int)round(mtr->motor_5_velocity));}
	if(m6_here){m6.update((int)round(mtr->motor_6_velocity));}
}

int main(int argc, char **argv){
	//Initialize ROS node
	ros::init(argc,argv,"dumc");
	ros::NodeHandle nh;
	ros::NodeHandle pnh("~");
	ros::Publisher wheelPub = nh.advertise<std_msgs::String>("wheel/Diagnostics",10);
	ros::Subscriber joySub = nh.subscribe("wheel/Velocity",10,joy_to_wheel);

	std::string param_name;
	if (pnh.searchParam("m1_usb_port", param_name)){
		std::string usb_port = "";
		pnh.getParam(param_name, usb_port);
		ROS_ERROR_STREAM("Creating Motor 1 at " << usb_port);
		m1.init(usb_port);
		m1_here = true;
	}
	else{ROS_ERROR("Motor 1 Not found, Not attaching");}
	
	if (pnh.searchParam("m2_usb_port", param_name)){
		std::string usb_port = "";
		pnh.getParam(param_name, usb_port);
		ROS_ERROR_STREAM("Creating Motor 2 at " << usb_port);
		m2.init(usb_port);
		m2_here = true;
	}
	else{ROS_ERROR("Motor 2 Not found, Not attaching");}
	
	if (pnh.searchParam("m3_usb_port", param_name)){
		std::string usb_port = "";
		pnh.getParam(param_name, usb_port);
		ROS_ERROR_STREAM("Creating Motor 3 at " << usb_port);
		m3.init(usb_port);
		m3_here = true;
	}
	else{ROS_ERROR("Motor 3 Not found, Not attaching");}
	
	if (pnh.searchParam("m4_usb_port", param_name)){
		std::string usb_port = "";
		pnh.getParam(param_name, usb_port);
		ROS_ERROR_STREAM("Creating Motor 4 at " << usb_port);
		m4.init(usb_port);
		m4_here = true;
	}
	else{ROS_INFO("Motor 4 Not found, Not attaching");}
	
	if (pnh.searchParam("m5_usb_port", param_name)){
		std::string usb_port = "";
		pnh.getParam(param_name, usb_port);
		ROS_ERROR_STREAM("Creating Motor 5 at " << usb_port);
		m5.init(usb_port);
		m5_here = true;
	}
	else{ROS_ERROR("Motor 5 Not found, Not attaching");}
	
	if (pnh.searchParam("m6_usb_port", param_name)){
		std::string usb_port = "";
		pnh.getParam(param_name, usb_port);
		ROS_ERROR_STREAM("Creating Motor 6 at " << usb_port);
		m6.init(usb_port);
		m6_here = true;
	}
	else{ROS_ERROR("Motor 6 Not found, Not attaching");}


	ros::Rate loop_rate(100);
	while (ros::ok()) {
		
		if(m1_here and m1.checkData()){
			std_msgs::String newMessage;
			newMessage.data = "Motor1: " + m1.data_returned;
			wheelPub.publish(newMessage);
		}
		if(m2_here and m2.checkData()){
			std_msgs::String newMessage;
			newMessage.data = "Motor2: " + m2.data_returned;
			wheelPub.publish(newMessage);
		}
		if(m3_here and m3.checkData()){
			std_msgs::String newMessage;
			newMessage.data = "Motor3: " + m3.data_returned;
			wheelPub.publish(newMessage);
		}
		if(m4_here and m4.checkData()){
			std_msgs::String newMessage;
			newMessage.data = "Motor4: " + m4.data_returned;
			wheelPub.publish(newMessage);
		}
		if(m5_here and m5.checkData()){
			std_msgs::String newMessage;
			newMessage.data = "Motor5: " + m5.data_returned;
			wheelPub.publish(newMessage);
		}
		if(m6_here and m6.checkData()){
			std_msgs::String newMessage;
			newMessage.data = "Motor6: " + m6.data_returned;
			wheelPub.publish(newMessage);
		}
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}
