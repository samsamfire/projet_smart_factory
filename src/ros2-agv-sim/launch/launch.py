import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import ThisLaunchFileDir
from launch.actions import ExecuteProcess


def generate_launch_description():

    gazebo = IncludeLaunchDescription(
                PythonLaunchDescriptionSource(['/opt/ros/foxy/share/gazebo_ros/launch', '/gazebo.launch.py']),
             )

    urdf_file_name = 'model.urdf'
    urdf = os.path.join(get_package_share_directory('ros2-agv-sim'),
      'models/'+urdf_file_name)
        
    # node=Node(
    #     package = 'gazebo_ros',
    #     name = 'urdf_spawner',
    #     type= 'spawn_model',
    #     executable = 'agv-sensors',
    # )

     # GAZEBO_MODEL_PATH has to be correctly set for Gazebo to be able to find the model
    # spawn_entity = Node(package='gazebo_ros', node_executable='spawn_entity.py',
    #                     arguments=['-entity', 'demo', '-database', 'double_pendulum_with_base'],
    #                     output='screen')

    # ld.add_action(node)

    #spawn_entity = Node(package='gazebo_ros',node_executable='spawn_entity.py', arguments=['-entity','agv','-file',urdf])

    ld = LaunchDescription([
        ExecuteProcess(
            cmd=['gazebo', '--verbose', '-s', 'libgazebo_ros_init.so'],
            output='screen'),

        ExecuteProcess(
            cmd=['ros2', 'param', 'set', '/gazebo'],
            output='screen')])

