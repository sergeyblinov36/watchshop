#ifndef _DB_C
#define _DB_C
#include "DB.h"
#include <iostream>
#include <string>
#include <tuple>
#include<vector>

using namespace std;

void clearCin() {
	string temp;
	getline(cin, temp);
}


Database* Database::instance = 0;

Database::Database() : driver(get_driver_instance()) { //63 68 65 70 61 
	connection_properties["hostName"] = DB_HOST;
	connection_properties["port"] = DB_PORT;
	connection_properties["userName"] = DB_USER;
	connection_properties["password"] = DB_PASS;
	connection_properties["OPT_RECONNECT"] = true;
}


Database& Database::getInstance() {
	if (Database::instance == 0) {
		instance = new Database();
	}
	return *instance;
}

Connection* Database::getConnection() {
	try {
		Connection* con = driver->connect(connection_properties);
		con->setSchema(DB_NAME);
		return con;
	}
	catch (SQLException& e) {
		cout << e.what();
	}
	return 0;
}



//log in
tuple<string, string, int, int> Database::login(string email, string pass)
{
	Connection* con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet* rset;
	PreparedStatement* pstmt = NULL;
	try {
		pstmt = con->prepareStatement("SELECT * FROM users WHERE email = ? AND password = ?;");
	}
	catch (SQLException& e) {
		cout << e.what();
	}
	pstmt->setString(1, email);
	pstmt->setString(2, pass);
	rset = pstmt->executeQuery();

	if (rset->next())
	{
		string name = rset->getString("name");
		int sub = rset->getInt("subscribed");
		int role = rset->getInt("role");
		tuple<string, string, int, int> userInfo(name, email, sub, role);
		delete con;
		delete pstmt;
		delete rset;
		return userInfo;
	}
	else {
		delete con;
		delete pstmt;
		delete rset;
		throw(0);
	}

}
//;
bool Database::signUp(string email, string name , string pass)
{
	bool state = false;
	Connection* con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet* rset = NULL;
	PreparedStatement* pstmt = NULL;
	try {
		pstmt = con->prepareStatement("INSERT INTO `sys`.`users` (`userid`, `name`, `email`, `subscribed`, `role`, `password`) VALUES(?, ?, ?, ?, ?, ?);");
	}
	catch (SQLException& e) {
		cout << e.what();
	}
	srand(time(0));
	int random = rand();
	try{
		pstmt->setString(1, to_string(random));
		pstmt->setString(2, name);
		pstmt->setString(3, email);
		pstmt->setInt(4, 0);
		pstmt->setInt(5, 0);
		pstmt->setString(6, pass);
		rset = pstmt->executeQuery();
	}
	catch (SQLException& e) {
		cout << e.what();
		cout << endl;
	}

	delete con;
	delete pstmt;
	delete rset;
	
}

bool Database::CheckMail(string email)
{
	bool state = false;
	Connection* con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet* rset;
	PreparedStatement* pstmt = NULL;
	try {
		pstmt = con->prepareStatement("SELECT * FROM users WHERE email = ?;");
	}
	catch (SQLException& e) {
		cout << e.what();
	}
	pstmt->setString(1, email);
	rset = pstmt->executeQuery();

	if (rset->next())
	{
		state = true;
	}
	delete con;
	delete pstmt;
	delete rset;
	return state;
}

