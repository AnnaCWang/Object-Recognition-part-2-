#include <ros/ros.h>
#include <tf/tf.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_listener.h>
#include <actionlib/server/simple_action_server.h>
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseArray.h"
#include "std_msgs/Float32.h"
#include <vector>
#include <iostream>

int yaw_val;

void cb(const std_msgs::Float32::ConstPtr& msg)
{
   yaw_val = msg->data;
   ROS_INFO("I heard: [%d]", yaw_val);
}
 
int main(int argc, char **argv)
{
	
  ros::init(argc, argv, "move_base_client");
  ros::NodeHandle n;

  ros::NodeHandle nhX;
  ros::Subscriber sub = nhX.subscribe("xCoordinate", 100, cb);
  
  ros::Rate loop_rate(10);
  actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base",true); //move_base is action name
  ac.waitForServer(); 
  
  
  move_base_msgs::MoveBaseGoal goal;
  
  double x = 0;
  double yaw = 0;
  double y = 0;
  // need call back function
  while (ros::ok())
  {
    
    //set the header
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.header.frame_id = "/base_link"; //frame of reference, goal is relative to wheels
    
    //set relative x, y, and angle - set the goal
    goal.target_pose.pose.position.x = 0.0;
    goal.target_pose.pose.position.y = 0.0;
    goal.target_pose.pose.position.z = 0.0;
    goal.target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(yaw_val); //create orientation

	//send the goal
    ac.sendGoal(goal);
    ros::spinOnce();
    loop_rate.sleep();
   

  }
  return 0;

}
