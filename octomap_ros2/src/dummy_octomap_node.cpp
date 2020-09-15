// Copyright 2020 Intelligent Robotics Lab
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "octomap_ros2/dummy_octomap_node.hpp"

using namespace std::chrono_literals;

namespace dummy_octomap {

DummyOctomap::DummyOctomap(const std::string & name): Node(name)
{
  double probHit, probMiss, thresMin, thresMax;
  double voxel_res = 0.1;
  probHit = 0.7;
  probMiss = 0.4;
  thresMin = 0.12;
  thresMax = 0.97;

  octree_ = std::make_shared<octomap::OcTree>(voxel_res);
  octree_->setProbHit(probHit);
  octree_->setProbMiss(probMiss);
  octree_->setClampingThresMin(thresMin);
  octree_->setClampingThresMax(thresMax);

  pub_ = create_publisher<octomap_msgs::msg::Octomap>("/dummy_octomap", 1);

  initOctomap();
}

void DummyOctomap::initOctomap() {
  octomap::KeySet cells;
  octomap::KeyRay keyRay;
  octomap::point3d origin(0.0, 0.0, 0.0);
  octomap::point3d target(3.0, 1.0, 0.1);

  // We take some voxels
  if (octree_->computeRayKeys(origin, target, keyRay)) {
	  cells.insert(keyRay.begin(), keyRay.end());
  }

  // We insert these voxels in the octree
  for(auto cell : cells){
    octree_->updateNode(cell, false);
	  octree_->setNodeValue(cell, 1.0, true);
  }
}

void DummyOctomap::publishFullOctoMap() {
  octomap_msgs::msg::Octomap map;
  map.header.frame_id = "map";
  //map.header.stamp = rostime;
  size_t octomapSize = octree_->size();
  if (octomapSize <= 1){
	  RCLCPP_WARN(get_logger(),"Nothing to publish, octree is empty");
	  return;
  }
  if (octomap_msgs::fullMapToMsg(*octree_, map)){
    pub_->publish(map);
    RCLCPP_INFO(get_logger(), "publishing a octomap of size [%u]", octomapSize);
  }else{
	  RCLCPP_ERROR(get_logger(),"Error serializing OctoMap");
  }
}

void DummyOctomap::step() {
  publishFullOctoMap();
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