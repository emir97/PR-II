#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

char * crt = "\n----------------------------------------------------\n";
char _Indeks[9] = "IB150000";
struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() {
		delete _dan; delete _mjesec; delete _godina;
		_dan = _mjesec = _godina = nullptr;
	}
	
};
struct Izostanak {
	char * _razlogIzostanka;
	Datum _datumIzostanka;
	int _brojSati;
	bool _opravdano;//pravdanje zahtijeva da se evidentira i razlog izostanka	
	void Unos(Datum datumIzostanka, int brojSati) {
		_datumIzostanka.Unos(*datumIzostanka._dan, *datumIzostanka._mjesec, *datumIzostanka._godina);
		_brojSati = brojSati;
		_opravdano = false;
		_razlogIzostanka = new char[1];
		_razlogIzostanka[0] = '\0';
	}
	void Dealociraj() { delete[]_razlogIzostanka; _razlogIzostanka = nullptr; }
	void Ispis() { _datumIzostanka.Ispis(); cout << _brojSati << " " << _razlogIzostanka; }
	void Opravdaj(char * razlogIzostanka) {
		if (_razlogIzostanka != nullptr)
			delete[] _razlogIzostanka;
		int vel = strlen(razlogIzostanka) + 1;
		_razlogIzostanka = new char[vel];
		strcpy_s(_razlogIzostanka, vel, razlogIzostanka);
		_opravdano = true;
	}
};
struct Student{
	//format broja indeksa: IB150051
	//osigurati automatsko generisanje broja indeksa pocevsi od IB150001
	char _brojIndeksa[9];
	char * _imePrezime;
	Izostanak * _izostanci;
	int _brojIzostanaka;
	//Potpisi funkcija trebaju odgovarati onima u main-u
	//1. Unos
	void Unos(char *broj_indexa, char *ime_prezime){
		strcpy_s(_brojIndeksa, 9, broj_indexa);

		int size = strlen(ime_prezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, ime_prezime);

		_izostanci = nullptr;
		_brojIzostanaka = 0;
	}
	//2. Dealociraj
	void Dealociraj(){
		for (size_t i = 0; i < _brojIzostanaka; i++)
			_izostanci[i].Dealociraj();
		delete[] _izostanci;
		_izostanci = nullptr;
		delete[] _imePrezime;
		_imePrezime = nullptr;
	}
	/*3. DodajIzostanak - dodaje novi izostanak u listu izostanaka.	Onemoguciti ponavljanje izostanaka na nacin da se uvecava samo broj sati ukoliko
	korisnik (profesor) vise puta pokusa dodati izostanak na isti dan.*/
	void DodajIzostanak(Izostanak izostanak){
		for (size_t i = 0; i < _brojIzostanaka; i++)
		{
			if (*_izostanci[i]._datumIzostanka._dan == *izostanak._datumIzostanka._dan &&
				*_izostanci[i]._datumIzostanka._mjesec == *izostanak._datumIzostanka._mjesec &&
				*_izostanci[i]._datumIzostanka._godina == *izostanak._datumIzostanka._godina){
				_izostanci[i]._brojSati = _izostanci[i]._brojSati + izostanak._brojSati;
				return;
			}
		}

		Izostanak *temp_izostanci = new Izostanak[_brojIzostanaka + 1];

		for (size_t i = 0; i < _brojIzostanaka; i++)
		{
			temp_izostanci[i].Unos(_izostanci[i]._datumIzostanka, _izostanci[i]._brojSati);
			temp_izostanci[i]._opravdano = _izostanci[i]._opravdano;
			if (_izostanci[i]._opravdano){
				temp_izostanci[i].Opravdaj(_izostanci[i]._razlogIzostanka);
			}
			_izostanci[i].Dealociraj();
		}
		if (_izostanci != nullptr){
			delete[] _izostanci;
		}
		

		temp_izostanci[_brojIzostanaka].Unos(izostanak._datumIzostanka, izostanak._brojSati);
		if (izostanak._opravdano){
			temp_izostanci[_brojIzostanaka].Opravdaj(izostanak._razlogIzostanka);
		}

		_izostanci = temp_izostanci;

		_brojIzostanaka++;
	}
	//4. Ispis - ispisuje izostanke u formatu prikazanom na slici (nije obavezno da ispis bude identican, ali je svakako pozeljno). Prilikom svakog ispisa, izostanke je potrebno spasiti u tekstualni fajl sa istim nazivom kao i broj indeksa.
	void Ispis(){
		cout << "Broj indexa -> " << _brojIndeksa << endl;
		cout << "Broj Izostanaka -> " << _brojIzostanaka<<endl;
		cout << "Opravdani: " << endl;
		cout << crt;
		for (size_t i = 0; i < _brojIzostanaka; i++)
		{
			if (_izostanci[i]._opravdano){
				cout << "Datum -> ";
				_izostanci[i]._datumIzostanka.Ispis();
				cout << "Broj sati -> " << _izostanci[i]._brojSati << endl;
				cout << "Razlog -> " << _izostanci[i]._razlogIzostanka << endl;
				cout << crt;
			}
		}
		cout << "Neopravdani: "<<endl;
		cout << crt;
		for (size_t i = 0; i < _brojIzostanaka; i++)
		{
			if (!_izostanci[i]._opravdano){
				cout << "Datum -> ";
				_izostanci[i]._datumIzostanka.Ispis();
				cout << "Broj sati -> " << _izostanci[i]._brojSati << endl;
				cout << crt;
			}
		}
		char naziv_fajla[13];
		strcpy_s(naziv_fajla, 13, _brojIndeksa);
		strcat_s(naziv_fajla, 13, ".txt");
		ofstream upis_u_fajl(naziv_fajla);
		upis_u_fajl << "Broj indexa -> " << _brojIndeksa << endl <<
			"Broj Izostanaka -> " << _brojIzostanaka << endl << 
			"Opravdani: " << crt;
		for (size_t i = 0; i < _brojIzostanaka; i++)
		{
			if (_izostanci[i]._opravdano){
				upis_u_fajl << "Datum -> " << *_izostanci[i]._datumIzostanka._dan << "/" << *_izostanci[i]._datumIzostanka._mjesec << "/" << *_izostanci[i]._datumIzostanka._godina<<endl<<
				  "Broj sati -> " << _izostanci[i]._brojSati << endl <<
				 "Razlog -> " << _izostanci[i]._razlogIzostanka << endl <<
				 crt;
			}
		}
		upis_u_fajl << "Neopravdani: " << endl;
		upis_u_fajl << crt;
		for (size_t i = 0; i < _brojIzostanaka; i++)
		{
			if (!_izostanci[i]._opravdano){
				upis_u_fajl << "Datum -> " << *_izostanci[i]._datumIzostanka._dan << "/" << *_izostanci[i]._datumIzostanka._mjesec << "/" << *_izostanci[i]._datumIzostanka._godina << endl <<
					"Broj sati -> " << _izostanci[i]._brojSati << endl <<
					crt;
			}
		}
		upis_u_fajl.close();
	}
	//5. BinarnaPretraga - koristeci binarnu pretragu, na osnovu datuma, pronalazi i vraca pokazivac na izostanak. Ukoliko trazeni izostanak ne postoji funkcija vraca nullptr.
	bool PoredjenjeDatuma(Datum d1, Datum d2){
		// funkcija vraca 
		// 0 -> ako su datumi jednaki
		// 1 -> ako je d1 mjadji od d2
		// -1 -> ako je d1 stariji od d2
		if (*d1._godina == *d2._godina && *d1._mjesec == *d2._mjesec && *d1._dan == *d2._dan)
			return 0;
		if (*d1._godina < *d2._godina)
			return 1;
		else if (*d1._godina > *d2._godina)
			return -1;
		else {
			if (*d1._mjesec < *d2._mjesec)
				return 1;
			else if (*d1._mjesec>*d2._mjesec)
				return -1;
			else {
				if (*d1._dan < *d2._dan)
					return 1;
				else
					return -1;
			}
		}
	}
	Izostanak *BinarnaPretraga(Datum d){
		// sortiranje izostanaka
		for (int i = 0; i < _brojIzostanaka-1; i++){
			for (int j = i+1; j < _brojIzostanaka; j++)
			{
				if (PoredjenjeDatuma(_izostanci[i]._datumIzostanka, _izostanci[j]._datumIzostanka) == 1){
					Izostanak temp;
					temp.Unos(_izostanci[i]._datumIzostanka, _izostanci[i]._brojSati);
					if (_izostanci[i]._opravdano)
						temp.Opravdaj(_izostanci[i]._razlogIzostanka);
					_izostanci[i].Dealociraj();
					_izostanci[i].Unos(_izostanci[j]._datumIzostanka, _izostanci[j]._brojSati);
					if (_izostanci[j]._opravdano)
						_izostanci[i].Opravdaj(_izostanci[j]._razlogIzostanka);
					_izostanci[j].Dealociraj();
					_izostanci[j].Unos(temp._datumIzostanka, temp._brojSati);
					if (temp._opravdano)
						_izostanci[j].Opravdaj(temp._razlogIzostanka);
					temp.Dealociraj();
				}
			}
		}
		
		// binarna pretraga
		int prvi = 0, zadnji = _brojIzostanaka;
		while (prvi<=zadnji)
		{
			int sredni = (prvi + zadnji) / 2;
			if (PoredjenjeDatuma(d, _izostanci[sredni]._datumIzostanka) == 0)
				return _izostanci + sredni;

			if (PoredjenjeDatuma(d, _izostanci[sredni]._datumIzostanka) == 1)
				prvi = sredni + 1;

			if (PoredjenjeDatuma(d, _izostanci[sredni]._datumIzostanka) == -1)
				zadnji = sredni - 1;
		}

		return nullptr;
	}
};
//1. PronadjiNajveciNeopravdaniIzostanak - rekurzivna funkcija koja vraca pokazivac na neopravdani izostanak koji ima najveci broj sati
Izostanak *PronadjiNajveciNeopravdaniIzostanak(Student *s, int brojac = 0, Izostanak * i = nullptr){
	if (brojac == s->_brojIzostanaka)
		return i;
	if (i == nullptr)
		return PronadjiNajveciNeopravdaniIzostanak(s, brojac + 1, &s->_izostanci[brojac]);
	if (s->_izostanci[brojac]._brojSati > i->_brojSati && s->_izostanci[brojac]._opravdano == false)
		return PronadjiNajveciNeopravdaniIzostanak(s, brojac + 1, &s->_izostanci[brojac]);
	return PronadjiNajveciNeopravdaniIzostanak(s, brojac + 1, i);
}
//2. GenerisiSljedeciBrojIndeksa - generise i vraca naredni broj indeksa

