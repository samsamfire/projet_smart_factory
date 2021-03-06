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



    ld.add_action(controller_node)


    return ld
