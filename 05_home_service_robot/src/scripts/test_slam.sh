#!/bin/sh

xterm -e " source /home/workspace/05_home_service_robot/devel/setup.bash; roslaunch add_markers turtlebot_world.launch " &
sleep 15
xterm -e " source /home/workspace/05_home_service_robot/devel/setup.bash; roslaunch add_markers gmapping_demo.launch " &
sleep 7
xterm -e " source /home/workspace/05_home_service_robot/devel/setup.bash; roslaunch add_markers view_navigation.launch " &
sleep 7
xterm -e " source /home/workspace/05_home_service_robot/devel/setup.bash; roslaunch add_markers keyboard_teleop.launch "
