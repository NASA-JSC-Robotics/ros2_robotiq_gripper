
#include <chrono>
#include <cmath>
#include <limits>
#include <memory>
#include <vector>

#include "hardware_interface/actuator_interface.hpp"
#include "hardware_interface/types/hardware_interface_type_values.hpp"
#include "rclcpp/rclcpp.hpp"

#include "robotiq_driver/hardware_interface.hpp"
#include "robotiq_driver/hande_hardware_interface.hpp"

constexpr double kGripperMinPos = 0.0;
constexpr double kGripperMaxPos = 255.0;
constexpr double kGripperMaxSpeed = 0.15;  // m/s
constexpr double kGripperMaxForce = 185;    // N

const auto kLogger = rclcpp::get_logger("RobotiqGripperHandeHardwareInterface");

namespace robotiq_driver_hande
{
RobotiqGripperHandeHardwareInterface::RobotiqGripperHandeHardwareInterface()
{
}

hardware_interface::return_type RobotiqGripperHandeHardwareInterface::read(const rclcpp::Time& /*time*/,
                                                                           const rclcpp::Duration& /*period*/)
{
  gripper_position_ = gripper_closed_pos_ * (1.0 - ((gripper_current_state_.load() - kGripperMinPos) / kGripperMaxPos)) / 2.0;

  if (!std::isnan(reactivate_gripper_cmd_))
  {
    RCLCPP_INFO(kLogger, "Sending gripper reactivation request.");
    reactivate_gripper_async_cmd_.store(true);
    reactivate_gripper_cmd_ = NO_NEW_CMD_;
  }

  if (reactivate_gripper_async_response_.load().has_value())
  {
    reactivate_gripper_response_ = reactivate_gripper_async_response_.load().value();
    reactivate_gripper_async_response_.store(std::nullopt);
  }

  return hardware_interface::return_type::OK;
}

hardware_interface::return_type RobotiqGripperHandeHardwareInterface::write(const rclcpp::Time& /*time*/,
                                                                            const rclcpp::Duration& /*period*/)
{
  double gripper_pos = (1.0 - ((2 * gripper_position_command_) / gripper_closed_pos_)) * kGripperMaxPos + kGripperMinPos;

  gripper_pos = std::max(std::min(gripper_pos, kGripperMaxPos), kGripperMinPos);
  write_command_.store(uint8_t(gripper_pos));

  return hardware_interface::return_type::OK;
}

}  // namespace robotiq_driver_hande

#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(robotiq_driver_hande::RobotiqGripperHandeHardwareInterface, hardware_interface::SystemInterface)
