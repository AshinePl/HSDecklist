#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <list>

#include <libpq-fe.h>
#pragma comment(lib, "libpq.lib")

using namespace std;

//---------------------------------------------------------------------------------------------------

class Card {
	// Main Card class. Also a spell card.
protected:
	string _name;
	int _cost;
public:
	//--- Get / Set -----------------------------
	const string GetName() const {
		return _name;
	}
	void SetName(string name) {
		_name = name;
	}

	const int GetCost() const {
		return _cost;
	}
	void SetCost(int cost) {
		_cost = cost;
	}

	//--- Constructors / Destructor --------------
	Card() {  }
	Card(Card& c) {
		_name = c._name;
		_cost = c._cost;
	}
	Card(string s, int c) : _name(s), _cost(c) { 	}

	inline virtual ~Card() {	}

	//--- Operator<< -------------------------------
	ostream& operator<<(ostream& o){
		o << _name << ' ' << _cost;
		return o;
	}
};
//---------------------------------------------------------------------------------------------------

class Minion : public Card {
	// Minion card class.
	int _attack;
	int _health;
public:
	//--- Get / Set --------------------------------
	const int GetAttack() const {
		return _attack;
	}
	void SetAttack(int attack) {
		_attack = attack;
	}

	const int GetHealth() const {
		return _health;
	}
	void SetHealth(int health) {
		_health = health;
	}

	//--- Constructors / Destructor --------------
	Minion() {  }
	Minion(Minion& c) {
		_name = c._name;
		_cost = c._cost;
		_attack = c._attack;
		_health = c._health;
	}
	Minion(string s, int c, int a, int h)
		: Card(s, c), _attack(a), _health(h) { 	}

	inline virtual ~Minion() {	}
};
// --------------------------------------------------------------------------------------------------

class Decklist{
	// class used in list container
	Card* _card;
	int _quantity;
	int _oryginalQuantity;
public:
	// --- Get / Set / Decrement --------------
	Card* GetCard() const {
		return _card;
	}
	void SetCard(Card* c){
		_card = c;
	}

	const int GetQuantity() const{
		return _quantity;
	}
	void SetQuantity(int q){
		_quantity = q;
	}

	Decklist& operator--(){
		_quantity--;
		return *this;
	}
	// --- Constructors / Destructor --------------
	Decklist(Card* c, int q) : _card(c), _quantity(q), _oryginalQuantity(q)	{	}
	~Decklist() {
		delete _card;
	}
	// --- Functions ------------------------------
	int Decrease() {
		_quantity--;
		return _quantity;
	}
};

class Deck {
	// single deck 
	list<Decklist*> _l;
	PGconn* _conn;
	int _quantity;
	int _nameLength;
	char _border[11]; // for Show() function
public:

	// --- Get/Set --------------------------------
	list<Decklist*> GetList() {
		return _l;
	}
	int GetNameLength() const {
		return _nameLength;
	}
	int GetQuantity() const {
		return _quantity;
	}
	PGconn* GetConnection() const {
		return _conn;
	}

	// --- Construtors / Destructor ---------------
	Deck() : _quantity(0), _nameLength(0)  {
		// connecting to SQL
		_conn = PQconnectdb("dbname=postgres user=postgres password=1234");
		if (PQstatus(_conn) == CONNECTION_BAD)	{
			cout << "ERROR: " << PQerrorMessage(_conn) << endl;
			PQfinish(_conn);
		}
		cout << "Connection OK" << endl;

		_border[0] = (char)185; // ╣
		_border[1] = (char)186; // ║
		_border[2] = (char)187; // ╗
		_border[3] = (char)188; // ╝
		_border[4] = (char)200; // ╚
		_border[5] = (char)201; // ╔
		_border[6] = (char)202; // ╩
		_border[7] = (char)203; // ╦
		_border[8] = (char)204; // ╠
		_border[9] = (char)205; // ═
		_border[10] = (char)206; // ╬
	}
	~Deck() {
		PQfinish(_conn);
		list<Decklist*>::iterator it;
		for (it = _l.begin(); it != _l.end(); ++it){
			delete (*it);
		}
	}

	// --- Functions ------------------------------
	void Add(Card* c, int q = 2) {
		_quantity += q;
		int newLen = strlen(c->GetName().c_str());
		if (newLen > _nameLength)
			_nameLength = newLen;
		_l.push_back(new Decklist(c, q));
	}

