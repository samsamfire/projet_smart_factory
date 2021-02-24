from launch import LaunchDescription
from ament_index_python.packages import get_package_share_directory
from launch_ros.actions import Node
import os

def generate_launch_description():
    ld = LaunchDescription()

    config = os.path.join(
        get_package_share_directory('ros2-agv-sensors'),
        'config',
        'params.yaml'
        )

    sensors_node=Node(
        package = 'ros2-agv-sensors',
        #namespace='agv1',
        executable = 'agv-sensors',
        name = 'sensors',
        parameters = [config]
    )

    marvelmind_node=Node(
        package = 'ros2-agv-sensors',
        #namespace='agv1',
        executable = 'agv-marvelmind',
        name = 'marvelmind',
    )


    wrapper_node=Node(
        package='ros2-agv-wrapper',
        #namespace='agv1',
        executable='agv-wrapper',
        name='wrapper'
    )

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



    ld.add_action(sensors_node)
    #ld.add_action(marvelmind_node)
    ld.add_action(wrapper_node)
    ld.add_action(controller_node)

    return ld
