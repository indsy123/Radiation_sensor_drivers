/*
 *
 *  Copyright (c) 2015 Warren J. Jasper <wjasper@tx.ncsu.edu>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
/*#include <string.h>*/
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
/*Added by Indrajeet on 07/13/2017*/
#include "ros/ros.h"
#include "std_msgs/Int8.h"
#include "geometry_msgs/PointStamped.h"
#include<sstream>
/*Added by Indrajeet on 07/13/2017*/
#include "pmd.h"
#include "usb-1024LS.h"


std::ofstream outdata;
using namespace std;



#define MAX_STR 255

/* Test Program */
int toContinue() 
{
  int answer;
  answer = 0; //answer = getchar();
  printf("Continue [yY]? ");
  while((answer = getchar()) == '\0' ||
	answer == '\n');
  return ( answer == 'y' || answer == 'Y');
}

int main (int argc, char **argv)
{
  int flag;
  uint8_t input, pin = 0; 
  int temp;
  int ch;
  hid_device* hid = 0x0;
  int ret;
  wchar_t serial[64];
  wchar_t wstr[MAX_STR];
   

  ret = hid_init();
  if (ret < 0) {
    fprintf(stderr, "hid_init failed with return code %d\n", ret);
    return -1;
  }

  if ((hid = hid_open(MCC_VID, USB1024LS_PID, NULL)) > 0) {
    printf("USB 1024LS Device is found!\n");
  } else if ((hid = hid_open(MCC_VID, USB1024HLS_PID, NULL)) > 0) {
    printf("USB 1024HLS Device is found!\n");
  } else {
    cout <<hid;
    fprintf(stderr, "USB 1024LS and USB 1024HLS not found.\n");
    exit(1);
  }

  usbDConfigPort_USB1024LS(hid, DIO_PORTA, DIO_DIR_OUT);
  usbDConfigPort_USB1024LS(hid, DIO_PORTB, DIO_DIR_IN);
  usbDConfigPort_USB1024LS(hid, DIO_PORTC_LOW, DIO_DIR_OUT);
  usbDConfigPort_USB1024LS(hid, DIO_PORTC_HI, DIO_DIR_IN);

  /*Added by Indrajeet on 07/13/2017, to start a ros publisher node*/
  ros::init(argc, argv, "Domino_node");
  ros::NodeHandle n;
  ros::Publisher Domino_data = n.advertise<geometry_msgs::PointStamped>("/udrone1/neutron_counts",1);
  int no;  
  ros::param::get("run_number", no);
  string str = to_string(no);
 
  outdata.open("generated_counts"+str+".txt", std::ios::out|std::ios::binary);
  //ros::spin();
  // just read the time and x field which is 1 when there is a count otherwise 0
  /*Added by Indrajeet on 07/13/2017,to start a ros publisher node*/  
  while(ros::ok()) {
    /*printf("\nUSB 1024 Testing\n");
    printf("----------------\n");
    printf("Hit 'b' to blink LED\n");
    printf("Hit 's' to set user id\n");
    printf("Hit 'g' to get user id\n");
    printf("Hit 'i' for information\n");
    printf("Hit 'n' to get serial number\n");
    printf("Hit 'c' to test counter \n");
    printf("Hit 'd' to test digital I/O \n");
    printf("Hit 'r' to reset the device.\n");
    printf("Hit 't' to test digital bit I/O\n");
    printf("Hit 'e' to exi
n");

    while((ch = getchar()) == '\0' || ch == '\n');*/

    ch = 'c';
    
    switch(tolower(ch)) {
    case 'b': /* test to see if led blinks */
      usbBlink_USB1024LS(hid);
      break;
    case 'i':
      // Read the Manufacuter String
      ret = hid_get_manufacturer_string(hid, wstr, MAX_STR);
      printf("Manufacturer String: %ls\n", wstr);
      // Read the Product String
      ret = hid_get_product_string(hid, wstr, MAX_STR);
      printf("Product String: %ls\n", wstr);
      // Read the Serial Number String
      ret = hid_get_serial_number_string(hid, wstr, MAX_STR);
      printf("Serial Number String: %ls\n", wstr);
      break;            
    case 's':
      printf("enter a user id :");
      scanf("%d",&temp);
      usbSetID_USB1024LS(hid, temp);
      printf("User ID is set to %d\n", usbGetID_USB1024LS(hid));      
      break;
    case 'g':
      printf("User ID = %d\n", usbGetID_USB1024LS(hid));      
      break;
    case 'c':
      printf("connect pin 21 and 20\n");
      usbInitCounter_USB1024LS(hid);
      flag = fcntl(fileno(stdin), F_GETFL);
      fcntl(0, F_SETFL, flag | O_NONBLOCK);
      do {
        usbDOut_USB1024LS(hid, DIO_PORTA, 1);
        usbDOut_USB1024LS(hid, DIO_PORTA, 0);
	//printf("Counter = %d\n",usbReadCounter_USB1024LS(hid));
        /*Added by Indrajeet on 07/13/2017 to publish count on a ros node*/	

        /*ros::Rate loop_rate(10);*/
        int count = 0;

          geometry_msgs::PointStamped msg;
	  msg.header.stamp = ros::Time::now();
          int counts = usbReadCounter_USB1024LS(hid);
	  ROS_INFO("%d", counts);
	  msg.point.x = counts;
	  Domino_data.publish(msg);
	  ros::Time current_time = ros::Time::now();
          outdata << current_time <<","<<counts<<endl;
	  
 	  /*loop_rate.sleep();*/
	  ++count;
        
        /*Added by Indrajeet on 07/13/2017 to publish count on a ros node*/
        usleep(200000);
      } while (!isalpha(getchar()));
      fcntl(fileno(stdin), F_SETFL, flag);
      break;
    case 'd':
      printf("\nTesting Digital I/O....\n");
      printf("connect pins 21 through 28 <=> 32 through 39 and pins 1-4 <==> 5-8\n");
      do {
        printf("Enter a byte number [0-0xff] : " );
        scanf("%x", &temp);
        usbDOut_USB1024LS(hid, DIO_PORTA,(uint8_t)temp);
        usbDIn_USB1024LS(hid, DIO_PORTB, &input);
        printf("The number you entered = %#x\n\n",input);
        printf("Enter a nibble [0-0xf] : " );
        scanf("%x", &temp);
        usbDOut_USB1024LS(hid, DIO_PORTC_LOW,(uint8_t)temp);
        usbDIn_USB1024LS(hid, DIO_PORTC_HI, &input);
        printf("The number you entered = %#x\n", input);
      } while (toContinue());
      break;
    case 't':
      //reset the pin values
      usbDOut_USB1024LS(hid, DIO_PORTA,0x0);
      printf("\nTesting Bit  I/O....\n");
      printf("Enter a bit value for output (0 | 1) : ");
      scanf("%d", &temp);
      input = (uint8_t) temp;
      printf("Select the Pin in port A [0-7] :");
      scanf("%d", &temp);
      pin = (uint8_t) temp;
      usbDBitOut_USB1024LS(hid, DIO_PORTA, pin, input);
      usbDIn_USB1024LS(hid, DIO_PORTB, &input);
      printf("The number you entered 2^%d = %d \n", temp, input);
      break;
    case 'e':
      hid_close(hid);
      hid_exit();
      return 0;
      break;
    case 'r':
      usbReset_USB1024LS(hid);
      exit(0);
      break;
    case 'n':
      hid_get_serial_number_string(hid, serial, 64);
      printf("Serial Number = %ls\n", serial);
      break;
    default:
      break;
    }
  }
}

