#include<iostream>
#include<memory>
#include<tuple>
using namespace std;
/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NECE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB160061.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI CETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)


enum VrstaAktivnosti { Poligon, Gradska, Nocna, Parkiranje };
enum Kategorija { A, B, C, D, E };
const int maxKategorija = 5;
const char *VrstaAktivnostiChar[] = { "Poligon", "Gradska", "Nocna", "Parkiranje" };
const char *KategorijaChar[] = { "A", "B", "C", "D", "E" };

struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() { /*delete  _dan; delete  _mjesec; delete _godina;*/ }
	int GetDays() { return *_dan + *_mjesec * 30 + *_godina * 365; }
};
struct Aktivnost {
	unique_ptr<VrstaAktivnosti> _vrsta;
	Datum _datumIzvrsenja;
	char * _komentar;
	int _ocjena; // 1 - 5
	void Unos(VrstaAktivnosti vrsta, Datum * datum, int ocjena, const char * komentar) {
		_datumIzvrsenja.Unos(*datum->_dan, *datum->_mjesec, *datum->_godina);
		_ocjena = ocjena;
		int size = strlen(komentar) + 1;
		_komentar = new char[size];
		strcpy_s(_komentar, size, komentar);
		_vrsta = make_unique<VrstaAktivnosti>(vrsta);
	}
	void Ispis() {
		cout << VrstaAktivnostiChar[*_vrsta] << " " << _ocjena << " " << _komentar;
		_datumIzvrsenja.Ispis();
		cout << endl;
	}
	void Dealociraj() {
		_datumIzvrsenja.Dealociraj();
		delete[] _komentar; _komentar = nullptr;
	}
};
struct VozackaKategorija {
	Datum * _datumPolaganja;//datum koji ce se evidentirati kao datum polaganja kategorije tj. kada su ispunjene sve aktivnosti/obaveze
	Kategorija _kategorija;
	Aktivnost * _listaIzvrsenihAktivnosti;
	int _trenutnoIzvrsenihAktivnosti;
	void Unos(Kategorija kategorija) {
		_listaIzvrsenihAktivnosti = nullptr;
		_trenutnoIzvrsenihAktivnosti = 0;
		_datumPolaganja = nullptr;
		_kategorija = kategorija;
	}
	bool DodajIzvrsenuAktivnost(VrstaAktivnosti v, Datum *d, int ocjena, const char *opis) {
		if (_datumPolaganja != nullptr) return false; // Kategorija je uspjesno polozena
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
		{
			if (v == *_listaIzvrsenihAktivnosti[i]._vrsta && _listaIzvrsenihAktivnosti[i]._datumIzvrsenja.GetDays() == d->GetDays() && _listaIzvrsenihAktivnosti[i]._ocjena > 2)
				return false;
			if (v == *_listaIzvrsenihAktivnosti[i]._vrsta && (d->GetDays() - _listaIzvrsenihAktivnosti[i]._datumIzvrsenja.GetDays()) < 30)
				return false;
		}
		Aktivnost *temp = new Aktivnost[_trenutnoIzvrsenihAktivnosti + 1];
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
		{
			temp[i].Unos(*_listaIzvrsenihAktivnosti[i]._vrsta, &_listaIzvrsenihAktivnosti[i]._datumIzvrsenja, _listaIzvrsenihAktivnosti[i]._ocjena, _listaIzvrsenihAktivnosti[i]._komentar);
			_listaIzvrsenihAktivnosti[i].Dealociraj();
		}
		delete[] _listaIzvrsenihAktivnosti;

		temp[_trenutnoIzvrsenihAktivnosti].Unos(v, d, ocjena, opis);
		_listaIzvrsenihAktivnosti = temp;
		_trenutnoIzvrsenihAktivnosti++;
		return true;
	}
	void Sortiraj() {
		bool prolaz = true;
		while (prolaz)
		{
			prolaz = false;
			for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti - 1; i++)
			{
				if (_listaIzvrsenihAktivnosti[i]._ocjena < _listaIzvrsenihAktivnosti[i + 1]._ocjena) {
					Aktivnost temp;
					temp.Unos(*_listaIzvrsenihAktivnosti[i]._vrsta, &_listaIzvrsenihAktivnosti[i]._datumIzvrsenja, _listaIzvrsenihAktivnosti[i]._ocjena, _listaIzvrsenihAktivnosti[i]._komentar);
					_listaIzvrsenihAktivnosti[i].Dealociraj();

					_listaIzvrsenihAktivnosti[i].Unos(*_listaIzvrsenihAktivnosti[i + 1]._vrsta, &_listaIzvrsenihAktivnosti[i + 1]._datumIzvrsenja, _listaIzvrsenihAktivnosti[i + 1]._ocjena, _listaIzvrsenihAktivnosti[i + 1]._komentar);
					_listaIzvrsenihAktivnosti[i + 1].Dealociraj();

					_listaIzvrsenihAktivnosti[i + 1].Unos(*temp._vrsta, &temp._datumIzvrsenja, temp._ocjena, temp._komentar);
					temp.Dealociraj();
					prolaz = true;
				}
			}
		}
	}
	bool DaLiJePolozena() {
		int brojaci[4] = { 0 };
		int negativneOcjene[4] = { 0 };
		Datum temp;
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
		{
			if (_listaIzvrsenihAktivnosti[i]._ocjena > 2) {
				brojaci[*_listaIzvrsenihAktivnosti[i]._vrsta]++;
				temp.Unos(*_listaIzvrsenihAktivnosti[i]._datumIzvrsenja._dan, *_listaIzvrsenihAktivnosti[i]._datumIzvrsenja._mjesec, *_listaIzvrsenihAktivnosti[i]._datumIzvrsenja._godina);
			}
			if (_listaIzvrsenihAktivnosti[i]._ocjena < 3)
				negativneOcjene[*_listaIzvrsenihAktivnosti[i]._vrsta]++;
		}
		for (size_t i = 0; i < 4; i++)
		{
			if (negativneOcjene[i] >= 3 || brojaci[i] < ((int)_kategorija + 1))
				return false;
		}
		_datumPolaganja = new Datum;
		_datumPolaganja->Unos(*temp._dan, *temp._mjesec, *temp._godina);
		return true;
	}
	float PretragaRekrzivno(const char *str, int brojac = 0, int brojIzvrsenihAktivnosti = 0, int sumaOcjena = 0) {
		if (brojac == _trenutnoIzvrsenihAktivnosti) {
			if (brojIzvrsenihAktivnosti == 0) return 0;
			return (float)sumaOcjena / brojIzvrsenihAktivnosti;
		}
		if (strstr(_listaIzvrsenihAktivnosti[brojac]._komentar, str) != nullptr && _listaIzvrsenihAktivnosti[brojac]._ocjena > 2)
			return PretragaRekrzivno(str, brojac + 1, brojIzvrsenihAktivnosti + 1, sumaOcjena + _listaIzvrsenihAktivnosti[brojac]._ocjena);
		return PretragaRekrzivno(str, brojac + 1, brojIzvrsenihAktivnosti, sumaOcjena);
	}
	void Kopiraj(VozackaKategorija k) {
		Unos(k._kategorija);
		_trenutnoIzvrsenihAktivnosti = k._trenutnoIzvrsenihAktivnosti;
		_listaIzvrsenihAktivnosti = new Aktivnost[k._trenutnoIzvrsenihAktivnosti];
		for (size_t i = 0; i < k._trenutnoIzvrsenihAktivnosti; i++)
			_listaIzvrsenihAktivnosti[i].Unos(*k._listaIzvrsenihAktivnosti[i]._vrsta, &k._listaIzvrsenihAktivnosti[i]._datumIzvrsenja, k._listaIzvrsenihAktivnosti[i]._ocjena, k._listaIzvrsenihAktivnosti[i]._komentar);
		if (k._datumPolaganja != nullptr)
			DaLiJePolozena();
	}
	void Dealociraj() {
		if (_datumPolaganja != nullptr) {
			_datumPolaganja->Dealociraj();
			delete _datumPolaganja;
		}
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			_listaIzvrsenihAktivnosti[i].Dealociraj();
		delete[] _listaIzvrsenihAktivnosti;
		_listaIzvrsenihAktivnosti = nullptr;
	}
	void Ispis() {
		if (_datumPolaganja != nullptr)
			_datumPolaganja->Ispis();
		cout << KategorijaChar[_kategorija] << endl;
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			_listaIzvrsenihAktivnosti[i].Ispis();
	}
};

