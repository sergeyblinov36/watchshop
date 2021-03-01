#include "DB.h"
#include <stdlib.h>
#include "Controller.h"

using namespace std;

void buy();

int main(int argc, char* argv[])
{
	Database& db = Database::getInstance(); //63 68 65 70 61
	Controller& ctrl = Controller::getInstance();
	int mainSelect = 99;
	int SecSelect = 0;

	db.getConnection();
	//first while,first menu
	while (mainSelect != 0) {

		cout << "\n\n"
			<< "1) login" << endl
			<< "2) sign up" << endl;
		cin >> mainSelect;

		switch (mainSelect) {
			//log in
		case 1:
		{
			system("CLS");
			string email;
			string password;
			cout << endl << "Enter your mail" << endl;
			cin >> email;
			cout << endl << "Enter password" << endl;
			cin >> password;
			if (ctrl.login(email, password))
			{
				SecSelect = 1;
				system("CLS");
			}
			else
			{
				cout << "Email or password is incorrect";
			}
			break;
		}
		//sign up
		case 2:
		{
			system("CLS");
			string email, name, password;
			cout << endl << "Enter your mail" << endl;
			cin >> email;
			cout << endl << "Enter your name" << endl;
			cin >> name;
			cout << endl << "Enter password" << endl;
			cin >> password;
			if (ctrl.signUp(email, name, password))
			{
				SecSelect = 1;
				system("CLS");
			}
			else
			{
				cout << "Incorrect input";
			}
			break;
		}
		default:
		{
			cout << endl << "please select the options above";
		}
		}
		//second while,main menu,after user verification
		while (SecSelect != 0)
		{
			ctrl.welcomeMSG();
			cin >> SecSelect;
			system("CLS");
			//if a customer
			if (ctrl.checkRole())
			{
			cout << "Press 1 to check out selection of watches" << endl
				<< "Press 2 to read your massages" << endl
				<< "press 3 to un/subscribe" << endl
				<< "press 4 to log off" << endl;
			cin >> SecSelect;
			switch (SecSelect)
			{
			case 1:
			{
				ctrl.browse(SecSelect);
#pragma region options

				cout << endl << "To buy a watch from the list please press 1"
					<< endl << "To filter by price please press 2"
					<< endl << "To filter by brand please press 3"
					<< endl << "To filter by type please press 4"
					<< endl << "If you don't see the watch you've been searching for,please consider ordering it by pressing 5"
					<< endl << "To check out please press 6"
					<< endl << "To go back to main menu please press 7"
					<< endl;
#pragma endregion
				cin >> SecSelect;
				system("CLS");
				switch (SecSelect)
				{
					case 1:
					{
						ctrl.browse(SecSelect);
						buy();
						break;
					}
					case 2:
					{
							int low, high;
							cout << "Please enter your lowest price" << endl;
							cin >> low;
							cout << "Please enter your highest price" << endl;
							cin >> high;
							ctrl.browse(SecSelect, "", low, high);
							buy();
							break;
					}
					case 3:
					{
						string brand;
						cout << "Please write the brand of watch you would like to buy" << endl;
						cin >> brand;
						ctrl.browse(SecSelect, brand);
						buy();
						break;
					}
					case 4:
					{
						string type;
						cout << "Please write the type of watch you would like to buy" << endl;
						cin >> type;
						ctrl.browse(SecSelect, type);
						buy();
						break;
					}
					case 5:
					{
						string msg;
						cout << "Please write what watch would you like to order" << endl;
						cin >> msg;
						if (ctrl.order(2, msg))
						{
							cout << "Your order has been recieved.We will contact you soon" << endl;
						}
						else
						{
							cout << "Something went wrong" << endl;
						}
						break;
					}
					case 6:
					{
						ctrl.showReceipt();
						SecSelect = 0;
						break;
					}
					case 7:
					{
						SecSelect = 0;
						break;
					}
				}
			}
			case 2:
			{
				cout << "Feature is under development :(" << endl;
				break;
			}
			case 3:
			{
				ctrl.subcribe();
				if (ctrl.checkSubscribed())
				{
					cout << "Thank you for subscribing" << endl;
				}
				else cout << "Unsubscribed" << endl;
				break;
			}
			case 4:
			{
				SecSelect = 0;
				break;
			}
			default:
			{
				cout << "Wrong input.Please try again" << endl;
			}
			}
			}
			else
			{

			}
		}
	}
	return 0;
}

void buy()
{
	Controller& ctrl = Controller::getInstance();
	int select;
	while (select == 1)
	{
		int watchid;
		cout << "please write the ID of the watch you wish to buy" << endl;
		cin >> watchid;
		if (ctrl.buy(watchid))
		{
			cout << "Enjoy your new watch.\n To buy another watch press 1" << endl
				<< "To go back to main menu press any button";

		}
		else
		{
			cout << "ID does not exist,please press 1 to try again or press any button to go back to main menu" << endl;
		}
		cin >> select;
	}
}

