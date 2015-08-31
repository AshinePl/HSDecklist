// HearthstoneDecklist.cpp : Defines the entry point for the console application.
// Decklist for Heartstone with File DataBase ( TODO: SQL )

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <list>
#include <libpq-fs.h>
#pragma comment(lib, "libpq.lib")

#include "Deck.h"

using namespace std;

int main()
{
	PGConn* c;
	
	//  /*
	Deck deck;
	deck.Add(new Card("Andrzej", 3));
	deck.Add(new Minion("Tomek", 6, 3, 8));
	deck.Add(new Card("Maciek", 3),1);
	deck.Add(new Card("Aleksandra", 3));
	deck.Add(new Minion("Iza", 8, 8, 8));
	deck.Add(new Card("Pawel", 3),1);
	deck.Add(new Minion("Nabuchodonozor 2000", 10, 1, 10), 1);


	deck.Show();
	//*/

	/*
	for (int i = 0; i < 255; i++)
	{
	printf("%d: %c\n", i, i);
	}
	//*/
	system("pause");

	return 0;
}
