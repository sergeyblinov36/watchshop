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
bool Controller::login(string mail, string pass) 
{
	bool state = false;
	if ((mail.empty() || pass.empty())||(mail=="\n" || pass=="\n"))
	{
		return state;
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
	Controller::watcheid.clear();
	vector<string> data = db.showStock(option,msg,start,top);
	int numOfCol = 6;
	int i = 0;
	
	int j = 0;
	cout << setw(10) << left << " Watch ID" << "	";
	cout << setw(10) << left << "Brand" << "	";
	cout << setw(10) << left << "Type" << "	";
	cout << setw(10) << left << "Price" << "	";
	cout << setw(10) << left << "Quantity" << "	";
	cout << setw(10) << left << "Model" << "	";
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

bool Controller::showReceipt()
{
	bool state = false;
	vector<string> data = db.getReceipt(user->getEmail());
	if (data.empty())
	{
		state = true;
	}
	int numOfCol = 2;
	int i = 0;
	for (i = 0; i < data.size(); i++)
	{
		if (i % numOfCol == 0)
		{
			cout << endl;
		}
		cout << setw(10) << left << data[i] << "	";
		state = true;
	}
	cout << endl;
	return state;
}

void Controller::showOrders()
{
	vector<string> data = db.viewOrders();
	int numOfCol = 5;
	int i = 0;

	int j = 0;
	for (i = 0; i < data.size(); i++)
	{
		if (i % numOfCol == 0)
		{
			cout << endl;
		}
		cout << setw(10) << left << data[i] << "	";
	}
	cout << endl;
}

bool Controller::updateStock(int option, int watchid, int quantity, int price, string brand , string type, string model )
{
	bool state = false;
	if (option == 1)
	{
		state = db.updateStock(option, watchid, quantity);
	}
	if (option == 2)
	{
		state = db.updateStock(option, watchid, quantity, price);
		
	}
	if (option == 3)
	{
		if (brand != NULL && type != NULL && model != NULL)
		{
			state = db.updateStock(option, watchid, quantity, price,brand, type, model);
		}
	}
	return false;
}



bool Controller::viewReport(int date[])
{
	bool state = false;
	string startDate;
	string endDate= "";
	if (date != NULL)
	{
		if (date[3] < 2020)
		{
			startDate = to_string(date[0]) + "-" + to_string(date[1]) + "-" + to_string(date[2]);
		}
		else if (date[3] >= 2020)
		{
			startDate = to_string(date[0]) + "-" + to_string(date[1]) + "-" + to_string(date[2]);
			endDate = to_string(date[3]) + "-" + to_string(date[4]) + "-" + to_string(date[5]);
		}
		else return false;
	}
	vector<string> data = db.viewReport(startDate,endDate);
	int numOfCol = 2;
	int i = 0;
	for (i = 0; i < data.size(); i++)
	{
		if (i % numOfCol == 0)
		{
			cout << endl;
		}
		cout << setw(10) << left << data[i] << "	";
		state = true;
	}
	cout << endl;
	return state;

}

bool Controller::checkRole()
{
	bool state = false;
	if (user->getRole() == 0)
	{
		state = true;
	}
	return state;
}

bool Controller::changeRole(string email)
{
	if (!email.empty())
	{
		if (db.changeRole(email))
		{
			return true;
		}
	}
	return false;
}

void Controller::readMsgs()
{
	vector<string> data = db.readMsgs(user->getEmail());

	int numOfCol = 2;
	int i = 0;
	for (i = 0; i < data.size(); i++)
	{
		if (i % numOfCol == 0)
		{
			cout << endl;
		}
		cout << setw(10) << left << data[i] << "	";
		
	}
	cout << endl;
	
}

void Controller::deleteMsgs()
{
	db.deleteMsgs(user->getEmail());
}





