cmake_minimum_required(VERSION 2.8.3)
project(domino)
add_compile_options(-std=c++11)
option(CXX "enable C++ compilation" ON)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  geometry_msgs
)
catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS roscpp std_msgs geometry_msgs
)

include_directories(
 ${catkin_INCLUDE_DIRS}
 include/${PROJECT_NAME}
)

add_executable(${PROJECT_NAME}_node src/domino_node.cpp src/pmd.cpp src/usb-1024LS.cpp)

target_link_libraries(${PROJECT_NAME}_node
   ${catkin_LIBRARIES}
   /usr/lib/x86_64-linux-gnu/libusb-1.0.so
   /usr/lib/x86_64-linux-gnu/libhidapi-libusb.so
 )

