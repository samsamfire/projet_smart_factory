import rclpy
from rclpy.node import Node

from std_msgs.msg import UInt8, UInt16, Float32

import numpy as np
import cv2 as cv
from math import atan2,pi

class Vision(Node):
	def __init__(self):
		super().__init__('vision')

		self.id_publisher_ = self.create_publisher(UInt8, 'id', 10)
		self.orientation_publisher_ = self.create_publisher(UInt16, 'orientation', 10)
		self.x_cord_publisher_ = self.create_publisher(Float32, 'x_cord', 10)
		self.y_cord_publisher_ = self.create_publisher(Float32, 'y_cord', 10)

		timer_period = 0.1  # seconds
		self.timer = self.create_timer(timer_period, self.timer_callback)
		self.i = 0

		self.declare_parameter("camera_id",2)
		camera_id = self.get_parameter("camera_id").value

		self.camera = cv.VideoCapture(camera_id)

	def timer_callback(self):
		ret, frame = self.camera.read()
		img,marker_id,orientation_angle,center = process_img(frame)
		cv.imshow('Camera Window',img)
		cv.waitKey(1)

		if marker_id != -1 :
			id_msg = UInt8()
			id_msg.data = int(marker_id)
			self.id_publisher_.publish(id_msg)

			orientation_msg = UInt16()
			orientation_msg.data = int(orientation_angle)
			self.orientation_publisher_.publish(orientation_msg)

			x_cord_msg = Float32()
			x_cord_msg.data = float(center[0])
			self.x_cord_publisher_.publish(x_cord_msg)

			y_cord_msg = Float32()
			y_cord_msg.data = float(center[1])
			self.y_cord_publisher_.publish(y_cord_msg)

			self.get_logger().info('Publishing: id: '+str(id_msg.data)+
				'	orientation: '+str(orientation_msg.data)+
				'	X position: '+str(x_cord_msg.data)+
				'	Y position: '+str(y_cord_msg.data))
		self.i += 1

def process_img(src):
	ddepth = cv.CV_16S
	max_a = 30
	threshold = 100
	l,h,c = src.shape
	reduction_ratio = 2


	#Applying Hough Transform to find circles
	gray = cv.cvtColor(src, cv.COLOR_BGR2GRAY)
	gray = cv.medianBlur(gray, 5)
	
	
	rows = gray.shape[0]
	circles = cv.HoughCircles(gray, cv.HOUGH_GRADIENT, 1, 30,
							   param1=100, param2=40,
							   minRadius=20, maxRadius=50)
	
	
	if circles is not None:
		circles = np.uint16(np.around(circles))
		max_radius = 0
		for i in circles[0, :]:
			radius = i[2]
			if radius > max_radius:
				max_radius = radius
				center = (i[0], i[1])

		#Threshold square around marker
		max_radius *=3
		x_min,x_max = center[1]-max_radius,center[1]+max_radius
		y_min,y_max = center[0]-max_radius,center[0]+max_radius
		ret,src[x_min:x_max,y_min:y_max] = cv.threshold(src[x_min:x_max,y_min:y_max],threshold,255,cv.THRESH_BINARY)

		square = src[x_min:x_max,y_min:y_max]
		if all(square.shape):
			square = cv.resize(square,(square.shape[0]//reduction_ratio,
				square.shape[1]//reduction_ratio))
		center_in_square = max_radius//reduction_ratio,max_radius//reduction_ratio

		H = np.zeros(360)
		for y in range(len(square)):
			for x in range(len(square[0])):
				if not square[y][x].any(): #black square
					x_coord = x - center_in_square[0]
					y_coord = y - center_in_square[1]

					angle = round(atan2(y_coord,x_coord)/pi*180)
					if angle < 0:
						angle  = 360 + angle
					H[angle] += 1

		max_H = np.amax(H)
		orientation_angle = np.argmax(H)

		Z = 0
		for angle in range(45,359,45):
			Z+=H[(angle+orientation_angle)%360]
		Z/=6

		marker_id = 0
		margin = 5
		for i in range(3):
			if any([H[(orientation_angle+m+(i+1)*90)%360] > Z for m in range(-margin,margin)]):
				marker_id += 2**i

		# plt.plot(np.arange(360),H)
		# plt.plot([0,360],[Z,Z])
		# plt.plot([orientation_angle,orientation_angle],[0,max_H])
		# plt.plot([(orientation_angle+90)%359,(orientation_angle+90)%359],[0,max_H])
		# plt.plot([(orientation_angle+180)%359,(orientation_angle+180)%359],[0,max_H])
		# plt.plot([(orientation_angle+270)%359,(orientation_angle+270)%359],[0,max_H])

		cv.circle(src, center, 1, (255, 0, 0), 3)

		# print("id : ",marker_id)
		# print("angle : ",orientation_angle)
		# print("position : ",center)

		return(src,marker_id,orientation_angle,center)
	else:
		return(src,-1,0,(0,0))

def main(args=None):
	rclpy.init(args=args)
	print('Beginning of marker detection')

	cv.namedWindow("Camera Window")

	vision = Vision()
	rclpy.spin(vision)

	camera.release()
	cv.destroyAllWindows()	


if __name__ == '__main__':
	main()
