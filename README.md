# domino_neutron_sensor
ROS linked drivers for Domino Neutron Sensor

This package contains the ROS drivers for Domino Neutron Sensor described at https://www.rotundascitech.com/products/domino-neutron-detector. 
These sensors can either be used with a Data Acquision (DAQ) device https://www.mccdaq.com/usb-data-acquisition/USB-1024-Series.aspx or with the device that 
is supplied with the Domino. 
If using with the DAQ, the wiring diagram is as shown below: 

Once done, you can start the node by typing: 

rosrun domino domino_node

If using the data acquisition device that comes with the sensor, you can use the python node by typing:

rosrun domino DominoCounts.py

