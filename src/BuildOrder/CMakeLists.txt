cmake_minimum_required(VERSION 2.8.3)
project(BuildOrder)
## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy rosserial_arduino std_msgs)

add_definitions(-std=c++11)
## Declare ROS messages and services
#add_message_files(DIRECTORY msg FILES Num.msg)
#add_service_files(DIRECTORY srv FILES AddTwoInts.srv)

## Generate added messages and services
#generate_messages(DEPENDENCIES std_msgs)

## Declare a catkin package
catkin_package()
include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(BuildOrder src/BuildOrder.cpp)
target_link_libraries(BuildOrder ${catkin_LIBRARIES})

add_executable(BuildOrder2 src/BuildOrderMultiple.cpp)
target_link_libraries(BuildOrder2 ${catkin_LIBRARIES})
#add_dependencies(BuildOrder BuildOrder_generate_messages_cpp)


#add_executable(talker src/talker.cpp)
#target_link_libraries(talker ${catkin_LIBRARIES})
#add_dependencies(talker talker_generate_messages_cpp)

#add_executable(listener src/listener.cpp)
#target_link_libraries(listener ${catkin_LIBRARIES})
#add_dependencies(listener beginner_tutorials_generate_messages_cpp)