	Card* FindCard(const char* name) {
		stringstream query;
		query << "SELECT t_name AS Name, t_cost AS Cost, t_type AS Type, t_attack AS Attack, t_health AS Health FROM tab WHERE t_name ~ '";
		query << name;
		query << "';";

		// Query
		PGresult* result = PQexec(_conn, query.str().c_str());

		ExecStatusType status = PQresultStatus(result);
		if (status == PGRES_TUPLES_OK)
		{
			int rows = PQntuples(result);
			int cols = PQnfields(result);
			// columns names
			cout << "ID ";
			for (int j = 0; j < cols; j++)
			{
				cout << PQfname(result, j) << ' ';
			}
			cout << endl;
			// searching for longest name
			int le = 0;
			for (int i = 0; i < rows; i++)
			{
				if (strlen(PQgetvalue(result, i, 0)) > le)
					le = strlen(PQgetvalue(result, i, 0));
			}

			cout << endl;
			for (int i = 0; i < le + 28; i++)
				cout << '-';
			cout << endl;

			// printing list
			int i = 0;
			for (; i < rows; i++)
			{
				cout << setw(2) << i + 1 << ' ';

				char * ch_result = PQgetvalue(result, i, 0);
				cout << setw(le) << ch_result << ' '; //Name

				ch_result = PQgetvalue(result, i, 1);
				cout << setw(4) << ch_result << ' '; // Cost

				ch_result = PQgetvalue(result, i, 2);
				cout << setw(4) << ch_result << ' '; // Type

				if (!strcmp(ch_result, "M")) {
					ch_result = PQgetvalue(result, i, 3);
					cout << setw(6) << ch_result << ' '; // Attack

					ch_result = PQgetvalue(result, i, 4);
					cout << setw(6) << ch_result << ' '; // Health
				}

				cout << endl;
			}
			cout << endl << "Add Card with ID: ";

			string c;
			cin >> c;
			int num = atoi(c.c_str());

			if (num == 0 || num > i + 1) return NULL;

			else if (!strcmp(PQgetvalue(result, num - 1, 2), "M")) { // if Minion 
				return new Minion(
					PQgetvalue(result, num - 1, 0), // Name
					atoi(PQgetvalue(result, num - 1, 1)), // Cost
					atoi(PQgetvalue(result, num - 1, 3)), // Attack
					atoi(PQgetvalue(result, num - 1, 4)) // Health
					);
			}
			else { // Spell Card
				return new Card(
					PQgetvalue(result, num - 1, 0), // Name
					atoi(PQgetvalue(result, num - 1, 1)) // Cost
					);
			}

		}
		else {
			PQclear(result);
			return NULL;
		}

		//Clearing
		PQclear(result);

	}

	void WriteHeader() {
		// ╔════╦═
		std::cout << _border[5] << _border[9] << _border[9] << _border[9] << _border[9]
			<< _border[7] << _border[9];
		// ═══════...══════════
		for (int i = 0; i < _nameLength; i++)	{
			std::cout << _border[9];
		}
		// ═╦══════╦═══════╦═══╗
		std::cout << _border[9] << _border[7] << _border[9] << _border[9] << _border[9] << _border[9] << _border[9]
			<< _border[9] << _border[7] << _border[9] << _border[9] << _border[9]
			<< _border[9] << _border[9] << _border[9] << _border[9]
			<< _border[7] << _border[9] << _border[9] << _border[9] << _border[2] << endl;

		// ║ LP ║ Name ║ Cost ║ AT/HP ║ N ║
		std::cout << _border[1] << ' ' << "ID" << ' ' << _border[1] << ' ' << setw(_nameLength) << "Name" << ' '
			<< _border[1] << ' ' << "Cost" << ' ' << _border[1]
			<< " At/Hp " << _border[1] << ' ' << "N" << ' ' << _border[1] << endl;

		// ╠════╬═
		std::cout << _border[8] << _border[9] << _border[9] << _border[9] << _border[9]
			<< _border[10] << _border[9];
		// ═══════...══════════
		for (int i = 0; i < _nameLength; i++)	{
			std::cout << _border[9];
		}
		// ═╬══════╬═══════╬═══╣
		std::cout << _border[9] << _border[10] << _border[9] << _border[9] << _border[9] << _border[9] << _border[9]
			<< _border[9] << _border[10] << _border[9] << _border[9] << _border[9]
			<< _border[9] << _border[9] << _border[9] << _border[9] << _border[10] << _border[9]
			<< _border[9] << _border[9] << _border[0] << endl;
	}
	void WriteCards() {

		// ║ $LP ║ $NAME ║ $COST ║ $AT/HP ║   ║
		list<Decklist*>::iterator it = _l.begin();
		int i = 0;
		for (; it != _l.end(); ++it) {
			i++;

			if ((*it)->GetQuantity() > 0) {
				Minion* m = dynamic_cast<Minion*>((*it)->GetCard());
				// - if Minion / Card --- writing attack and healthpoint
				if (m == 0) {
					std::cout << _border[1] << ' ' << setw(2) << i << ' ' << _border[1] << ' ' << setw(_nameLength)
						<< (*it)->GetCard()->GetName() << ' '
						<< _border[1] << ' ' << setw(4) << (*it)->GetCard()->GetCost() << ' ' << _border[1]
						<< "       " << _border[1] << ' ' << (*it)->GetQuantity() << ' ' << _border[1] << endl;
				}
				else {
					std::cout << _border[1] << ' ' << setw(2) << i << ' ' << _border[1] << ' '
						<< setw(_nameLength) << m->GetName() << ' ' << _border[1] << ' ' << setw(4)
						<< m->GetCost() << ' ' << _border[1] << ' ' << setw(2) << m->GetAttack()
						<< '/' << setw(2) << m->GetHealth() << ' ' << _border[1] << ' ' << (*it)->GetQuantity()
						<< ' ' << _border[1] << endl;
				}
			}
		} // end of for loop
	}
	void WriteFooter() {

		// ╚════╩═
		std::cout << _border[4] << _border[9] << _border[9] << _border[9] << _border[9]
			<< _border[6] << _border[9];
		// ═══════...══════════
		for (int i = 0; i < _nameLength; i++)	{
			std::cout << _border[9];
		}
		// ═╩══════╩═══════╩═══╝
		std::cout << _border[9] << _border[6] << _border[9] << _border[9] << _border[9] << _border[9] << _border[9]
			<< _border[9] << _border[6] << _border[9] << _border[9] << _border[9]
			<< _border[9] << _border[9] << _border[9] << _border[9] << _border[6] << _border[9]
			<< _border[9] << _border[9] << _border[3] << endl;
	}

