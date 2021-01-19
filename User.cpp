#include "User.h"

User::User(string name, string email, int sub, int role)
{
	this->name = name;
	this->email = email;
	this->subscribed = sub;
	this->role = role;
}