vector<string> Database::showStock()
{
	vector<string> data;
	Connection* con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet* rset;
	PreparedStatement* pstmt = NULL;
	try
	{
		pstmt = con->prepareStatement("SELECT * FROM watches ;");
	}
	catch (SQLException& e) {
		cout << e.what();
	}
	rset = pstmt->executeQuery();
	while (rset->next())
	{
		data.push_back(to_string(rset->getInt(1)));
		data.push_back((rset->getString(2)));
		data.push_back((rset->getString(3)));
		data.push_back(to_string(rset->getInt(4)));
		data.push_back(to_string(rset->getInt(5)));
		data.push_back((rset->getString(6)));
	}
	delete con;
	delete pstmt;
	delete rset;
	return data;
}
//1 - browse all
//2 - browse by price
//3 - browse by brand
//4 - browse by type
vector<string> Database::showStock(int option,string msg = NULL, int start = 0, int top = 0)
{
	vector<string> data;
	Connection* con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet* rset;
	PreparedStatement* pstmt = NULL;
	switch (option)
	{
	case 1: 
	{
		try
		{
			pstmt = con->prepareStatement("SELECT * FROM watches ;");
		}
		catch (SQLException& e) {
			cout << e.what();
		}
		break;
	}
	case 2:
	{
		try
		{
			pstmt = con->prepareStatement("SELECT * FROM watches WHERE price>? AND price < ?;");
		}
		catch (SQLException& e) {
			cout << e.what();
		}
		pstmt->setInt(1, start);
		pstmt->setInt(2, top);
		break;
	}
	case 3:
	{
		try
		{
			pstmt = con->prepareStatement("SELECT * FROM watches WHERE brand like ?;");
		}
		catch (SQLException& e) {
			cout << e.what();
		}
		pstmt->setString(1, "%" + msg + "%");
		break;
	}
	case 4:
	{
		try
		{
			pstmt = con->prepareStatement("SELECT * FROM watches WHERE type = ?;");
		}
		catch (SQLException& e) {
			cout << e.what();
		}
		pstmt->setString(1, "%" + msg + "%");
		break;
	}
	}
	rset = pstmt->executeQuery();
	while (rset->next())
	{
		data.push_back(to_string(rset->getInt(1)));
		data.push_back((rset->getString(2)));
		data.push_back((rset->getString(3)));
		data.push_back(to_string(rset->getInt(4)));
		data.push_back(to_string(rset->getInt(5)));
		data.push_back((rset->getString(6)));
	}
	delete con;
	delete pstmt;
	delete rset;
	return data;
}

bool Database::buy(int watchid,string email) throw(int )
{
	bool state = false;
	Connection* con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet* rset;
	PreparedStatement* pstmt = NULL;
	try
	{
		pstmt = con->prepareStatement("SELECT watches.quantity FROM watches WHERE watchid = ?;");
	}
	catch (SQLException& e) {
		cout << e.what();
	}
	pstmt->setInt(1, watchid);
	rset = pstmt->executeQuery();
	if (rset->next())
	{
		if(rset->getInt(1)==0)
		{
			throw 1;
		}
		else {
			int newQuantity = rset->getInt(1) - 1;

			try
			{
				pstmt = con->prepareStatement("UPDATE watches set quantity=? WHERE watchid = ?");
			}
			catch (SQLException& e) {
				cout << e.what();
			}
			pstmt->setInt(1, newQuantity);
			pstmt->setInt(2, watchid);
			rset = pstmt->executeQuery();
			
			int userid = getUserid(email);
			try
			{
				pstmt = con->prepareStatement("INSERT INTO `sys`.`purchases` (`idpurchases`, `userid`, `watchid`, `date`) VALUES(?, ?, ?, NOW()); ");
			}
			catch (SQLException& e) {
				cout << e.what();
			}
			srand(time(0));
			pstmt->setInt(1, rand());
			pstmt->setInt(2, userid);
			pstmt->setInt(3, watchid);
			try{ rset = pstmt->executeQuery(); }
			catch (SQLException& e) {
				cout << e.what();
			}
			
			
			state = true;
			
		}
		
	}
	delete con;
	delete pstmt;
	delete rset;
	return state;
}

//option 1 - watch exists in db but ran out of stock
//option 2 - watch does not exist in db
bool Database::order(int option,string email,string description = NULL,int watchid = 0)
{
	bool state = false;
	Connection* con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet* rset;
	PreparedStatement* pstmt = NULL;
	int userid = getUserid(email);
	cout << description<<endl;
	if (option == 1)
	{
		try
		{
			pstmt = con->prepareStatement("INSERT INTO `sys`.`orders` (`userid`,`watchid`,`date`,`description`) VALUES(?,?,NOW(),?);");
		}
		catch (SQLException& e) {
			cout << e.what();
		}
		pstmt->setInt(1, userid);
		pstmt->setInt(2, watchid);
		pstmt->setString(3, " ");
		rset = pstmt->executeQuery();
		state = true;
	}
	else
	{
		try
		{
			pstmt = con->prepareStatement("INSERT INTO `sys`.`orders` (`userid`,`watchid`,`date`,`description`,`orderid`) VALUES (?,?,NOW(),?,?); ");
		}
		catch (SQLException& e) {
			cout << e.what();
		}
		srand(time(0));
		int newWatch = rand();
		int orderid = rand();
		pstmt->setInt(1, userid);
		pstmt->setInt(2, newWatch);
		pstmt->setString(3, description);
		pstmt->setInt(4, orderid);
		rset = pstmt->executeQuery(); 
		state = true;
	}
	delete con;
	delete pstmt;
	delete rset;

	return state;
}

