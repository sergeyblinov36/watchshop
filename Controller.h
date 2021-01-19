#pragma once
#include <iostream>

using namespace std;


class Controller
{
	// singleton instance
	static Controller* instance;

	public:
		static Controller& getInstance();

		bool login(string, string) throw(int);
};

