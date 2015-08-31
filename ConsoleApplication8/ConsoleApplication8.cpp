// HearthstoneDecklist.cpp : Defines the entry point for the console application.
// Decklist for Heartstone with File DataBase ( TODO: SQL )

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <list>

#include "Deck.h"

using namespace std;

int main()
{
	///*
	Deck deck;
	deck.Add(new Card("Andrzej", 3));
	deck.Add(new Minion("Tomek", 6, 3, 8));
	deck.Add(new Card("Maciek", 3));
	deck.Add(new Card("Aleksandra", 3));
	deck.Add(new Minion("Iza", 8, 8, 8));
	deck.Add(new Card("Pawe³", 3));


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
