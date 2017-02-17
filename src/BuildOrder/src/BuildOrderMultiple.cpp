//============================================================================
// Name        : BuildOrder.cpp
// Author      : Marco Houtman
// Version     :
// Copyright   : robinRobotDesign
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <ros/ros.h>
#include "std_msgs/String.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <math.h>
#include <list>
#include <thread>
using namespace std;
char name[25];
static string charSet = "9876543210";
string finalOrder;
ofstream myfile;
ifstream myfile2;
std::list<int> handledOrders;
std::list<string> orders;
bool arduinoIsReady;
ros::Publisher chatter_pub;
ros::Subscriber sub;

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
	string a = msg->data;
//	cout<<a;
	if(a.find("Ready for new order") != string::npos){
//		ROS_INFO("%s", a.c_str());
//		cout<<"Ready for new order";
		arduinoIsReady = true;
	} else
	if(a.find("Order has been received") != string::npos){
//		cout<<"Order has been received";
		arduinoIsReady = false;
	}
}

string trim(string s) {
	s=s.substr(1,s.length()-1);
	s.erase(s.find_last_not_of(" \n\r\t")+1);
	return s;
}


long long decodeOrder(string num) {
	long long base10 = 0;
	cout<<num;
	for(int i = 0; i<num.length(); i++) {
		base10 += pow(charSet.length(), i)*charSet.find(num[i]);
/*		cout<< "Find char:"<<num[i]<<endl;
		cout<< base10<<endl;*/
	}
	return base10;
}

string createFullOrder(string num, string station) {
	std_msgs::String msg;
	long long a = decodeOrder(trim(num));
	int nDigits = floor(log10(abs(a))) + 1;
	char str[nDigits];
	sprintf(str, "%lld", a);
	std::string order(str);
	cout<<"Order = " <<order <<endl;
	if(order.length()>12){
		cout<<order.length()<<endl;
		int a = atoi(order.substr(0, order.length()-12).c_str());
		cout<< a <<endl;
		if(!(std::find(handledOrders.begin(), handledOrders.end(), a) != handledOrders.end())){
			myfile << a<<"\n";
			string substring = order.substr(order.length()-12,order.length()-1);
			cout << substring <<endl;
			substring+=station;
//			handledOrders.push_back(a);
			if(!orders.empty()){
				string a = station;
				int position = orders.size();
				a+=position;
				msg.data = a;
				chatter_pub.publish(msg);
				cout<<a;
			}
			orders.push_back(substring);
			return substring;
		}
		cout <<"Order has already been scanned";
		msg.data="Order has already been scanned";
		chatter_pub.publish(msg);
		return "Order has already been scanned";
	}else {
		cout <<"Order can not be scanned";
		msg.data="Order can not be scanned";
		chatter_pub.publish(msg);
		return "Order can not be scanned";
	}
}

void readFile() {
	myfile2.open ("orderNumber.txt",std::fstream::in | std::fstream::out | std::fstream::app);
	char output[100];
	if(myfile2.is_open()) {
		while(!myfile2.eof()){
			myfile2>>output;
			cout <<output<<"\n";
			int a = atoi(output);
	//		handledOrders.push_back(a);
		}
	}
	myfile2.close();
}

void communication() {
	std_msgs::String msg;
	while(true){
		ros::Rate loop_rate(100);
		if(arduinoIsReady&&!orders.empty()){
			string orderToPush = orders.front();
			orders.pop_front();
			msg.data = orderToPush.c_str();
			chatter_pub.publish(msg);
			ROS_INFO("%s", orderToPush.c_str());
			arduinoIsReady = false;
			ros::Duration(3).sleep();
//			usleep(10000);
		}else if(!orders.empty()) {
//			cout<< "Waiting for previous order to complete";
//			cout<<arduinoIsReady<<"\n";
//			ROS_INFO("%s", "Waiting for previous order to complete");
		}
		ros::spinOnce();
		loop_rate.sleep();

	}
}

int main(int argc, char **argv) {
//
	//On start the arduino is ready to take orders
	arduinoIsReady=false;

	//Start thread, used to receive external communication from ROS.
	ros::init(argc, argv, "order");

	ros::NodeHandle nh;
	std::thread first (communication);


	//Read the previous orders
	readFile();

	//Initialize a ROS publisher
	chatter_pub = nh.advertise<std_msgs::String>("set_msg", 100);
	sub = nh.subscribe("orderStatus", 1000, chatterCallback);
	ros::Rate loop_rate(10);

	//Runs while ROS is ready for communication
	while(ros::ok()){
		std_msgs::String msg;

		//Receive message from the scanner
		cin >> name;

		const char letter = name[0];
		std::string order(name);

		/*
		ROS_INFO("%s", msg.data.c_str());
		std::string order(name);
		msg.data = name;
	    chatter_pub.publish(msg);*/

		switch(letter) {
		case 'Z': cout<<orders.size();
		break;
			case 'A': cout<<"Resulting amount order A:";
			myfile.open ("orderNumber.txt", std::fstream::in | std::fstream::out | std::fstream::app);
			order = createFullOrder(name, "A");
			myfile.close();
			break;
			case 'B': cout<<"Resulting amount order B:";
			myfile.open ("orderNumber.txt",std::fstream::in | std::fstream::out | std::fstream::app);
			order =createFullOrder(name, "B");
			myfile.close();
			break;
			case 'C': cout<<"Resulting amount order C:";
			myfile.open ("orderNumber.txt",std::fstream::in | std::fstream::out | std::fstream::app);
			order =createFullOrder(name, "C");
			myfile.close();
			break;
			case 'D': cout<<"Resulting amount order D:";
			myfile.open ("orderNumber.txt",std::fstream::in | std::fstream::out | std::fstream::app);
			order =createFullOrder(name, "D");
			myfile.close();
			break;
		}
		unsigned char cstr[order.length()];
	//	strcpy( cstr, order.c_str());
		//char hello[13]="hello world";
	//	msg.data = order.c_str();
//		chatter_pub.publish(msg);
	//	ROS_INFO("%s", msg.data.c_str());
		//Send Displays "Er wordt al een bestelling verwerkt"
		ros::spinOnce();
		loop_rate.sleep();


/*
	    std_msgs::String msg;
	    ROS_INFO("%s", msg.data.c_str());
	    msg.data = "TEST";
	    chatter_pub.publish(msg);
	    ros::spinOnce();
	    loop_rate.sleep();*/
	//	createfullOrder("BLABLAAAA");
	}

	return 0;
}
