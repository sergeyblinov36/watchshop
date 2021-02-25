#pragma once
#include <iostream>

using namespace std;
class User
{
private:
	string name;
	string email;
	int subscribed;
	int role;
public:
	User(string,string,int,int);
	void print();
	string getEmail();

};

