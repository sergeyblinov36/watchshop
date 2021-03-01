#pragma once
#include <iostream>
#include "User.h"
#include <vector>
using namespace std;


class Controller
{
	// singleton instance
	static Controller* instance;
	vector<int> watcheid;

	public:
		static Controller& getInstance();

		bool login(string, string);
		bool signUp(string mail, string name, string pass)throw(string);
		bool browse(int option, string msg = "", int start = 0, int top = 0);
		bool searchByBrand(string);
		bool searchByType(string);
		bool searchByPrice(int);
		bool order(int,string description = NULL,int watchid = 0)throw(int);
		bool buy(int);
		bool subcribe();
		bool checkSubscribed();
		bool unsubscribe();
		void welcomeMSG();
		bool showReceipt();
		void showOrders();
		bool updateStock(int, int, int = 0, int = 0, string = NULL, string = NULL, string = NULL);
		bool viewReport(int[]);
		bool checkRole();
		bool changeRole(string);

};

