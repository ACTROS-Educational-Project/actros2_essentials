#ifndef DUMMYOCTOMAP_H_
#define DUMMYOCTOMAP_H_

#include "rclcpp/rclcpp.hpp"
#include <iostream>
#include <octomap/octomap.h>
#include <octomap_msgs/conversions.h>
#include <octomap_msgs/msg/octomap.hpp>

//#include <octomap_msgs/GetOctomap.hpp>
//#include <octomap_msgs/BoundingBoxQuery.hpp>

//#include <octomap_ros/conversions.h>
#include <octomap/OcTree.h>
#include <octomap/OcTreeKey.h>
#include <math.h>

namespace dummy_octomap {

class DummyOctomap : public rclcpp::Node
{
public:
	//typedef pcl::PointXYZ PCLPoint;
	//typedef pcl::PointCloud<pcl::PointXYZ> PCLPointCloud;

	DummyOctomap(const std::string & name);
	void step();

private:
	std::shared_ptr<octomap::OcTree> m_octree_;
};

}

#endif /* DUMMYOCTOMAP_H_ */
