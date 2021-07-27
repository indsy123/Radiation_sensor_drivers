#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Created on Mon Feb 17 09:59:56 2020
first play with RANSAC in python 
@author: ind
"""
import time
import numpy as np
import rospy
import serial 
from geometry_msgs.msg import PointStamped


class dominocounts(object):
    def __init__(self, N): 
        self.run_number = N
        """initializes various paramters"""
        self.logpath = '/home/pelican/data_collection/TRO/'
        open(self.logpath+'detection_data'+str(self.run_number)+'.txt', 'w').close()
	self.pub = rospy.Publisher('/udrone1/neutron_counts', PointStamped, queue_size = 1)

        self.ser = serial.Serial('/dev/ttyACM0',115200,timeout=1)
        if (not self.ser.isOpen()):
            self.ser.open()
        self.number = 0
        self.starttime = time. time()

    def getcounts(self): 
        while not rospy.is_shutdown():
	
	    self.ser.write(b'\x07') #clear counts
	    self.ser.write(b'\x06') # start counting
	    self.ser.write(b'\x05') # get counts
	    buff = 0
	    string = ""

 	    while buff < 4: 
	        byte = self.ser.read(1)
	        string = string + str('%x' % ord(byte))
	        buff = buff+1

	    print 'total counts:', self.number, 'time:', rospy.Time.now()
            print string
	    if int(string) > 0:
	        msg = PointStamped()
                msg.header.stamp = rospy.Time.now()
                currenttime = time.time() 
	        self.number += 1
                msg.point.x = int(self.number) 
                msg.point.y = 0 
                msg.point.y = 0   

	        self.pub.publish(msg)
	        print 'total counts:', self.number, 'time:', msg.header.stamp
    	        f = open(self.logpath+'detection_data'+str(self.run_number)+'.txt', 'a')
    	        f.write("%2.3f, %2.3f\n" % (currenttime, self.number))

        if rospy.is_shutdown():
	    print "\n"
	    print "Shutting down Domino"
	    self.ser.close()        
        



        

if __name__ == '__main__':
    rospy.init_node('dominocounts', anonymous=False, log_level=rospy.DEBUG)
    N = rospy.get_param('run_number')
    calc = dominocounts(N)
    calc.getcounts() 


