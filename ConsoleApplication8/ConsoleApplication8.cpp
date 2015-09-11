// HearthstoneDecklist.cpp : Defines the entry point for the console application.
// Decklist for Heartstone with File DataBase ( TODO: SQL )

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <list>

#include <libpq-fe.h>
#pragma comment(lib, "libpq.lib")

// NEED:
// libpq.dll
// intl.dll
// libeay32.dll

#include "Deck.h"

using namespace std;

int main()
{

	Deck d;
	bool _running;

	do {
		// if there are no connection then exit
		if (PQstatus(d.GetConnection()) == CONNECTION_BAD) { 
			return 0; 
		}

		d.Show();
		_running = d.Menu();

	} while (_running);

	

	system("pause");

	return 0;
}