bool Database::subscribe(string email)
{
	bool state = false;
	Connection* con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet* rset;
	PreparedStatement* pstmt = NULL;
	int userid = getUserid(email);
	try
	{
		pstmt = con->prepareStatement("UPDATE users set subscribed=? WHERE userid = ?;");
	}
	catch (SQLException& e) {
		cout << e.what();
	}
	if (checkifSubscribed(email))
	{
		pstmt->setInt(1, 0);
	}
	else
	{
		pstmt->setInt(1, 1);
		state = true;
	}
	pstmt->setInt(2,userid);
	rset = pstmt->executeQuery();
	
	delete con;
	delete pstmt;
	delete rset;
	return state;
}

bool Database::checkifSubscribed(string email)
{
	bool state = false;
	Connection* con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet* rset;
	PreparedStatement* pstmt = NULL;
	int userid = getUserid(email);
	try
	{
		pstmt = con->prepareStatement("SELECT users.subscribed FROM users WHERE userid = ?;");
	}
	catch (SQLException& e) {
		cout << e.what();
	}
	pstmt->setInt(1, userid);
	rset = pstmt->executeQuery();
	if (rset->next())
	{
		if (rset->getInt(1) != 0)
		{
			state = true;
		}

	}
	delete con;
	delete pstmt;
	delete rset;
	return false;
}

int Database::getUserid(string email)
{
	Connection* con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet* rset;
	PreparedStatement* pstmt = NULL;
	try {
		pstmt = con->prepareStatement("SELECT users.userid FROM users WHERE email = ?;");
	}
	catch (SQLException& e) {
		cout << e.what();
	}
	pstmt->setString(1, email);
	rset = pstmt->executeQuery();
	int userid;
	rset->next();
	userid = rset->getInt(1);

	delete con;
	delete pstmt;
	delete rset;
	return userid;
}

vector<string> Database::getReceipt(string email)
{
	Connection* con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet* rset;
	PreparedStatement* pstmt = NULL;
	int userid = getUserid(email);
	vector<string> data;
	try {
		pstmt = con->prepareStatement("SELECT watches.brand,watches.price FROM watches JOIN purchases on purchases.watchid = watches.watchid WHERE purchases.userid = ? AND purchases.date >= CURDATE() AND purchases.date < CURDATE() + INTERVAL 1 DAY;");
	}
	catch (SQLException& e) {
		cout << e.what();
	}
	pstmt->setInt(1, userid);
	rset = pstmt->executeQuery();
	while (rset->next())
	{
		data.push_back(rset->getString(1));
		data.push_back(to_string(rset->getInt(2)));
	}
	delete con;
	delete pstmt;
	delete rset;
	return data;
}

int Database::getQuantity(int watchid)
{
	int quantity =0;
	Connection* con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet* rset;
	PreparedStatement* pstmt = NULL;
	try {
		pstmt = con->prepareStatement("SELECT quantity FROM watches WHERE watchid=?;");
	}
	catch (SQLException& e) {
		cout << e.what();
	}
	pstmt->setInt(1, watchid);
	rset = pstmt->executeQuery();
	if (rset->next());
	{
		quantity = rset->getInt(1);
	}
	delete con;
	delete pstmt;
	delete rset;
	return quantity;
}

bool Database::changeRole(string email)
{
	bool state = false;
	Connection* con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet* rset;
	PreparedStatement* pstmt = NULL;
	int userid = getUserid(email);
	try
	{
		pstmt = con->prepareStatement("UPDATE users set role=1 WHERE userid = ?;");
	}
	catch (SQLException& e) {
		cout << e.what();
	}
	pstmt->setInt(1, userid);
	rset = pstmt->executeQuery();
	if (rset->next())
	{
		state = true;
	}
	
	delete con;
	delete pstmt;
	delete rset;
	return state;
}


