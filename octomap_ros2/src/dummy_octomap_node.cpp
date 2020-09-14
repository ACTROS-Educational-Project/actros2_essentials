#include "octomap_ros2/dummy_octomap_node.hpp"

using namespace std::chrono_literals;

//using octomap_msgs::Octomap;

namespace dummy_octomap {

DummyOctomap::DummyOctomap(const std::string & name): Node(name)
{
	double probHit, probMiss, thresMin, thresMax;
	double voxel_res = 0.1;
  probHit = 0.7;
  probMiss = 0.4;
  thresMin = 0.12;
  thresMax = 0.97;

	m_octree_ = std::make_shared<octomap::OcTree>(voxel_res);
  //m_octree_->setProbHit(probHit);
  //m_octree_->setProbMiss(probMiss);
  //m_octree_->setClampingThresMin(thresMin);
  //m_octree_->setClampingThresMax(thresMax);
}

void DummyOctomap::step() {
  RCLCPP_ERROR(get_logger(), "octree OccupancyThres [%f]", m_octree_->getOccupancyThres());
}

}

int main(int argc, char * argv[]) {
	rclcpp::init(argc, argv);
  auto dummy_octomap = std::make_shared<dummy_octomap::DummyOctomap>("dummy_octomap_node");
  rclcpp::Rate loop_rate(1000ms); 
  while (rclcpp::ok()) {
    dummy_octomap->step();
    rclcpp::spin_some(dummy_octomap);
    loop_rate.sleep();
  }
  rclcpp::shutdown();

  return 0;
}