#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <cmath>

class PlotHideMarkerTimeBased
{
public:
  PlotHideMarkerTimeBased()
  {
    m_pub = m_n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
    getROSParameters();
    oneSubscriberRequired();
    setMarkerProperties();
    plotMarkerAt(pickupPosition);
    sleep(5);
    hideMarker();
    sleep(5);
    plotMarkerAt(dropoffPosition);
  }

private:
  struct Position
  {
    double x;
    double y;
  };
  ros::NodeHandle m_n;
  ros::Publisher m_pub;
  visualization_msgs::Marker marker;
  Position pickupPosition{};
  Position dropoffPosition{};

  void setMarkerProperties();
  void plotMarkerAt(const Position &position);
  void hideMarker();
  void oneSubscriberRequired();
  void getROSParameters();
};

void PlotHideMarkerTimeBased::getROSParameters()
{
  ros::param::get("/pickup_x", pickupPosition.x);
  ros::param::get("/pickup_y", pickupPosition.y);
  ros::param::get("/dropoff_x", dropoffPosition.x);
  ros::param::get("/dropoff_y", dropoffPosition.y);
}

void PlotHideMarkerTimeBased::setMarkerProperties()
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

void PlotHideMarkerTimeBased::oneSubscriberRequired()
{
  while (m_pub.getNumSubscribers() < 1)
  {
    ROS_WARN_ONCE("Please create a subscriber to the marker");
    sleep(1);
  }
}

void PlotHideMarkerTimeBased::plotMarkerAt(const Position &position)
{
  marker.pose.position.x = position.x;
  marker.pose.position.y = position.y;
  marker.action = visualization_msgs::Marker::ADD;
  m_pub.publish(marker);
}

void PlotHideMarkerTimeBased::hideMarker()
{
  marker.action = visualization_msgs::Marker::DELETE;
  m_pub.publish(marker);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "add_markers_time_based");

  PlotHideMarkerTimeBased plot_hide_marker_time_based{};

  ros::spin();
}
