# Robotics software engineer

## Home service
![Home service](media/home_service.gif)

## Initial world
![My world](media/world_home.png)

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
### Add markers
