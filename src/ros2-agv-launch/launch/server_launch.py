from launch import LaunchDescription
from ament_index_python.packages import get_package_share_directory
from launch_ros.actions import Node
import os

def generate_launch_description():
    ld = LaunchDescription()

    config = os.path.join(
    get_package_share_directory('ros2-agv-controller'),
    'config',
    'params.yaml')

    controller_node=Node(
        package='ros2-agv-controller',
        #namespace='agv1',
        executable='agv-controller',
        name='controller',
        parameters = [config]
    )

    teleop_node=Node(
        package='teleop_twist_keyboard',
        #namespace='agv1',
        executable='teleop_twist_keyboard',
        name='teleop_twist_keyboard',
        output='screen',
        prefix = 'xterm -e', #launch in a new terminal
        remappings=[('/cmd_vel','/user_vel_cmd')]
    )


  #  ld.add_action(controller_node)

    ld.add_action(teleop_node)

    return ld
