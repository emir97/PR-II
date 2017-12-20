#include<iostream>
using namespace std;

enum Kompanija{ Pegasus, TurkishAirlines, AustrianAirlines, FlyEmirates };

ostream & operator << (ostream & COUT, Kompanija &kompanija){
	if (kompanija == 0)
		cout << "Pegasus";
	return COUT;
}

int ID = 1;

struct Putnik{
	int _putnikID;
	char * _imePrezime;
	float _brojPredjenihMilja;
	void Unos(char * imePrezime){
		_putnikID = ID++;
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		_brojPredjenihMilja = 0;
	}
	void Info(){
		cout << "[" << _putnikID << "] " << _imePrezime <<
			" (" << _brojPredjenihMilja << " milja)" << endl;
	}

	void Dealociraj(){ delete[] _imePrezime; _imePrezime = nullptr; }

	void DodajPredjeneMilje(int brojMilja){ _brojPredjenihMilja += brojMilja; }
};

struct Rezervacija{
	Putnik * _putnik;
	int _oznakaSjedista;
	float _cijena;
	void Unos(Putnik *putnik, int oznaka, float cijena){
		_putnik = new Putnik;
		_putnik->Unos(putnik->_imePrezime);
		_oznakaSjedista = oznaka;
		_cijena = cijena;
	}
	void Info(){
		_putnik->Info();
		cout << "Sjediste: " << _oznakaSjedista << " Cijena: "
			<< _cijena << endl;
	}
	void Dealociraj(){ _putnik->Dealociraj(); }
};
struct Let{
	Kompanija * _kompanija;
	char * _pocetak; //pocetna lokacija
	char * _destinacija;
	Rezervacija * _rezervacije;
	int _brojMjesta; //maksimalan broj mjesta na letu  
	float _brojMilja; //odnosi se na duzinu leta – broj predjenih milja
	float _cijenaKarte;

	void Unos(Kompanija kompanija, char * pocetak, char * destinacija,
		int brojMjesta, float brojMilja, float cijena){

		_kompanija =  new Kompanija;
		*_kompanija = kompanija;

		int size = strlen(pocetak) + 1;
		_pocetak = new char[size];
		strcpy_s(_pocetak, size, pocetak);

		size = strlen(destinacija) + 1;
		_destinacija = new char[size];
		strcpy_s(_destinacija, size, destinacija);

		_brojMjesta = brojMjesta;

		_brojMilja = brojMilja;

		_rezervacije = new Rezervacija[_brojMjesta];
		Putnik * temp = new Putnik;
		temp->Unos("<SLOBODNO MJESTO>");		  
		for (int i = 0; i < _brojMjesta; i++)
			_rezervacije[i].Unos(temp, i, 0);
		
		_cijenaKarte = cijena;
	}

	void Dealociraj(){
		/*definisati funkciju vodeci racuna o oslobadjanju svih resursa koji su alocirani
		za potrebe objekta tipa Let*/
		delete[] _pocetak; _pocetak = nullptr;
		delete[] _destinacija; _destinacija = nullptr;
		for (int i = 0; i < _brojMjesta; i++)
		{
			_rezervacije[i]._putnik->Dealociraj();
		}
		delete[] _rezervacije;
		_rezervacije = nullptr;
	}
	bool AddRezervaciju(Putnik * p){
		system("cls");
		/*na samom pocetku, funkcija treba ispisati listu svih SLOBODNIH sjedista na letu, te omoguciti odabir nekog do njih. 
		prilikom formiranja cijene karte voditi racuna o broju predjenih milja; ako je putnik presao od  10000 do 50000 milja 
		onda ostvaruje popust od 10% na punu cijenu karte (_cijenaKarte), od 50000 do 100000 milja ostvaruje popust od 20% 
		na punu cijenu karte, a preko 100000 milja ostvaruje  popust od 30% na punu cijenu karte. onemoguciti dodavanje rezervacija 
		sa identicnim putnicima (isti putnikID i imePrezime). u zavisnosti  od uspjesnosti funkcija vraca true ili false */

		for (int i = 0; i < _brojMjesta; i++)
		{
			if (strcmp(p->_imePrezime, _rezervacije[i]._putnik->_imePrezime) == 0 || p->_putnikID == _rezervacije[i]._putnik->_putnikID){
				cout << "TAJ VEC POSTOJI "<<endl;
				return 0;
			}
		}

		for (int i = 0; i < _brojMjesta; i++)
		{
			if (!strcmp(_rezervacije[i]._putnik->_imePrezime, "<SLOBODNO MJESTO>"))	{
				cout << "Mjesto sa oznakom " << i+1 << " je slobodno..."<<endl;
			}
		}
		int oznaka;
		cout << "Unesite redni broj zeljenog mjesta -> ";
		do{
			cin >> oznaka;
			if (oznaka > _brojMjesta || strcmp(_rezervacije[oznaka-1]._putnik->_imePrezime, "<SLOBODNO MJESTO>") != 0)
			{
				cout << "Mjesto ne postoji ili je vec zauzeto" << endl;
				cout << "Ponovite unos -> ";
			}							   
			
		} while (oznaka > _brojMjesta || strcmp(_rezervacije[oznaka-1]._putnik->_imePrezime, "<SLOBODNO MJESTO>") != 0);

		if (p->_brojPredjenihMilja > 10000 && p->_brojPredjenihMilja <= 50000)
			_cijenaKarte = _cijenaKarte * 0.9;

		else if (p->_brojPredjenihMilja > 50000 && p->_brojPredjenihMilja <= 100000)
			_cijenaKarte = _cijenaKarte * 0.8;
		else
			_cijenaKarte = _cijenaKarte * 0.7;

		_rezervacije[oznaka - 1].Unos(p, oznaka, _cijenaKarte);
		p->DodajPredjeneMilje(_brojMilja);

		return true;
	}

