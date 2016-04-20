#ifndef ACTION_HPP
#define ACTION_HPP

// ROS
#include <ros/ros.h>

// MoveIt!
#include <moveit/move_group_interface/move_group.h>

// Grasp generation
#include <moveit_simple_grasps/simple_grasps.h>
#include <moveit_simple_grasps/grasp_data.h>

//for showing grasps
#include <moveit_visual_tools/moveit_visual_tools.h>

// Forward kinematics to have final pose of trajectory
#include <moveit/robot_state/robot_state.h>
#include <moveit/robot_state/conversions.h>

#include "metablock.hpp"
#include "postures.hpp"

#define FLAG_NO_MOVE        1
#define FLAG_MOVE           2

namespace moveit_simple_actions
{

class Action
{
public:
  Action(ros::NodeHandle *nh_,
         moveit_visual_tools::MoveItVisualToolsPtr &visual_tools,
         const std::string arm_,
         const std::string robot_name);
  bool pickAction(MetaBlock *block, const std::string surface_name,
                  const int attempts_nbr=0,
                  const double planning_time=0.0);
  bool placeAction(MetaBlock *block, const std::string surface_name);
  bool pickDefault(MetaBlock *block);

  //reaching positions generated by moveit simple grasps
  bool graspPlan(MetaBlock *block, const std::string surface_name);
  bool graspPlanAndMove(MetaBlock *block, const std::string surface_name);
  bool graspPlanAllPossible(MetaBlock *block, const std::string surface_name);
  bool executeAction();

  //reaching default grasping pose
  float reachGrasp(MetaBlock *block, const std::string surface_name);
  bool reachPregrasp(geometry_msgs::Pose pose_target, const std::string surface_name);
  bool reachAction(geometry_msgs::Pose pose_target, const std::string surface_name="");
  bool reachInitPose();

  bool poseHand(std::vector<double> *pose_hand);

  //void filterGrasps(MetaBlock *block);

  geometry_msgs::Pose getPose();
  bool poseHandInit();
  bool poseHandZero();
  void poseHandOpen();
  void poseHandClose();
  bool poseHeadDown();
  bool poseHeadZero();

  void setTolerance(const double value);

  void publishPlanInfo(moveit::planning_interface::MoveGroup::Plan plan, geometry_msgs::Pose pose_target);

  void setPlanningTime(const double value);
  void setToleranceStep(const double value);
  void setToleranceMin(const double value);
  void setMaxVelocityScalingFactor(const double value);
  void setVerbose(bool verbose);
  void setAttemptsMax(int value);
  void setFlag(int flag);

  //active end effector
  const std::string arm;
  const std::string end_eff;
  const std::string plan_group;
  Posture posture;
  //GraspFilter grasp_filter_;

  //grasp configuration
  moveit_simple_grasps::GraspData grasp_data_;

private:
  std::vector<moveit_msgs::Grasp> generateGrasps(MetaBlock *block);
  std::vector<geometry_msgs::Pose> configureForPlanning(const std::vector<moveit_msgs::Grasp> &grasps);

  //grasp generator
  moveit_simple_grasps::SimpleGraspsPtr simple_grasps_;

  //interface with MoveIt
  boost::scoped_ptr<move_group_interface::MoveGroup> move_group_;

  //for planning actions
  boost::shared_ptr<moveit::planning_interface::MoveGroup::Plan> current_plan_;

  moveit_visual_tools::MoveItVisualToolsPtr visual_tools_;

  ros::Publisher pub_obj_pose, pub_obj_poses;

  //publish final pose of trajectory
  ros::Publisher pub_plan_pose_;
  ros::Publisher pub_plan_traj_;
  ros::ServiceClient client_fk_;

  bool verbose_;
  int attempts_max_;
  double planning_time_;
  double tolerance_min_, tolerance_step_;
  double max_velocity_scaling_factor_;

  int flag_;

  geometry_msgs::Pose pose_init;

  // class for filter object
  //GraspFilterPtr grasp_filter_;
};



}

#endif // ACTION_HPP
