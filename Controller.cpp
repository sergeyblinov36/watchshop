#include "Controller.h"
#include <tuple>
#include "DB.h"
#include "User.h"

Database& db = Database::getInstance();
Controller* Controller::instance = 0;

Controller& Controller::getInstance() {
	if (Controller::instance == 0) {
		instance = new Controller();
	}
	return *instance;
}

bool Controller::login(string mail, string pass) throw(string)
{
	bool state = false;
	if (mail.empty() && pass.empty())
	{
		throw "mail or password is empty";
	}
	string name,email;
	int sub, role;
	try {
		tie(name, email, sub, role) = db.login(mail, pass);
		state = true;
		cout << name << "  " << email << "  " << sub << "  " << role << endl;
		User* user = new User(name, email, sub, role);
		user->print();
	}
	catch (int e)
	{
		state = false;
	}


	
}




