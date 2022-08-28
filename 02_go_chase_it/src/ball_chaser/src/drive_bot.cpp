#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"


class DriveBotService
{
public:
	DriveBotService()
	{
		motor_command_publisher_ = n_.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
		service_ = n_.advertiseService("/ball_chaser/command_robot", &DriveBotService::handle_drive_request, this);
	}
	// service call back function
	bool handle_drive_request(ball_chaser::DriveToTarget::Request& req,ball_chaser::DriveToTarget::Response& res);

private:
	// ROS::ServiceServer service to receive requests and return responses:
	ros::ServiceServer service_;
	// ROS::Publisher motor commands:
	ros::Publisher motor_command_publisher_;
	// Create a ROS NodeHandle object
	ros::NodeHandle n_;

};


bool DriveBotService::handle_drive_request(ball_chaser::DriveToTarget::Request& req,ball_chaser::DriveToTarget::Response& res)
{
	geometry_msgs::Twist motor_command{};
	
	motor_command.linear.x = req.linear_x;
	motor_command.angular.z = req.angular_z;

	motor_command_publisher_.publish(motor_command);

	res.msg_feedback = "Drive: linear x velocity " + std::to_string(motor_command.linear.x) + " and angular z velocity " + std::to_string(motor_command.angular.z);
	ROS_INFO_STREAM(res.msg_feedback);
	return true;
	    
}

int main(int argc, char** argv)
{
	// Initialize a ROS node
	ros::init(argc, argv, "drive_bot");

	DriveBotService drive_bot_service{};

	ros::spin();
	
}