	void Show(){
		WriteHeader();
		WriteCards();
		WriteFooter();

		std::cout << endl;
	}

	void UpdateLength() { // when quantity of card is 0, then don't count it in length of longest string
		unsigned int newlen = 0;
		list<Decklist*>::iterator it = _l.begin();
		for (; it != _l.end(); ++it) {
			if ((**it).GetQuantity() > 0 && newlen < strlen((**it).GetCard()->GetName().c_str())) {
				// if Qunatity > 0 and Length of card more then newlen
				newlen = strlen((**it).GetCard()->GetName().c_str());
			}
		}
		if (newlen < _nameLength) { _nameLength = newlen; } // updating _nameLength
	}

	int Draw(int number_of_cards) { // Decrease number of cards in Deck with given id
		list<Decklist*>::iterator it = _l.begin();
		for (int i = 1; i < number_of_cards && it != _l.end(); i++) { it++; } // setting iterator on card we draw
		if (it == _l.end()) { return 1; } // error: no card with given number
		else {
			if (!(**it).Decrease()) UpdateLength(); // if Decrease() returned 0 then update length
		}
	}

	bool Menu()	{
		char c;
		std::cout << "A - Add Cards to Deck" << endl;
		std::cout << "D - Permamently remove Card from Deck" << endl;
		std::cout << "S - Save Deck to file" << endl;
		std::cout << "L - Load Deck from file" << endl;
		std::cout << "R - Restart Deck" << endl;
		std::cout << "N - New Deck" << endl;
		std::cout << "C - Draw Card" << endl;
		std::cout << "0 - Quit" << endl;
		std::cout << "> ";

		cin >> c; // choice of user
		if (c == '0') return false;

	

		switch (toupper(c)){
			//======================================
		case 'A':
		{
			string name;
			cout << "Podaj nazwe karty: ";
			cin >> name;
			Card* newCard = FindCard(name.c_str());
			do {
				cout << "Podaj ilość: ";
				cin >> name;
				if (atoi(name.c_str())) { break; }
				else cout << "Błędna ilość" << endl;
			} while (true);
			Add(newCard, atoi(name.c_str()));
			break;
		}
			//===========================================
		case 'D':
		{

		}

		}
		return true;
	}
};
//---------------------------------------------------------------------------------------------------

// Prototypes
//--- Printing Card with given Name Width --------------

void PrintCard(Card* c, int nameW){
	Minion* m = dynamic_cast<Minion*>(c);
	if (m == 0) {
		std::cout << setw(nameW) << c->GetName() << ' ' << c->GetCost();
	}
	else {
		std::cout << setw(nameW) << m->GetName() << ' ' << m->GetCost()
			<< setw(2) << m->GetAttack() << '/' << setw(2) << m->GetHealth();
	}
}
