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
	PGconn* conn = PQconnectdb("dbname=postgres user=postgres password=1234");

	if (PQstatus(conn) == CONNECTION_BAD)	{
		cout << "ERROR: " << PQerrorMessage(conn) << endl;
	}
	else	{
		cout << "Connection OK" << endl;
	}

	/* for testing purposes
	Deck deck;
	deck.Add(new Card("Andrzej", 3));
	deck.Add(new Minion("Tomek", 6, 3, 8));
	deck.Add(new Card("Maciek", 3),1);
	deck.Add(new Card("Aleksandra", 3));
	deck.Add(new Minion("Iza", 8, 8, 8));
	deck.Add(new Card("Pawel", 3),1);
	deck.Add(new Minion("Nabuchodonozor 2000", 10, 1, 10), 1);

	deck.UpdateLength();
	bool _running = true;
	do
	{
		deck.Show();
		_running = deck.Menu();


		system("cls");
	} while (_running);
	//*/

	/*
	for (int i = 0; i < 255; i++)
	{
	printf("%d: %c\n", i, i);
	}
	//*/

	PQfinish(conn);
	system("pause");

	return 0;
}
