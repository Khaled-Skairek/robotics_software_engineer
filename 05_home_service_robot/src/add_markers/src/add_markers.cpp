#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <cmath>

struct Position
{
  double x;
  double y;
};

bool operator==(const Position &lhs, const Position &rhs)
{
  constexpr auto tolerence = 0.3;
  return std::abs(lhs.x - rhs.x) < tolerence && std::abs(lhs.y - rhs.y) < tolerence;
}

class PlotHideMarker
{
public:
  PlotHideMarker()
  {
    m_pub = m_n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
    m_sub = m_n.subscribe("/amcl_pose", 10, &PlotHideMarker::newPoseCallBack, this);
    getROSParameters();
    oneSubscriberRequired();
    setMarkerProperties();
    plotMarkerAt(pickupPosition);
  }

  void newPoseCallBack(const geometry_msgs::PoseWithCovarianceStamped &msg)
  {
    Position robot_position{msg.pose.pose.position.x, msg.pose.pose.position.y};
    if (!object_is_picked_up)
    {
      const auto robot_is_at_pickup_position = robot_position == pickupPosition;
      if (robot_is_at_pickup_position)
      {
        ROS_INFO_ONCE("Robot is at pick up position");
        hideMarker();
        object_is_picked_up = true;
      }
    }
    else
    {
      const auto robot_is_at_dropoff_position = robot_position == dropoffPosition;
      if (robot_is_at_dropoff_position)
      {
        ROS_INFO_ONCE("Robot is at drop off position");
        plotMarkerAt(dropoffPosition);
      }
    }
  }

private:
  ros::NodeHandle m_n;
  ros::Publisher m_pub;
  ros::Subscriber m_sub;
  visualization_msgs::Marker marker;
  bool object_is_picked_up{};
  Position pickupPosition{};
  Position dropoffPosition{};

  void setMarkerProperties();
  void plotMarkerAt(const Position &position);
  void hideMarker();
  void oneSubscriberRequired();
  void getROSParameters();
};

void PlotHideMarker::getROSParameters()
{
  ros::param::get("/pickup_x", pickupPosition.x);
  ros::param::get("/pickup_y", pickupPosition.y);
  ros::param::get("/dropoff_x", dropoffPosition.x);
  ros::param::get("/dropoff_y", dropoffPosition.y);
}

void PlotHideMarker::setMarkerProperties()
{
  // Set the frame ID and timestamp.  See the TF tutorials for information on these.
  marker.header.frame_id = "map";
  marker.header.stamp = ros::Time::now();

  // Set the namespace and id for this marker.  This serves to create a unique ID
  // Any marker sent with the same namespace and id will overwrite the old one
  marker.ns = "add_markers";
  marker.id = 0;

  marker.type = visualization_msgs::Marker::CUBE;

  marker.scale.x = 0.4;
  marker.scale.y = 0.4;
  marker.scale.z = 0.6;

  marker.color.r = 0.0F;
  marker.color.g = 0.0F;
  marker.color.b = 1.0F;
  marker.color.a = 1.0;

  marker.lifetime = ros::Duration();

  // Marker const pose properties
  marker.pose.position.z = 0;
  marker.pose.orientation.x = 0.0;
  marker.pose.orientation.y = 0.0;
  marker.pose.orientation.z = 0.0;
  marker.pose.orientation.w = 1.0;
}

void PlotHideMarker::oneSubscriberRequired()
{
  while (m_pub.getNumSubscribers() < 1)
  {
    ROS_WARN_ONCE("Please create a subscriber to the marker");
    sleep(1);
    ROS_INFO("sleep(1);");
  }
}

void PlotHideMarker::plotMarkerAt(const Position &position)
{
  marker.pose.position.x = position.x;
  marker.pose.position.y = position.y;
  marker.action = visualization_msgs::Marker::ADD;
  m_pub.publish(marker);
}

void PlotHideMarker::hideMarker()
{
  marker.action = visualization_msgs::Marker::DELETE;
  m_pub.publish(marker);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "add_markers");

  PlotHideMarker plot_hide_marker{};

  ros::spin();
}
