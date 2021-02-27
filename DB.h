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

	//Log in
	tuple<string, string, int, int> login(string, string);
	bool signUp(string, string, string);
	bool CheckMail(string);
	vector<string> showStock();
	string showStock(int);
	vector<string> showStock(int,string,int,int);

	bool buy(int,string);
	bool order(int,string,string,int)throw(int);
	bool subscribe();
	bool unsubscribe();
	int getUserid(string);


	////Option 1 
	//void showBetweenTwoDates();
	////Option 2
	//void showSongsBetweenTwoDates();
	////Option 3
	//void showAlbumsBetweenDatesAndComposer();
	////Option 4
	//void showMostPopularInstrument();
	////Option 5
	//void instrumentsInAlbum();
	////Option 6
	//void releaseMostNumOfAlbums();
	////Option 7
	//void mostPopularManufacturer();
	//// Option 8
	//void totalRecordsInMin();
	//// Option 9
	//void mostAssistMusician();
	//// Option 10
	//void mostPopularGenre();
	//// Option 11 
	//void bestThechnician();
	////Option 12
	//void firstRecAlbum();
	////Option 13
	//void songsInTwoOrMoreAlbums();
	////Option 14
	//void techniciansInWholeAlbum();
	////Option 15 
	//void musicianWithMostGenres();



};
#endif