# Robotics software engineer

## Home service
![Home service](media/home_service.gif)

## Project packages
### Mapping
The robot uses odometry information and sensor range observations to learn the map of the environment using efficient Rao-Blackwellized particle filer<br/>
The algorithm carries out four main steps
1. Generate a new set of particles using odometry motion model and previous old set of particles
2. Calculate importance weight of each particle using sensor observations
3. Resample particles in accordance with importance weights.
4. Map estimation: calculate the corresponding map for each particle.  

The result map is then the most propable particle (highest importance weight).<br/><br/>
<i>Source:</i> <br/>
https://github.com/ros-perception/slam_gmapping
### Localization
Once the map is available from previous step, the robot needs to localize itself inside that map. For that purpose we used the AMCL algorithm available as a standard ros package.<br/>
AMCL algorithm uses particle filter to localize the robot inside the map. Each particle represents one possible pose.<br\>
Odometry motion model is used to generate a new set of particles. When sensor observations are available (laser scan in our case), importance weight is calculated for each particle, followed by resampling which keeps the most probable particles.<br/>
Finally the node publishes the amcl_pose topic which corresponds to the most probable particle (pose)
### Pick Objects
Send goal locations to the robot. Once for the pickup location, and once for the dropoff location.
When the robot reaches the pickup location, it pauses for 5 sec to simulate picking the virtual object up.
### Navigation
The navigation stack receives the request goal from pick objects and plan path from the robot location to the goal using Dijkstra algorithm.<br/>
If an obstacle (like a human) is found on the way to the goal, a local replanning takes place to avoid the obstacle.
### Add marker
This package takes care of visualizing the objects in rviz.<br/>
Initially, the node plots the virtual object at the pick up location.<br/>
The package subscribe to the topic amcl_pose to get the robot pose. It constantly compares the robot pose againt the robot pick up location.<br/>
Once the robot reaches the pick up location, the node hides the virtual object at the pick up location to simulate the robot has picked up the object.<br/>
When the robot reach the drop off location, add marker node plots the object at the drop off location.<br/>
The package has also another node which plot marker based on elapsed time<br/>
first it plots the object at pick up location<br/>
hide it after 5 seconds. After another 5 seconds the object reappears at the drop off location.


## Initial world
![My world](media/world_home.png)