	bool RemoveRezervacija(int oznakaSjedista){
		/*funkcija uklanja rezervaciju na sjedistu cija je oznaka proslijedjena kao parametar funkcije, te vraca vrijednost. 
		U slucaju da oznaceno mjesto nije rezervisano ili ne postoji na tom letu, funkcija vraca vrijednost false */
		if (oznakaSjedista > _brojMjesta)
		{
			cout << "Mjesto sa oznakom " << oznakaSjedista << " ne postoji na ovom letu."<<endl;
			return 0;
		}
		if (strcmp(_rezervacije[oznakaSjedista - 1]._putnik->_imePrezime, "<SLOBODNO MJESTO>") == 0){
			cout << "Greska mjesto sa oznakom " << oznakaSjedista << " nije zauzeto." << endl;
			return 0;
		}
		Putnik *temp = new Putnik;
		temp->Unos("<SLOBODNO MJESTO>");
		_rezervacije[oznakaSjedista - 1].Unos(temp, oznakaSjedista - 1, 0);
		return true;
	}

};

float GetMiljeByKompanija(Let * letovi, int maxLetova, Kompanija k, Putnik p){
	/*funkcija vraca broj milja koje je putnik p ostvario putujuci sa kompanijom k*/
	float brojPredjenihMilja = 0;
	for (int i = 0; i < maxLetova; i++){
		if (*(letovi[i]._kompanija) == k)
			for (int j = 0; j < letovi[i]._brojMjesta; j++)		
			if (strcmp(letovi[i]._rezervacije[j]._putnik->_imePrezime, p._imePrezime) == 0)
				brojPredjenihMilja = brojPredjenihMilja + letovi[i]._brojMilja;
	}
	return brojPredjenihMilja;
}
void main(){
	/*provjeriti validnost izvrsenja svih dostupnih funkcionalnosti. vrijednosti koje koristite prilikom testiranja mogu biti predefinisane tj. ne morate omogucavati korisniku da ih inicijalizuje.*/
	float milje;
	Putnik *a = new Putnik;
	Putnik *b = new Putnik;
	Putnik *c = new Putnik;
	(*a).Unos("Emir Veledar");
	(*b).Unos("Jasmin Azemovic");
	c->Unos("Denis Music");
	//c->Unos("Putnik 1");
	Let *l = new Let[5];
	l[0].Unos(Pegasus, "Mostar", "New York", 20, 2000, 200);
	l[1].Unos(FlyEmirates, "Mostar", "New York", 20, 200, 100);

	l[0].AddRezervaciju(a);
	l[1].AddRezervaciju(a);
	l[0].AddRezervaciju(c);
	milje = GetMiljeByKompanija(l, 2, FlyEmirates, *a);
	cout << milje << endl;
	l[0].Dealociraj();
	l[1].Dealociraj();

	delete[] l;
	l = nullptr;
}