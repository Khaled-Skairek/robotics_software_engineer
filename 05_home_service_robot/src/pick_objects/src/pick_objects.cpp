#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
  ros::init(argc, argv, "pick_objects");

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  //wait for the action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal{};

  //we'll send a goal to the robot to move 1 meter forward
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();

  ros::param::get("/pickup_x", goal.target_pose.pose.position.x);
  ros::param::get("/pickup_y", goal.target_pose.pose.position.y);
  goal.target_pose.pose.orientation.w = 1.57079;
  ROS_INFO("Sending robot to pickup zone ...");
  ac.sendGoal(goal);

  ac.waitForResult();

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
	{
		ROS_INFO("Robot is at pick up zone now, picking up ...");
		sleep(5);
	  ros::param::get("/dropoff_x", goal.target_pose.pose.position.x);
  	ros::param::get("/dropoff_y", goal.target_pose.pose.position.y);
    // goal.target_pose.pose.orientation.w = -1.57079;
		ROS_INFO("Sending robot to dropoff zone ...");
	  ac.sendGoal(goal);
	  ac.waitForResult();
		if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
		{
			ROS_INFO("Robot is at dropoff zone, dropping off ...");
		}
	}

  return 0;
}