#pragma warning (disable: 4996)
char *GenerisiSljedeciBrojIndeksa(){
	int i = 7;
	while (i>1)
	{
		char broj_za_uvecat = _Indeks[i];
		int broj = atoi(&broj_za_uvecat);
		broj = broj + 1;
		if (broj<10){
			char pretvori_u_karakter[2];
			_itoa(broj,pretvori_u_karakter,10);
			_Indeks[i] = pretvori_u_karakter[0];
			break;
		}
		_Indeks[i] = '0';
		i--;
	}
	return _Indeks;
}
#pragma warning (default: 4996) 

void main() {
	Datum jucer, prije5Dana, prije10Dana;
	jucer.Unos(12, 6, 2016);
	prije5Dana.Unos(8, 6, 2016);
	prije10Dana.Unos(3, 6, 2016);

	Student denis;
	denis.Unos(GenerisiSljedeciBrojIndeksa(), "Denis Music");

	Izostanak izostanakJucer, izostanakPrije5Dana, izostanakPrije10Dana;

	izostanakJucer.Unos(jucer, 5);
	denis.DodajIzostanak(izostanakJucer);

	izostanakPrije5Dana.Unos(prije5Dana, 3);
	izostanakPrije5Dana.Opravdaj("Odsutan zbog bolesti - gripa");
	denis.DodajIzostanak(izostanakPrije5Dana);
	izostanakPrije5Dana.Unos(prije5Dana, 2);
	denis.DodajIzostanak(izostanakPrije5Dana);

	izostanakPrije10Dana.Unos(prije10Dana, 1);
	denis.DodajIzostanak(izostanakPrije10Dana);
	izostanakPrije10Dana.Unos(prije10Dana, 1);
	denis.DodajIzostanak(izostanakPrije10Dana);

	denis.Ispis();
	Izostanak * p = denis.BinarnaPretraga(jucer);
	cout << "Binarnom pretragom pronadjen izostanak -> ";
	p->Ispis();
	denis.Ispis();
	Izostanak * p2 = PronadjiNajveciNeopravdaniIzostanak(&denis);
	cout << "Rekurzivno pronadjen najveci neopravdani izostanak -> ";
	p2->Ispis();
	cout << endl;
	//dealocirati zauzetu memoriju
	jucer.Dealociraj();
	prije5Dana.Dealociraj();
	prije10Dana.Dealociraj();
	izostanakJucer.Dealociraj();
	izostanakPrije5Dana.Dealociraj();
	izostanakPrije10Dana.Dealociraj();
	denis.Dealociraj();
}