# Robotics software engineer

## Home service
![Home service](media/home_service.gif)

## Project packages
### Mapping
The map where the robot operates is built using slam technique. The implementation was used from <br/>
https://github.com/ros-perception/slam_gmapping
### Localization
Once the map is available, the robot needs to localize itself inside that map. For that purpose we used the amcl algorithm available as a standard ros package.
### Pick Objects
Send goal locations to the robot. Once for the pickup location, and once for the dropoff location.
When the robot reaches the pickup location, it pauses for 5 sec to simulate picking the virtual object up.
### Navigation
The navigation stack receives the request goal from pick objects and plan path from the robot location to the goal using Dijkstra algorithm.<br/>
If an obstacle (like a human) is found on the way to the goal, a local replanning takes place to avoid the obstacle.
### Add marker
This package takes care of visualizing the objects on rviz.<br/>
First it plots the virtual objet at the pick up location. Once the robot reaches the pick up location, it hides the object to indicate the robot has picked it up.<br/>
When the robot reach the drop off location, add marker plots the object at the drop off location.

## Initial world
![My world](media/world_home.png)
