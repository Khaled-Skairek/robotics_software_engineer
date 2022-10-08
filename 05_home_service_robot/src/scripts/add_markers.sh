#!/bin/sh

xterm -e " source /home/workspace/05_home_service_robot/devel/setup.bash; roslaunch turtlebot_gazebo turtlebot_world.launch " &
sleep 15
xterm -e " source /home/workspace/05_home_service_robot/devel/setup.bash; roslaunch turtlebot_gazebo amcl_demo.launch " &
sleep 10
xterm -e " source /home/workspace/05_home_service_robot/devel/setup.bash; roslaunch turtlebot_rviz_launchers view_navigation.launch " &
sleep 10
xterm -e " source /home/workspace/05_home_service_robot/devel/setup.bash; roslaunch add_markers add_markers.launch "

