#include <iostream>
#include <fstream>
using namespace std;


char * crt = "\n----------------------------------------------------\n";
char generisanje[10] = "080658999";
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
struct Artikal {
	char * _naziv;
	float _cijena;
	void Unos(char * naziv, float cijena) {
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);
		_cijena = cijena;
	}
	void Dealociraj() { delete[]_naziv; _naziv = nullptr; }
	void Ispis() { cout << _naziv << " " << _cijena << " KM"; }
};
struct Racun {
	//format broja racuna: 006005491
	//osigurati automatsko generisanje broja racuna pocevsi od 000000001
	char _brojRacuna[10];
	Datum _datumKreiranja;
	Artikal * _artikli;
	int * _kolicine;//cuva informaciju o kolicini svakog artikla na racunu
	int _brojArtikala;
	//Potpisi funkcija trebaju odgovarati onima u main-u
	//1. Unos
	void Unos(char *_brojRacuna, Datum datum){
		strcpy_s(this->_brojRacuna, 10, _brojRacuna);
		_datumKreiranja.Unos(*datum._dan, *datum._mjesec, *datum._godina);
		_artikli = nullptr;
		_brojArtikala = 0;
	}
	//2. Dealociraj
	void Dealociraj(){
		_datumKreiranja.Dealociraj();
		for (int i = 0; i < _brojArtikala; i++)
			_artikli[i].Dealociraj();
		delete[] _artikli; _artikli = nullptr;
		delete[] _kolicine; _kolicine = nullptr;
	}
	//3. DodajArtikal - dodaje novi artikal u listu artikala zajedno sa njegovom kolicinom. 
	//Onemoguciti ponavljanje artikala na nacin da se uvecava samo kolicina ukoliko
	//	korisnik vise puta pokusa dodati isti artikal.
	void DodajArtikal(Artikal a, float kolicina){
		for (int i = 0; i < _brojArtikala; i++)
		{
			if (strcmp(_artikli[i]._naziv, a._naziv) == 0){
				_kolicine[i] = _kolicine[i] + kolicina;
				return;
			}
		}
		Artikal *temp = new Artikal[_brojArtikala + 1];
		int *temp_kolicina = new int[_brojArtikala + 1];
		for (int i = 0; i < _brojArtikala; i++){
			temp[i].Unos(_artikli[i]._naziv, _artikli[i]._cijena);
			temp_kolicina[i] = _kolicine[i];
		}

		temp[_brojArtikala].Unos(a._naziv, a._cijena);
		temp_kolicina[_brojArtikala] = kolicina;

		for (int i = 0; i < _brojArtikala; i++)
			_artikli[i].Dealociraj();
		if (_artikli != nullptr && _kolicine != nullptr){
			delete[] _artikli;
			delete[] _kolicine;
		}
		_artikli = temp;
		_kolicine = temp_kolicina;
		_brojArtikala++;

	}
		//4. Ispis - ispisuje racun u formatu prikazanom na slici (nije obavezno da bude 
	//	identican, ali je svakako pozeljno). Prilikom svakog ispisa, racun
	//	je potrebno spasiti u tekstualni fajl sa istim nazivom kao i broj racuna.
	void Ispis(){
		char naziv_fajla[14];
		strcpy_s(naziv_fajla, 14, _brojRacuna);
		strcat_s(naziv_fajla, 14, ".txt");
		ofstream upis_u_fajl(naziv_fajla);
		
		cout << "Broj Racuna -> " << _brojRacuna<<crt;
		cout << "Datum racuna -> ";
		_datumKreiranja.Ispis();
		cout << crt;
		cout << "Artikli: " << endl;
		for (int i = 0; i < _brojArtikala; i++)
		{
			_artikli[i].Ispis();
			cout << "Kolicina -> " << _kolicine[i]<<endl;
			cout << crt;
		}

		if (!upis_u_fajl.fail()){
			upis_u_fajl << "Broj racuna -> " << _brojRacuna <<crt;
			upis_u_fajl << "Datum racuna -> " << *_datumKreiranja._dan << "/" << *_datumKreiranja._mjesec << "/" << *_datumKreiranja._godina<<crt;
			for (int i = 0; i < _brojArtikala; i++)
			{
				upis_u_fajl << _artikli[i]._naziv << " " << _artikli[i]._cijena << " KM"<<endl;
				upis_u_fajl << "Kolicina -> " << _kolicine[i];
				upis_u_fajl << crt;
				
			}
			upis_u_fajl.close();
		}
		
	}
		//5. BinarnaPretraga - koristeci binarnu pretragu, na osnovu naziva, pronalazi i vraca 
	//	pokazivac na artikal. Ukoliko trazeni artikal ne postoji funkcija vraca nullptr.
	Artikal *BinarnaPretraga(char *naziv){
		
	}
};
//1. PronadjiNajskupljiArtikal - rekurzivna funkcija koja vraca pokazivac na artikal sa najvecom ukupnom cijenom (cijena*kolicina)
Artikal *PronadjiNajskupljiArtikal(Artikal *artikli, int broj_artikala, Artikal *najskuplji){
	if (broj_artikala - 1 < 0)
		return najskuplji;
	if (artikli[broj_artikala - 1]._cijena > najskuplji->_cijena)
		return PronadjiNajskupljiArtikal(artikli, broj_artikala - 1, &artikli[broj_artikala - 1]);
	return PronadjiNajskupljiArtikal(artikli, broj_artikala - 1, najskuplji);
}
//2. GenerisiSljedeciBrojRacuna - generise i vraca naredni broj racuna
#pragma warning (disable:4996)
char *GenerisiSljedeciBrojRacuna(){

	
	for (int i = 8; i >= 0; i--)
	{
			int temp;
			char temp_chr = generisanje[i];
			temp = atoi(&temp_chr);
			++temp;
			if (temp<10){
				char temp_chr[2];
				_itoa(temp, &temp_chr[0], 10);
				generisanje[i] = temp_chr[0];
				break;
			}
			generisanje[i] = '0';
			
	}
	return generisanje;
}
void main() {
	//za eventualne nejasnoce analizirati testni program i ispise na slici
	Datum danas; danas.Unos(13, 6, 2016);
	Artikal cigarete, cokolada, sok;
	cigarete.Unos("Cigarete", 2.23);
	cokolada.Unos("Cokolada", 1.23);
	sok.Unos("Cappy", 2.10);
	Racun racun;
	racun.Unos(GenerisiSljedeciBrojRacuna(), danas);
	racun.DodajArtikal(cokolada, 5);
	racun.DodajArtikal(sok, 10);
	racun.DodajArtikal(cigarete, 2);
	racun.DodajArtikal(cigarete, 5);
	racun.Ispis();
	Artikal * p1 = racun.BinarnaPretraga("Cokolada");
	cout << "Binarnom pretragom pronadjen artikal -> ";
	//p1->Ispis();
	racun.Ispis();
	Artikal * p2 = PronadjiNajskupljiArtikal(racun._artikli, racun._brojArtikala, &racun._artikli[0]);
	cout << "Rekurzivno pronadjen najskuplji artikal -> ";
	p2->Ispis();
	cout << endl;
	//dealocirati zauzetu memoriju
	danas.Dealociraj();
	cigarete.Dealociraj();
	cokolada.Dealociraj();
	sok.Dealociraj();
	racun.Dealociraj();
}