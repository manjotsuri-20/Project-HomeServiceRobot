#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

// Define a client for to send goal requests to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
  // Initialize the simple_navigation_goals node
  ros::init(argc, argv, "pick_objects");
  ros::NodeHandle nh;

  nh.setParam("pickobjectsrunning", true); 
  nh.setParam("reached_pickup_zone", false);
  nh.setParam("reached_drop_zone", false);
  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  // Wait 5 sec for move_base action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }
  
  
  move_base_msgs::MoveBaseGoal goal;

  // set up the frame parameters
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();

  // Define a position and orientation for the robot to reach
  goal.target_pose.pose.position.x = 4.0;
  goal.target_pose.pose.position.y = 2.0;
  goal.target_pose.pose.orientation.z = 0.0;
  goal.target_pose.pose.orientation.w = 1.0;

   // Send the goal position and orientation for the robot to reach
  ROS_INFO("Sending goal for pickup zone");
  ac.sendGoal(goal);

  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached its goal
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("HI, I reached my pickup zone");
  else
    ROS_INFO("SORRY, I couldn't reach the pickup zone due to some reasons");

  nh.setParam("reached_pickup_zone", true);
  
  // Delaying the code for 5 seconds
  ROS_INFO("Pausing for 5 seconds");
  ros::Duration(5.0).sleep();

  // Define a position and orientation for the robot to reach
  goal.target_pose.pose.position.x = -4.0;
  goal.target_pose.pose.position.y = 3.0;
  goal.target_pose.pose.orientation.z = 0.0;
  goal.target_pose.pose.orientation.w = 1.0;

   // Send the goal position and orientation for the robot to reach
  ROS_INFO("Sending goal for drop zone");
  ac.sendGoal(goal);

  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached its goal
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Hooray, I reached the drop zone !!");
  else
    ROS_INFO("SORRY, I couldn't reach the drop zone. Can I help u other way ? :-) ");
 
  nh.setParam("reached_drop_zone", true);
  
  ros::Duration(2.0).sleep();
  
  return 0;
}
