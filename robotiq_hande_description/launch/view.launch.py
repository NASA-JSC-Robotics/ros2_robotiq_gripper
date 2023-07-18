from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import Command, FindExecutable, LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():
    declared_arguments = []

    declared_arguments.append(
        DeclareLaunchArgument(
            "use_fake_hardware",
            description="Run with ros2 control fake hardware interface",
            default_value="false"
        )
    )
    declared_arguments.append(
        DeclareLaunchArgument(
            "tf_prefix",
            default_value="''",
            description="tf_prefix of the joint names, useful for \
                multi-robot setup. If changed than also joint names in the controllers' configuration \
                have to be updated.",
        )
    )
    declared_arguments.append(
        DeclareLaunchArgument(
            "name",
            default_value='robotiq_gripper_hande',
            description="Name of the robot. Gets used by ros2 control",
        )
    )

    # Initialize arguments
    use_fake_hardware = LaunchConfiguration("use_fake_hardware")
    tf_prefix = LaunchConfiguration("tf_prefix")
    name = LaunchConfiguration("name")

    robot_description_content = Command(
        [
            PathJoinSubstitution([FindExecutable(name="xacro")]),
            " ",
            PathJoinSubstitution([FindPackageShare("robotiq_hande_description"), "urdf", "robotiq_gripper_hande.urdf.xacro"]),
            " ", "use_fake_hardware:=", use_fake_hardware,
            " ", "tf_prefix:=", tf_prefix,
            " ", "name:=", name,
        ]
    )

    robot_description = {"robot_description": robot_description_content}

    rviz_config_file = PathJoinSubstitution(
        [FindPackageShare("robotiq_hande_description"), "rviz", "visualization.rviz"]
    )

    joint_state_publisher_node = Node(
        package="joint_state_publisher_gui",
        executable="joint_state_publisher_gui",
    )
    robot_state_publisher_node = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        output="both",
        parameters=[robot_description]
    )
    rviz_node = Node(
        package="rviz2",
        executable="rviz2",
        name="rviz2",
        output="log",
        arguments=["-d", rviz_config_file],
    )

    nodes_to_start = [
        joint_state_publisher_node,
        robot_state_publisher_node,
        rviz_node,
    ]

    return LaunchDescription(declared_arguments + nodes_to_start)
