from launch import LaunchDescription
from launch_ros.actions import Node

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

    wrapper_node=Node(
        package='ros2-agv-wrapper',
        #namespace='agv1',
        executable='agv-wrapper',
        name='wrapper'
    )

    ld.add_action(sensors_node)
    ld.add_action(wrapper_node)
    ld.add_action(controller_node)

    return ld
