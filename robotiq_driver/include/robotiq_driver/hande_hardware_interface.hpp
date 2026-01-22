
#pragma once

#include "robotiq_driver/hardware_interface.hpp"
#include "hardware_interface/system_interface.hpp"

namespace robotiq_driver_hande
{
class RobotiqGripperHandeHardwareInterface : public robotiq_driver::RobotiqGripperHardwareInterface
{
public:
  RCLCPP_SHARED_PTR_DEFINITIONS(RobotiqGripperHandeHardwareInterface)

  ROBOTIQ_DRIVER_PUBLIC
  RobotiqGripperHandeHardwareInterface();

  ROBOTIQ_DRIVER_PUBLIC
  hardware_interface::return_type read(const rclcpp::Time& time, const rclcpp::Duration& period) override;

  ROBOTIQ_DRIVER_PUBLIC
  hardware_interface::return_type write(const rclcpp::Time& time, const rclcpp::Duration& period) override;

  // protected:
  //   std::unique_ptr<RobotiqGripperInterface> gripper_interface_;
};

}  // namespace robotiq_driver_hande
