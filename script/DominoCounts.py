#!/usr/bin/env python
# Dedicated to Adam who helped (actually made) the Domino Driver Board working, 10/24/2017
import rospy
#from std_msgs.msg import Time
#from std_msgs.msg import Int32
from geometry_msgs.msg import PointStamped
import serial
import time 

def DominoCounts(N)
    filename = '/home/pelican/data_collection/TRO/detection_data'+str(N)+'.txt'
    open(filename, 'w').close()
    pub = rospy.Publisher('/udrone1/neutron_counts', PointStamped, queue_size = 1)
    rospy.init_node('Domino_005', anonymous=False, log_level=rospy.DEBUG)
    
    ser = serial.Serial('/dev/ttyACM1',115200,timeout=1)
    if (not ser.isOpen()):
        ser.open()
    number = 0
    starttime = time. time()
    while not rospy.is_shutdown():
	#ser.write(b'\x08') #stop counting
	ser.write(b'\x07') #clear counts
	ser.write(b'\x06') # start counting
	ser.write(b'\x05') # get counts
	buff = 0
	string = ""
	#xx = ser.read(4)
        #print 'xx:', xx
 	while buff < 4: 
	    byte = ser.read(1)
	    string = string + str('%x' % ord(byte))
	    buff = buff+1

	print 'total counts:', number, 'time:', rospy.Time.now()
	if int(string) > 0:
	    msg = PointStamped()
            msg.header.stamp = rospy.Time.now()
            currenttime = time.time() 
	    number += 1
            msg.point.x = int(number) 
            msg.point.y = 0 
            msg.point.y = 0   
	    #now = rospy.get_rostime()
	    pub.publish(msg)
	    print 'total counts:', number, 'time:', msg.header.stamp
    	    f = open(filename, 'a')
    	    f.write("%2.3f, %2.3f\n" % (currenttime, number))

        #print 'count is ', number, time.time()-starttime
    if rospy.is_shutdown():
	print "\n"
	print "Shutting down Domino"
	ser.close()


if __name__ == '__main__':
    try:
        N = rospy.get_param('run_number')
        DominoCounts(N)    
    except rospy.ROSInterruptException:
	pass