struct Kandidat {
	char * _imePrezime;
	shared_ptr<VozackaKategorija> _kategorije[maxKategorija];
	int _trenutnoKategorija;
	void Unos(const char * imePrezime) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		_trenutnoKategorija = 0;
	}
	bool DodajKategoriju(VozackaKategorija k) {
		auto dodaj = [this, k]() {
			if (_trenutnoKategorija != (int)k._kategorija) return false;
			return true;
		};
		if (!dodaj()) return false;
		_kategorije[_trenutnoKategorija] = make_shared<VozackaKategorija>();
		_kategorije[_trenutnoKategorija]->Kopiraj(k);
		_trenutnoKategorija++;
		return true;
	}
	pair<Kategorija, float> GetNajboljuKategoriju() {
		float prosjek = 0;
		Kategorija k;
		for (size_t i = 0; i < _trenutnoKategorija; i++)
		{
			float trenutnoProsjek = 0;
			int brojac = 0;
			for (size_t j = 0; j < _kategorije[i]->_trenutnoIzvrsenihAktivnosti; j++)
			{
				if (_kategorije[i]->_listaIzvrsenihAktivnosti[j]._ocjena > 2) {
					trenutnoProsjek += _kategorije[i]->_listaIzvrsenihAktivnosti[j]._ocjena;
					brojac++;
				}
			}
			if (brojac == 0) brojac = 1;
			trenutnoProsjek /= brojac;
			if (prosjek < trenutnoProsjek) {
				prosjek = trenutnoProsjek;
				k = _kategorije[i]->_kategorija;
			}
		}
		return pair<Kategorija, float>(k, prosjek);
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < _trenutnoKategorija; i++) {
			_kategorije[i]->Dealociraj();
			_kategorije[i].reset();
		}
	}
	void Ispis() {
		cout << _imePrezime << endl;
		for (size_t i = 0; i < _trenutnoKategorija; i++)
			_kategorije[i]->Ispis();
	}
};

