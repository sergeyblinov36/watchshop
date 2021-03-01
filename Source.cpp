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
			<< "2) sign up" << endl
			<< "3) exit" << endl;
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
		case 3:
		{
			mainSelect = 0;
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
			//cin >> SecSelect;
			//system("CLS");
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
						cout << "Press any button to continue" << endl;
						cin >> SecSelect;
						SecSelect = 0;
						break;
					}
					case 7:
					{
						SecSelect = 0;
						break;
					}
				}
				system("CLS");
				break;
			}
			case 2:
			{
				cout << "Inbox" << endl;
				ctrl.readMsgs();
				cout << endl << "to delete massages press 1. to go back press any button" << endl;
				cin >> SecSelect;
				if (SecSelect == 1)
				{
					ctrl.deleteMsgs();
				}
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
			system("CLS");
			//An employee has to register as a customer and then the manager changing his settings
			cout << "Press 1 to change role" << endl
				<< "Press 2 to see what is in stock" << endl
				<< "Press 3 to update stock" << endl
				<< "Press 4 to view pending orders" << endl
				<< "Press 5 to view report" << endl
				<< "Press 6 to notify customer upon arriving order" << endl
				<< "Press 7 to log off" << endl;
			cin >> SecSelect;
			switch (SecSelect)
			{
				case 1:
				{
					system("CLS");
					while (SecSelect == 1) 
					{
						string email;
						cout << "Please enter employee's email" << endl;
						cin >> email;
						if (ctrl.changeRole(email))
						{
							cout << "Status changed successfuly" << endl;
							SecSelect = 2;
						}
						else
						{
							cout << "Incorrect input.\n Would you like to try again? \n 1)yes    *)no" << endl;
							cin >> SecSelect;
						}
					}
					break;
				}
				case 2:
				{
					system("CLS");
					ctrl.browse(1);
					cout << "Press any button to go back"<<endl;
					cin >> SecSelect;

					break;
				}
				case 3:
				{
					while (SecSelect != 4)
					{
						system("CLS");
						cout << "Press 1 to refill stock" << endl
							<< "Press 2 to change prices" << endl
							<< "press 3 to stock brand new merchandise" << endl
							<< "Press 4 to go back to main menu" << endl;
						cin >> SecSelect;
						switch (SecSelect)
						{
							case 1:
							{
								int watchid, quantity;
								cout << "Please enter watch ID" << endl;
								cin >> watchid;
								cout << "Please enter how many watches have arrived" << endl;
								cin >> quantity;
								cout << 2;
								ctrl.updateStock(SecSelect, watchid, quantity);
								break;
							}
							case 2:
							{
								int watchid, price;
								
								cout << "Please enter watch ID" << endl;
								cin >> watchid;
								cout << "Please enter the new price" << endl;
								cin >> price;
								ctrl.updateStock(SecSelect, watchid, 0,price);
								break;
							}
							case 3:
							{
								int watchid, quantity, price;
								string brand, type, model;
								cout << "Please enter watch ID" << endl;
								cin >> watchid;
								cout << "Please enter how many watches have arrived" << endl;
								cin >> quantity;
								cout << "Please enter price" << endl;
								cin >> price;
								cout << "Please enter brand" << endl;
								cin >> brand;
								cout << "Please enter type" << endl;
								cin >> type;
								cout << "Please enter model" << endl;
								cin >> model;
								ctrl.updateStock(SecSelect, watchid, quantity, price, brand, type, model);
								break;
							}
							case 4:
							{
								break;
							}
							default:
							{
								cout << "Wrong input please try again" << endl;
								break;
							}
						}
					}
				
				}
				case 4:
				{
					ctrl.showOrders();
					cout << "Press any button to go back" << endl;
					cin >> SecSelect;
					break;
				}
				case 5:
				{
					int date[6];
					cout << "Please enter starting date [YYYY MM DD]"<<endl;
					cin >> date[0] >> date[1] >> date[2];
					cout << "Please enter ending date or anything under 2020 to check the last 24H" << endl;
					cin >> date[3] >> date[4] >> date[5];
					cout << 1;
					ctrl.viewReport(date);
					break;
				}
				case 6:
				{
					cout << "Under development" << endl;
					break;
				}
				case 7:
				{
					SecSelect = 0;
					break;
				}
			}
			}
		}
	}
	return 0;
}

void buy()
{
	Controller& ctrl = Controller::getInstance();
	int select = 1;
	cout << endl;
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

