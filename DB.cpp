#ifndef _DB_C
#define _DB_C
#include "DB.h"
#include <iostream>
#include <string>
#include <tuple>

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

//tuple<string, string, int, int> Database::login(string, string)
//{
//	return tuple<string, string, int, int>();
//}

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