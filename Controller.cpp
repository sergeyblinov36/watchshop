#include "Controller.h"
#include <tuple>
#include "DB.h"
#include "User.h"
#include <iomanip>

Database& db = Database::getInstance();
Controller* Controller::instance = 0;

Controller& Controller::getInstance() {
	if (Controller::instance == 0) {
		instance = new Controller();
	}
	return *instance;
}
static User* user=NULL;
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
		user = new User(name, email, sub, role);
		user->print();
	}
	catch (int e)
	{
		state = false;
	}


	
}

bool Controller::signUp(string mail,string name,string pass) throw(string)
{
	bool state = false;
	if (mail.empty() && pass.empty())
	{
		throw "mail ,password or name is empty";
	}
	else if (db.CheckMail(mail))
	{
		throw "user already exists";
	}
	//string name, email;
	//int sub, role;
	try {
		if(db.signUp(mail,name,pass))
		state = true;
		cout << name << "  " << mail << "  " << 0 << "  " << 0 << endl;
		user = new User(name, mail, 0, 0);
	}
	catch (int e)
	{
		state = false;
	}
	return state;
}

bool Controller::browse(int option, string msg, int start, int top)
{

	vector<string> data = db.showStock(option,msg,start,top);
	int numOfCol = 6;
	int i = 0;
	
	int j = 0;
	for (i = 0; i < data.size(); i++)
	{
		if (i % numOfCol == 0)
		{
			cout << endl;
			int id = stoi(data[i]);
			Controller::watcheid.push_back(id);
		}
		cout << setw(10) <<left<<data[i] <<"	";
	}
	return false;
}

bool Controller::order(int option, string description, int watchid)
{
	bool state = false;
	state = db.order(option,user->getEmail(), description, watchid);

	return state;
}

bool Controller::buy(int watchtobuy) throw(int)
{
	bool state = false;
	if (std::find(watcheid.begin(), watcheid.end(), watchtobuy) != watcheid.end())
	{
		try{
			state = db.buy(watchtobuy, user->getEmail());
		}
		catch (int e)
		{
			cout << "exception cought"<<endl;
			throw 0;
		}
		//if (db.buy(watchtobuy, user->getEmail()));
		//{
		//	state = true;
		//}
	}
	return state;
}

bool Controller::subcribe()
{
	bool state = false;
	if (db.subscribe(user->getEmail()))
	{
		state = true;
	}
	return state;
}

bool Controller::checkSubscribed()
{
	db.checkifSubscribed(user->getEmail());
	return false;
}

void Controller::welcomeMSG()
{
	user->print();
}





