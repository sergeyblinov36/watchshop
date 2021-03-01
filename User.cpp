#include "User.h"

User::User(string name, string email, int sub, int role)
{
	this->name = name;
	this->email = email;
	this->subscribed = sub;
	this->role = role;
}
void User::print()
{
	cout << "welcome  " << this->name << endl;
}
string User::getEmail()
{
	return this->email;
}

int User::getRole()
{
	return this->role;
}