//option 1 - update quantity of existing watch
//option 2 - update price of existing watch
//oprion 3 - insert non existing watch
bool Database::updateStock(int option,int watchid, int quantity, int price, string brand, string type,string model)
{
	bool state = false;
	Connection* con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet* rset = NULL;
	PreparedStatement* pstmt = NULL;
	if (option == 1)
	{
		try
		{
			pstmt = con->prepareStatement("UPDATE watches set quantity = ? WHERE watchid = ?;");
		}
		catch (SQLException& e) {
			cout << e.what();
		}
		int oldQuantity = getQuantity(watchid);
		pstmt->setInt(1, quantity + oldQuantity);
		pstmt->setInt(2, watchid);
		rset = pstmt->executeQuery();
		state = true;
	}
	else if (option == 2)
	{
		try
		{
			pstmt = con->prepareStatement("UPDATE watches set price = ? WHERE watchid = ?;");
		}
		catch (SQLException& e) {
			cout << e.what();
		}
		pstmt->setInt(1, price);
		pstmt->setInt(2, watchid);
		rset = pstmt->executeQuery();
		state = true;
	}
	else if(option == 3)
	{
		try
		{
			pstmt = con->prepareStatement("INSERT INTO `sys`.`watches` (`watchid`,`brand`,`type`,`price`,`quantity`,`model`) VALUES(?,?,?,?,?,?);");
		}
		catch (SQLException& e) {
			cout << e.what();
		}
		pstmt->setInt(1, watchid);
		pstmt->setString(2, brand);
		pstmt->setString(3, type);
		pstmt->setInt(4, price);
		pstmt->setInt(5, quantity);
		pstmt->setString(6, model);
		rset = pstmt->executeQuery();
		state = true;
	}
	delete con;
	delete pstmt;
	delete rset;
	return state;
}

vector<string> Database::viewOrders()
{
	{
		vector<string> data;
		Connection* con = driver->connect(connection_properties);
		con->setSchema(DB_NAME);
		ResultSet* rset;
		PreparedStatement* pstmt = NULL;
		try {
			pstmt = con->prepareStatement("SELECT * FROM orders;");
		}
		catch (SQLException& e) {
			cout << e.what();
		}
		rset = pstmt->executeQuery();
		while (rset->next())
		{
			data.push_back(to_string(rset->getInt(1)));
			data.push_back(to_string(rset->getInt(2)));
			data.push_back(rset->getString(3));
			data.push_back(rset->getString(4));
			data.push_back(to_string(rset->getInt(5)));

		}
		delete con;
		delete pstmt;
		delete rset;
		return data;
	}
}

vector<string> Database::viewReport(string startDate, string endDate)
{
	Connection* con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet* rset =NULL;
	PreparedStatement* pstmt = NULL;
	vector<string> data;
	if (endDate == NULL)
	{
		try {
			pstmt = con->prepareStatement("SELECT purchases.idpurchases,watches.model,watches.price FROM purchases,watches WHERE purchases.watchid = watches.watchid AND purchases.date >= CURDATE() AND purchases.date < ?;");
		}
		catch (SQLException& e) {
			cout << e.what();
		}
		pstmt->setString(1, startDate);
	}
	else {
		try {
			pstmt = con->prepareStatement("SELECT purchases.idpurchases,watches.model,watches.price FROM purchases,watches WHERE purchases.watchid = watches.watchid AND purchases.date >= ? AND purchases.date < ?;");
		}
		catch (SQLException& e) {
			cout << e.what();
		}
		pstmt->setString(1, endDate);
		pstmt->setString(2, startDate);
	}


	try{ rset = pstmt->executeQuery(); }
	catch (SQLException& e) {
		cout << e.what();
	}
	
	while (rset->next())
	{
		data.push_back(to_string(rset->getInt(1)));
		data.push_back(rset->getString(2));
		data.push_back(to_string(rset->getInt(3)));
	}
	delete con;
	delete pstmt;
	delete rset;
	return data;
}


#endif