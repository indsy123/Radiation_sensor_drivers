#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Created on Mon Feb 17 09:59:56 2020
first play with RANSAC in python 
@author: ind
"""
import cv2, time
import numpy as np
import hid
vid = 0x09db	# Change it for your device
pid = 0x0076	# Change it for your device

with hid.Device(vid, pid) as h:
	print('Device manufacturer:', h.manufacturer)
	print('Product:', h.product)
	print('Serial Number:', h.serial)

