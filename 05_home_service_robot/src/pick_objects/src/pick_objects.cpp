#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

class RobotMover
{
public:
  RobotMover()
  {
    // wait for the action server to come up
    while (!action_client.waitForServer(ros::Duration(5.0)))
    {
      ROS_INFO("Waiting for the move_base action server to come up");
    }
  }

  bool sendRobotTo(const move_base_msgs::MoveBaseGoal &goal);

private:
  typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
  MoveBaseClient action_client{"move_base", true}; // Spin a thread by default
};

bool RobotMover::sendRobotTo(const move_base_msgs::MoveBaseGoal &goal)
{
  ROS_INFO("Sending robot to zone [x = %f, y = %f]", goal.target_pose.pose.position.x, goal.target_pose.pose.position.y);
  action_client.sendGoal(goal);
  action_client.waitForResult();
  const auto goal_reached = action_client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED;
  return goal_reached;
}

move_base_msgs::MoveBaseGoal getPickupZone()
{
  move_base_msgs::MoveBaseGoal goal{};
  ros::param::get("/pickup_x", goal.target_pose.pose.position.x);
  ros::param::get("/pickup_y", goal.target_pose.pose.position.y);
  goal.target_pose.pose.orientation.w = 1.57079;
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();
  return goal;
}

move_base_msgs::MoveBaseGoal getDropoffZone()
{
  move_base_msgs::MoveBaseGoal goal{};
  ros::param::get("/dropoff_x", goal.target_pose.pose.position.x);
  ros::param::get("/dropoff_y", goal.target_pose.pose.position.y);
  goal.target_pose.pose.orientation.w = 1.57079;
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();
  return goal;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pick_objects");
  RobotMover robot_mover{};
  const auto pickupZone = getPickupZone();
  const auto robot_reached_pickup_zone = robot_mover.sendRobotTo(pickupZone);
  if (robot_reached_pickup_zone)
  {
    ROS_INFO("Robot is picking up");
    sleep(5);
    const auto dropoffZone = getDropoffZone();
    const auto robot_reached_dropoff_zone = robot_mover.sendRobotTo(dropoffZone);
    if (robot_reached_dropoff_zone)
    {
      ROS_INFO("Robot dropped off");
    }
  }

  return 0;
}
