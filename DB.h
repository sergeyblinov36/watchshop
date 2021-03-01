#pragma once

#ifndef _DB_H
#define _DB_H

#include <jdbc/mysql_connection.h>

#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <iostream>
#include <tuple>
#include <vector>
#include <ctime>

using namespace sql;
using namespace std;
#define DB_NAME "sys"
#define DB_HOST "tcp://127.0.0.1/"
#define DB_PORT 3306
#define DB_USER "root"
#define DB_PASS "root"

class Database {
private:
	// singleton instance
	static Database* instance;
	
	Driver* driver;
	ConnectOptionsMap connection_properties;

	// private CTOR (singleton)s
	Database();
public:
	static Database& getInstance();

	Connection* getConnection();

	virtual ~Database() {}


	/* Menu Options Functions */
	//customer functions
	//Log in
	tuple<string, string, int, int> login(string, string);
	bool signUp(string, string, string);
	bool CheckMail(string);
	vector<string> showStock();
	/*string showStock(int);*/
	vector<string> showStock(int,string,int,int);
	bool buy(int,string);
	bool order(int,string,string,int)throw(int);
	bool subscribe(string);
	//bool unsubscribe();
	bool checkifSubscribed(string);
	int getUserid(string);
	vector<string> getReceipt(string);
	vector<string> readMsgs(string);
	void deleteMsgs(string);

	//manager/employee functions
	int getQuantity(int);
	bool changeRole(string);
	bool updateStock(int,int,int = 0,int = 0,string = NULL,string = NULL,string = NULL);
	bool informCustomers();
	vector<string> viewOrders();
	vector<string> viewReport(string,string = NULL);

	
};
#endif