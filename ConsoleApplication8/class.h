#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;



class ABC {
public: virtual ~ABC() = 0;
};
//---------------------------------------------------------------------------------------------------
enum Rarity { common, rare, epic, legendary };

class Card : public ABC {
	// Main Card class. Also a spell card.
protected:
	string _name;
	int _cost;
	Rarity _r;
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

	const Rarity GetRarity() const {
		return _r;
	}
	void SetRarity(Rarity r){
		_r = r;
	}

	//--- Constructors / Destructor --------------
	Card() {  }
	Card(Card& c) {
		_name = c._name;
		_cost = c._cost;
		_r = c._r;
	}
	Card(string s, int c, Rarity r) : _name(s), _cost(c), _r(r) { 	}

	~Card() {	}

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
		_r = c._r;
		_attack = c._attack;
		_health = c._health;
	}
	Minion(string s, int c, Rarity r, int a, int h) 
		: Card(s,c,r), _attack(a), _health(h) { 	}

	~Minion() {	}

	//--- Operator<< -----------------------------------
	ostream& operator<<(ostream& o){
		o << _name << ' ' << _cost << ' ' << _attack << '/' << _health;
		return o;
	}
};
// --------------------------------------------------------------------------------------------------

class Decklist{
	// class used in list container
	Card* _card;
	int _quantity;
public:
	// --- Get / Set / Decrement --------------
	const Card* GetCard() const {
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
	Decklist(Card* c, int q) : _card(c), _quantity(q)	{	}
	~Decklist() { 
		delete _card;
	}
};

class Deck {
	// single deck 
	list<Decklist*> _l;
	int _quantity;
	int _nameLength;
	char _border[11];
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

	// --- Construtors / Destructor ---------------
	Deck() : _quantity(0), _nameLength(0)  {
		_border[0] = 185;
		_border[1] = 186;
		_border[2] = 187;
		_border[3] = 188;
		_border[4] = 200;
		_border[5] = 201;
		_border[6] = 202;
		_border[7] = 203;
		_border[8] = 204;
		_border[9] = 205;
		_border[10] = 206;
	}
	~Deck() {
		list<Decklist*>::iterator it;
		for (; it != _l.end(); ++it){
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

	void Show(){
		// ╔════╦══...══╦════╦═══════╗
		cout << _border[5] << _border[9] << _border[9] << _border[9] << _border[9] 
			<< _border[7] << _border[9]; 
		for (int i = 0; i < _nameLength; i++)	{
			cout << _border[9];
		}
		cout << _border[9] << _border[7] << _border[9] << _border[9] << _border[9]
			<< _border[9] << _border[7] << _border[9] << _border[9] << _border[9]
			<< _border[9] << _border[9] << _border[9] << _border[9] << _border[2];



	}
};
//---------------------------------------------------------------------------------------------------

//--- Printing Card with given Name Width --------------
void PrintCard(Card* c, int nameW){
	Minion* m = dynamic_cast<Minion*>(c);
	if (m == 0) {
		cout << setw(nameW) << c->GetName() << ' ' << c->GetCost();
	}
	else {
		cout << setw(nameW) << m->GetName() << ' ' << m->GetCost() 
			<< setw(2) << m->GetAttack() << '/' << setw(2) << m->GetHealth();
	}
}
