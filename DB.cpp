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
	state = true;
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
	ResultSet* rset;
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
	ResultSet* rset;
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






////Option 1
//void Database::showBetweenTwoDates() {
//	string temp1, temp2;
//	Connection* con = driver->connect(connection_properties);
//	con->setSchema(DB_NAME);
//	ResultSet* rset;
//	PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM record where beginning >= ? AND ending <= ?;");
//
//	cout << endl << "Please enter date in this pattern YYYY-MM-DD\n" << endl;
//	cout << "Please enter start date: ";
//	clearCin();
//	getline(cin, temp1);
//	pstmt->setString(1, temp1);
//	cout << "Please enter end date: ";
//	getline(cin, temp2);
//	pstmt->setString(2, temp2);
//	rset = pstmt->executeQuery();
//
//	if (rset->first()) {
//		cout << "Number of recorded albums between " << temp1 << " and " << temp2 << " is: " << rset->rowsCount() << "." << endl;
//	}
//	else cout << "Invalid Date or No Orders Found That Where Ordered Between Dates." << endl;
//
//
//	delete con;
//	delete pstmt;
//	delete rset;
//}
//
////Option 2
//void Database::showSongsBetweenTwoDates() {
//
//	string temp1, temp2, temp3;
//	Connection* con = driver->connect(connection_properties);
//	con->setSchema(DB_NAME);
//	ResultSet* rset;
//	PreparedStatement* pstmt = con->prepareStatement("SELECT people.name,song.title FROM people JOIN musicians on musicians.m_id = people.id AND people.name = ? JOIN musicians_song ON musicians.m_id = musicians_song.m_id JOIN song ON song.s_id = musicians_song.s_id AND(song.recordDate >= ? AND song.recordDate< ? );");
//
//
//	cout << endl << "Please enter date in this pattern YYYY-MM-DD\n" << endl;
//	cout << "Please enter musician Name: ";
//	clearCin();
//	getline(cin, temp1);
//	pstmt->setString(1, temp1);
//	cout << "Please enter start date: ";
//	getline(cin, temp2);
//	pstmt->setString(2, temp2);
//	cout << "Please enter end date: ";
//	getline(cin, temp3);
//	pstmt->setString(3, temp3);
//
//	rset = pstmt->executeQuery();
//
//	if (rset->first()) {
//		cout << "Number of recorded songs of " << temp1 << " between " << temp2 << " and " << temp3 << " is: " << rset->rowsCount() << "." << endl;
//	}
//	else cout << "Invalid Date or No Orders Found That Where Ordered Between Dates." << endl;
//
//
//	delete con;
//	delete pstmt;
//	delete rset;
//}
//
////Option 3
//void Database::showAlbumsBetweenDatesAndComposer() {
//	string temp1, temp2, temp3;
//	Connection* con = driver->connect(connection_properties);
//	con->setSchema(DB_NAME);
//	ResultSet* rset;
//	PreparedStatement* pstmt = con->prepareStatement("SELECT DISTINCT people.name,record.title FROM people JOIN musicians ON musicians.m_id=people.id AND people.name= ? JOIN musicians_song ON musicians.m_id=musicians_song.m_id JOIN song ON song.s_id=musicians_song.s_id AND (song.recordDate>= ? AND song.recordDate< ?) JOIN record ON song.r_id=record.r_id");
//
//	cout << endl << "Please enter date in this pattern YYYY-MM-DD\n" << endl;
//	cout << "Please enter musician Name: ";
//	clearCin();
//	getline(cin, temp1);
//	pstmt->setString(1, temp1);
//	cout << "Please enter from date:  ";
//	getline(cin, temp2);
//	pstmt->setString(2, temp2);
//	cout << " Please enter to date: ";
//	getline(cin, temp3);
//	pstmt->setString(3, temp3);
//
//	rset = pstmt->executeQuery();
//
//	if (rset->first()) {
//		cout << "Number of recorded albums where included " << temp1 << " between " << temp2 << " and " << temp3 << " is: " << rset->rowsCount() << "." << endl;
//	}
//	else cout << "Invalid Date or No Data Found." << endl;
//
//
//	delete con;
//	delete pstmt;
//	delete rset;
//}
//
////Option 4
//void Database::showMostPopularInstrument() {
//	Connection* con = driver->connect(connection_properties);
//	con->setSchema(DB_NAME);
//	Statement* stmt = con->createStatement();
//	ResultSet* rset = stmt->executeQuery("SELECT instrument.type,COUNT(musicians_instrument.i_id) AS `value_occurrence` FROM instrument,musicians_instrument WHERE instrument.i_id=musicians_instrument.i_id GROUP BY musicians_instrument.i_id ORDER BY `value_occurrence` DESC LIMIT 1;");
//
//	rset->beforeFirst();
//
//	if (rset->first()) {
//		cout << "Most used instrument is: " << rset->getString("type") << " appears " << rset->getString("value_occurrence") << " times." << endl;
//	}
//
//	delete con;
//	delete stmt;
//	delete rset;
//}
//
////Option 5
//void Database::instrumentsInAlbum() {
//	string temp1;
//	int counter = 1;
//	Connection* con = driver->connect(connection_properties);
//	con->setSchema(DB_NAME);
//	ResultSet* rset;
//	PreparedStatement* pstmt = con->prepareStatement("SELECT  instrument.type,musicians_instrument.snum,manufacturer.company FROM instrument,musicians_instrument,musicians,song,musicians_song,record,manufacturer WHERE record.title= ? AND record.r_id=song.r_id AND song.s_id=musicians_song.s_id  AND musicians_song.m_id=musicians.m_id AND musicians_instrument.m_id=musicians.m_id  AND instrument.i_id=musicians_instrument.i_id AND musicians_instrument.im_id=manufacturer.im_id GROUP BY musicians_instrument.snum;");
//
//	cout << "Please enter album name: ";
//	clearCin();
//	getline(cin, temp1);
//	pstmt->setString(1, temp1);
//
//	rset = pstmt->executeQuery();
//
//	if (rset->rowsCount() == 0) {
//		cout << "No such album found" << endl;
//		return;
//	}
//
//	rset->beforeFirst();
//	cout << "Instruments that album " << temp1 << " includes are:" << endl;
//
//	while (rset->next()) {
//		cout << counter << ") " << rset->getString("type") << endl;
//		++counter;
//	}
//
//	delete con;
//	delete pstmt;
//	delete rset;
//}
//
////Option 6
//void Database::releaseMostNumOfAlbums() {
//	string temp1, temp2;
//	Connection* con = driver->connect(connection_properties);
//	con->setSchema(DB_NAME);
//	ResultSet* rset;
//	PreparedStatement* pstmt = con->prepareStatement("SELECT recordproducer.p_id, producer.p_name, COUNT(recordproducer.p_id) as `count` FROM recordproducer JOIN producer ON producer.p_id = recordproducer.p_id JOIN record ON recordproducer.r_id = record.r_id WHERE record.beginning >= ? AND record.ending<? GROUP BY recordproducer.p_id ORDER BY `count` desc LIMIT 1; ");
//
//	cout << endl << "Please enter date in this pattern YYYY-MM-DD\n" << endl;
//	cout << "Please enter start date: ";
//	clearCin();
//	getline(cin, temp1);
//	pstmt->setString(1, temp1);
//	cout << "Please enter end date: ";
//	getline(cin, temp2);
//	pstmt->setString(2, temp2);
//	rset = pstmt->executeQuery();
//
//	if (rset->first()) {
//		cout << "Most productive producer is: " << rset->getString("p_name") << " which released " << rset->getString("count") << " albums." << endl;
//	}
//	else cout << "Invalid Date or No Orders Found That Where Ordered Between Dates." << endl;
//
//	delete con;
//	delete pstmt;
//	delete rset;
//}
//
////Option 7
//void Database::mostPopularManufacturer() {
//	Connection* con = driver->connect(connection_properties);
//	con->setSchema(DB_NAME);
//	Statement* stmt = con->createStatement();
//	ResultSet* rset = stmt->executeQuery(" SELECT DISTINCT manufacturer.company,COUNT(manufacturer.im_id) AS `value_occurrence` FROM musicians_song JOIN musicians_instrument ON musicians_song.m_id = musicians_instrument.m_id JOIN manufacturer ON musicians_instrument.im_id = manufacturer.im_id GROUP BY manufacturer.im_id ORDER BY `value_occurrence` DESC LIMIT 1; ");
//	int counter = 1;
//	rset->beforeFirst();
//
//	if (rset->first()) {
//		cout << "Most popular manufacturer maker is: " << rset->getString("company") << " which sold " << rset->getString("value_occurrence") << " instruments." << endl;
//	}
//
//	delete con;
//	delete stmt;
//	delete rset;
//	counter = 1;
//}
//
//// Option 8
//void Database::totalRecordsInMin() {
//
//	Connection* con = driver->connect(connection_properties);
//	con->setSchema(DB_NAME);
//	Statement* stmt = con->createStatement();
//	ResultSet* rset = stmt->executeQuery("SELECT COUNT(DISTINCT musicians.m_id) AS 'count' FROM musicians, musicians_song WHERE musicians.m_id = musicians_song.m_id; ");
//	rset->beforeFirst();
//
//	if (rset->first()) {
//		cout << "Total musicians recorded over the years : " << rset->getString("count") << " musicians" << endl;
//	}
//
//	delete con;
//	delete stmt;
//	delete rset;
//}
//
//// Option 9
//void Database::mostAssistMusician() {
//
//	Connection* con = driver->connect(connection_properties);
//	con->setSchema(DB_NAME);
//	Statement* stmt = con->createStatement();
//	ResultSet* rset = stmt->executeQuery("SELECT musicians_song.join_m_id, musicians.name, COUNT(join_m_id) as `count` FROM musicians_song JOIN musicians ON musicians.m_id = join_m_id GROUP BY join_m_id ORDER BY `count` desc LIMIT 1; ");
//	rset->beforeFirst();
//
//	if (rset->first()) {
//		cout << "Most assisted musician : " << rset->getString("name") << " with " << rset->getString("count") << " assists" << endl;
//	}
//
//	delete con;
//	delete stmt;
//	delete rset;
//}
//
//// Option 10
//void Database::mostPopularGenre() {
//	Connection* con = driver->connect(connection_properties);
//	con->setSchema(DB_NAME);
//	Statement* stmt = con->createStatement();
//	ResultSet* rset = stmt->executeQuery("SELECT song.genre_id, genre.genre, COUNT(genre_id) as `count` FROM song JOIN genre ON genre.g_id = genre_id GROUP BY genre_id ORDER BY `count` desc LIMIT 1;");
//	rset->beforeFirst();
//
//	if (rset->first()) {
//		cout << "Most popular genre : " << rset->getString("genre") << " with " << rset->getString("count") << " songs" << endl;
//	}
//
//	delete con;
//	delete stmt;
//	delete rset;
//}
//
//// Option 11 
//void Database::bestThechnician() {
//
//	string temp, temp2;
//	Connection* con = driver->connect(connection_properties);
//	con->setSchema(DB_NAME);
//	ResultSet* rset;
//	PreparedStatement* pstmt = con->prepareStatement(" SELECT song.t_id, technician.t_name,  COUNT(song.t_id) AS `count` FROM song JOIN technician ON technician.t_id = song.t_id WHERE song.recordDate >= ? AND song.recordDate< ? GROUP BY song.t_id ORDER BY `count` DESC LIMIT 1; ");
//	cout << "\nPlease enter date in this pattern YYYY-MM-DD\n" << endl;
//	cout << "Please Enter Start Date:> ";
//	clearCin();
//	getline(cin, temp);
//	pstmt->setString(1, temp);
//	cout << "Please enter to date: ";
//	getline(cin, temp2);
//	pstmt->setString(2, temp2);
//	rset = pstmt->executeQuery();
//
//	if (rset->first()) {
//		cout << rset->getString("t_name") << " " << ", Is the most Protuctive technicain, working on " << rset->getString("count") << " songs, in the given dates. " << endl;
//	}
//
//	else cout << "Invalid Date or No customers Found That Ordered Any Books After The Date That Was Given." << endl;
//
//
//	delete con;
//	delete pstmt;
//	delete rset;
//}
//
////Option 12
//void Database::firstRecAlbum() {
//	Connection* con = driver->connect(connection_properties);
//	con->setSchema(DB_NAME);
//	Statement* stmt = con->createStatement();
//	ResultSet* rset = stmt->executeQuery("SELECT record.title FROM record ORDER BY record.beginning ASC LIMIT 1;");
//	rset->beforeFirst();
//
//	if (rset->first()) {
//		cout << "First recorded album is: " << rset->getString("title") << endl;
//	}
//
//	delete con;
//	delete stmt;
//	delete rset;
//}
//
////Option 13
//void Database::songsInTwoOrMoreAlbums() {
//
//	int counter = 1;
//	Connection* con = driver->connect(connection_properties);
//	con->setSchema(DB_NAME);
//	Statement* stmt = con->createStatement();
//	ResultSet* rset = stmt->executeQuery("SELECT specialsongs.s_id, song.title, specialsongs.r_id, r_id2, r_id3 FROM specialsongs JOIN song ON song.s_id = specialsongs.s_id WHERE r_id2 IS NOT NULL;");
//
//	if (rset->rowsCount() == 0) {
//		cout << "No such song found" << endl;
//		return;
//	}
//
//	rset->beforeFirst();
//	cout << "List of songs that included in more then 2 albums: " << endl;
//	while (rset->next()) {
//		cout << counter << ") " << rset->getString("title") << endl;
//		++counter;
//	}
//	delete con;
//	delete stmt;
//	delete rset;
//}
//
////Option 14
//void Database::techniciansInWholeAlbum() {
//	string temp;
//	int counter = 1;
//	Connection* con = driver->connect(connection_properties);
//	con->setSchema(DB_NAME);
//	ResultSet* rset;
//	PreparedStatement* pstmt = con->prepareStatement(" SELECT technician.t_name, technician.t_id FROM technician, record, song WHERE record.title = ? AND record.r_id = song.r_id AND song.t_id = technician.t_id GROUP BY  technician.t_name;");
//	cout << "Please Enter album name:> ";
//	clearCin();
//	getline(cin, temp);
//	pstmt->setString(1, temp);
//	rset = pstmt->executeQuery();
//
//	rset->beforeFirst();
//	cout << "List of Technician that worked on whole albums: " << endl;
//	while (rset->next()) {
//		cout << counter << ") " << rset->getString("t_name") << endl;
//		++counter;
//	}
//
//
//	delete con;
//	delete pstmt;
//	delete rset;
//}
//
////Option 15
//void Database::musicianWithMostGenres() {
//
//	Connection* con = driver->connect(connection_properties);
//	con->setSchema(DB_NAME);
//	Statement* stmt = con->createStatement();
//	ResultSet* rset = stmt->executeQuery("SELECT  musicians.name, COUNT(song.genre_id) as `count` FROM song INNER JOIN musicians_song ON musicians_song.s_id = song.s_id INNER JOIN musicians ON musicians.m_id = musicians_song.m_id GROUP BY song.genre_id ORDER BY `count` desc LIMIT 1;");
//	rset->beforeFirst();
//	//by CHEPA
//	if (rset->first()) {
//		cout << "Most genres composited: " << rset->getString("name") << " with " << rset->getString("count") << " genres." << endl;
//	}
//
//	delete con;
//	delete stmt;
//	delete rset;
//
//}
#endif