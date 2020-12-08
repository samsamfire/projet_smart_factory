from setuptools import setup

package_name = 'marker_detection'

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='mathieu',
    maintainer_email='mathieu@todo.todo',
    description='Vision Package to help localize the AGV outside of rooms',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'vision = marker_detection.vision:main',
            'listener = marker_detection.subscriber_member_function:main'
        ],
    },
)