void main() {

	//BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA
	//STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
	Datum datumPolaganja1, datumPolaganja2, datumPolaganja3, datumPolaganja4;
	datumPolaganja1.Unos(10, 6, 2018);
	datumPolaganja2.Unos(18, 6, 2018);
	datumPolaganja3.Unos(22, 3, 2018);
	datumPolaganja4.Unos(28, 7, 2018);

	VozackaKategorija kategorijaA, kategorijaB, kategorijaC;
	kategorijaA.Unos(A);
	kategorijaB.Unos(B);
	kategorijaC.Unos(C);

	/*Na osnovu vrijednosti primljenog parametra osigurati dodavanje novoizvrsene aktivnosti za potrebe stjecanja odredjene vozacke kategorije. Broj aktivnosti nije ogranicen.
	Identicna aktivnost se moze dodati jedino u slucaju kada je prethodna (identivna aktivnost po vrsti i datumu izvrsenja) imala ocjenu manju od 3.
	Uspjesnom aktivnoscu se smatraju one aktivnosti koje imaju ocjenu vecu od 2, a svaka naredna identicna aktivnost, bez obzira da li je uspjesna ili ne,
	moze biti dodana jedino ako je proslo najmanje 30 dana od izvrsenja prethodne. Onemoguciti dodavanje aktivnosti uspjesno polozenoj kategoriji.*/
	if (kategorijaA.DodajIzvrsenuAktivnost(Poligon, &datumPolaganja1, 3, "izuzetno dobra orijentacija"))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (kategorijaA.DodajIzvrsenuAktivnost(Gradska, &datumPolaganja1, 2, "nesigurnost u raskrsnicama"))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (kategorijaA.DodajIzvrsenuAktivnost(Gradska, &datumPolaganja2, 4, "raskrsnice savladane"))//datum?
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (kategorijaA.DodajIzvrsenuAktivnost(Gradska, &datumPolaganja4, 4, "raskrsnice savladane"))
		cout << "Aktivnost uspjesno dodana!" << endl;


	//koristeci neki od obradjenih algoritama, po ocjeni sortirati aktivnosti u okviru odredjene kategorije
	kategorijaA.Sortiraj();

	/*Stjecanje kategorije zahtijeva uspjesnu realizaciju svih planiranih aktivnosti, a one se za jedan povecavaju sa svakom novom kategorijom, npr.
	A: 1 x poligon, 1 x gradska, 1 x nocna, 1 x parkiranje;
	B: 2 x poligon, 2 x gradska, 2 x nocna, 2 x parkiranje; i td...
	Funkcija vraca false u slucaju da: su kandidatu u listu aktivnosti evidentirane tri negativno ocijenjene identicne vrste aktivnosti (npr. tri negativne ocjene iz gradske voznje), onda se ta kategorija ne moze smatrati uspjesno polozenom
	,te ukoliko nedostaje bilo koja od aktivnosti zahtijevanih u okviru te kategorije. Ukoliko je kandidat uspjesno realizovao sve aktivnost, datum polaganja se postavlja na datum posljednje uspjesno realizovane aktivnosti*/
	if (kategorijaA.DaLiJePolozena())
		kategorijaA.Ispis();

	/*Funkcija vraca prosjecnu ocjenu svih uspjesno realizovanih aktivnosti koja u nazivu ili opisu sadrze vrijednost primljenog parametra. Ukoliko smatrate da je potrebno, mozete dodati i druge parametre za potrebe implementacije ove funkcije*/
	cout << "Prosjecna ocjena za kategoriju A -> " << kategorijaA.PretragaRekrzivno("nesigurno") << endl;

	//ispisuje sve dostupne podatke o kategoriji
	kategorijaA.Ispis();

	Kandidat jasmin;
	jasmin.Unos("Jasmin Azemovic");

	/*
	Pretpostavka je da se kategorije moraju polagati po redoslijedu tj. ne smije se dozvoliti dodavanje kategorije C ukoliko prethodno nisu uspjesno polozene kategorije B i C. Za provjeru ispunjenosti preduslova dodavanja nove kategorije, unutar funkcije DodajKategoriju, kreirati lambda funkciju.
	*/
	if (jasmin.DodajKategoriju(kategorijaA))
		cout << "Kategorija uspjesno dodan!" << endl;
	if (jasmin.DodajKategoriju(kategorijaC))
		cout << "Kategorija uspjesno dodan!" << endl;
	if (jasmin.DodajKategoriju(kategorijaB))
		cout << "Kategorija uspjesno dodan!" << endl;

	float prosjek = 0;
	Kategorija kategorija;
	//Funkcija GetNajboljuKategoriju  vraca par koji sadrzi oznaku i prosjecnu ocjenu (uspjesno okoncanih aktivnosti) kategorije sa najvecim prosjekom
	tie(kategorija, prosjek) = jasmin.GetNajboljuKategoriju();
	cout << "Najbolji rezultat od " << prosjek << " je ostvaren tokom polaganja kategorije " << kategorija << endl;


	datumPolaganja1.Dealociraj(), datumPolaganja2.Dealociraj(), datumPolaganja3.Dealociraj(), datumPolaganja4.Dealociraj();
	kategorijaA.Dealociraj(), kategorijaB.Dealociraj(), kategorijaC.Dealociraj();
	jasmin.Dealociraj();

	system("pause");
}

