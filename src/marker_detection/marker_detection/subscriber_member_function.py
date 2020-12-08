import rclpy
from rclpy.node import Node

from std_msgs.msg import UInt8, UInt16, Float32

class MinimalSubscriber(Node):

    def __init__(self):
        super().__init__('minimal_subscriber')
        self.id_subscription = self.create_subscription(
            UInt8,
            'id',
            self.id_callback,
            10)
        self.orientation_subscription = self.create_subscription(
            UInt16,
            'orientation',
            self.orientation_callback,
            10)
        self.x_cord_subscription = self.create_subscription(
            Float32,
            'x_cord',
            self.x_cord_callback,
            10)
        self.y_cord_subscription = self.create_subscription(
            Float32,
            'y_cord',
            self.y_cord_callback,
            10)
        #self.subscription  # prevent unused variable warning

    def id_callback(self, msg):
        self.get_logger().info("Id: "+str(msg.data))
    def orientation_callback(self, msg):
        self.get_logger().info("Orientation: "+str(msg.data))
    def x_cord_callback(self, msg):
        self.get_logger().info("X position: "+str(msg.data))
    def y_cord_callback(self, msg):
        self.get_logger().info("Y position: "+str(msg.data))


def main(args=None):
    rclpy.init(args=args)

    minimal_subscriber = MinimalSubscriber()

    rclpy.spin(minimal_subscriber)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    minimal_subscriber.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
