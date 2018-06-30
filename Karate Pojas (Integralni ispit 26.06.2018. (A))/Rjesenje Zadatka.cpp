#include<iostream>
#include <memory>
#include <tuple>
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


enum VrstaAktivnosti { Tehnike, Kata, Borba };
enum Pojas { Zuti, Narandzasti, Zeleni, Plavi, Smedji, Crni };
const char *PojasChar[] = { "Zuti","Narandzasti", "Zeleni", "Plavi", "Smedji", "Crni" };
const char *VrstaAktivnostiChar[] = { "Tehnike", "Kata", "Borba" };
struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() { delete _dan; delete _mjesec; delete _godina; }
	int GetDays() { return *_dan + *_mjesec * 30 + *_godina * 365; }
};
struct Aktivnost {
	unique_ptr<VrstaAktivnosti> _vrsta;
	Datum _datumIzvrsenja;
	char * _nazivOpis;
	int _ocjena; // 1 - 10 
	void Unos(VrstaAktivnosti vrsta, Datum * datum, int ocjena, const char * nazivOpis) {
		_vrsta = make_unique<VrstaAktivnosti>(vrsta);
		_datumIzvrsenja.Unos(*datum->_dan, *datum->_mjesec, *datum->_godina);
		_ocjena = ocjena;
		int size = strlen(nazivOpis) + 1;
		_nazivOpis = new char[size];
		strcpy_s(_nazivOpis, size, nazivOpis);
	}
	void Ispis() {
		cout << VrstaAktivnostiChar[*_vrsta] << " " << _ocjena << " " << _nazivOpis << " ";
		_datumIzvrsenja.Ispis();
		cout << endl;
	}
	void Dealociraj() {
		_datumIzvrsenja.Dealociraj();
		delete[] _nazivOpis; _nazivOpis = nullptr;
	}
};
struct KaratePojas {
	Datum * _datumPolaganja;//datum koji ce se evidentirati kao datum polaganja pojasa tj. kada su ispunjene sve aktivnosti/obaveze
	Pojas _pojas;
	Aktivnost * _listaIzvrsenihAktivnosti;
	int _trenutnoIzvrsenihAktivnosti;
	void Unos(Pojas pojas) {
		_listaIzvrsenihAktivnosti = nullptr;
		_trenutnoIzvrsenihAktivnosti = 0;
		_datumPolaganja = nullptr;
		_pojas = pojas;
	}
	bool DodajIzvrsenuAktivnost(Aktivnost *a) {
		if (_datumPolaganja != nullptr) return false; // Ako je datum polaganja evidentiran smatra se da je pojas uspjesno polozen
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
		{
			if (a->_datumIzvrsenja.GetDays() == _listaIzvrsenihAktivnosti[i]._datumIzvrsenja.GetDays() && *a->_vrsta == *_listaIzvrsenihAktivnosti[i]._vrsta && _listaIzvrsenihAktivnosti[i]._ocjena > 5)
				return false;
			if (*a->_vrsta == *_listaIzvrsenihAktivnosti[i]._vrsta && (a->_datumIzvrsenja.GetDays() - _listaIzvrsenihAktivnosti[i]._datumIzvrsenja.GetDays()) < 15)
				return false;
		}

		// Dodajemo izvrsenu aktivnost
		Aktivnost *temp = new Aktivnost[_trenutnoIzvrsenihAktivnosti + 1];
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++) {
			temp[i].Unos(*_listaIzvrsenihAktivnosti[i]._vrsta, &_listaIzvrsenihAktivnosti[i]._datumIzvrsenja, _listaIzvrsenihAktivnosti[i]._ocjena, _listaIzvrsenihAktivnosti[i]._nazivOpis);
			_listaIzvrsenihAktivnosti[i].Dealociraj();
		}
		delete[] _listaIzvrsenihAktivnosti;
		temp[_trenutnoIzvrsenihAktivnosti].Unos(*a->_vrsta, &a->_datumIzvrsenja, a->_ocjena, a->_nazivOpis);
		_listaIzvrsenihAktivnosti = temp;
		_trenutnoIzvrsenihAktivnosti++;
		return true;
	}
	void Sortiraj() {
		// Koristit cemo bubble sort
		bool prolaz = true;
		while (prolaz)
		{
			prolaz = false;
			for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti - 1; i++)
			{
				if (_listaIzvrsenihAktivnosti[i]._ocjena < _listaIzvrsenihAktivnosti[i + 1]._ocjena) {
					Aktivnost temp;
					temp.Unos(*_listaIzvrsenihAktivnosti[i]._vrsta, &_listaIzvrsenihAktivnosti[i]._datumIzvrsenja, _listaIzvrsenihAktivnosti[i]._ocjena, _listaIzvrsenihAktivnosti[i]._nazivOpis);
					_listaIzvrsenihAktivnosti[i].Dealociraj();
					
					_listaIzvrsenihAktivnosti[i].Unos(*_listaIzvrsenihAktivnosti[i + 1]._vrsta, &_listaIzvrsenihAktivnosti[i + 1]._datumIzvrsenja, _listaIzvrsenihAktivnosti[i + 1]._ocjena, _listaIzvrsenihAktivnosti[i + 1]._nazivOpis);
					_listaIzvrsenihAktivnosti[i + 1].Dealociraj();

					_listaIzvrsenihAktivnosti[i + 1].Unos(*temp._vrsta, &temp._datumIzvrsenja, temp._ocjena, temp._nazivOpis);
					temp.Dealociraj();
					prolaz = true;
				}
			}
		}
	}
	bool DaLiJePolozen() {
		int brojaci[3] = { 0 };
		int negativneOcjene[3] = { 0 };
		Datum d;
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
		{
			if (_listaIzvrsenihAktivnosti[i]._ocjena > 5) {
				brojaci[*_listaIzvrsenihAktivnosti[i]._vrsta]++;
				// Uzimamo datum posljednje izvrsene obaveze
				d.Unos(*_listaIzvrsenihAktivnosti[i]._datumIzvrsenja._dan, *_listaIzvrsenihAktivnosti[i]._datumIzvrsenja._mjesec, *_listaIzvrsenihAktivnosti[i]._datumIzvrsenja._godina);
			}
			if (_listaIzvrsenihAktivnosti[i]._ocjena == 1) {
				negativneOcjene[*_listaIzvrsenihAktivnosti[i]._vrsta]++;
			}
		}
		// provjera da li postoji neka obaveza sa 3 negativne ocjenei i da li ispunjava sve obaveze
		for (size_t i = 0; i < 3; i++)
		{
			if (negativneOcjene[i] > 2 || brojaci[i] < ((int)_pojas + 1))
				return false;
		}
		_datumPolaganja = new Datum;
		_datumPolaganja->Unos(*d._dan, *d._mjesec, *d._godina);
		return true;
	}
	float PretragaRekrzivno(const char *str, int i = 0, int brojacIzvrsenih = 0, float prosjek = 0) {
		if (i == _trenutnoIzvrsenihAktivnosti) {
			if (brojacIzvrsenih == 0) return 0;
			return prosjek / brojacIzvrsenih;
		}
		if (_listaIzvrsenihAktivnosti[i]._ocjena > 5 && strstr(_listaIzvrsenihAktivnosti[i]._nazivOpis, str) != nullptr)
			return PretragaRekrzivno(str, i + 1, brojacIzvrsenih + 1, prosjek + _listaIzvrsenihAktivnosti[i]._ocjena);
		return PretragaRekrzivno(str, i + 1, brojacIzvrsenih, prosjek);
	}
	void Kopiraj(KaratePojas p) {
		Unos(p._pojas);
		_trenutnoIzvrsenihAktivnosti = p._trenutnoIzvrsenihAktivnosti;
		_listaIzvrsenihAktivnosti = new Aktivnost[p._trenutnoIzvrsenihAktivnosti];
		for (size_t i = 0; i < p._trenutnoIzvrsenihAktivnosti; i++)
		{
			_listaIzvrsenihAktivnosti[i].Unos(*p._listaIzvrsenihAktivnosti[i]._vrsta, &p._listaIzvrsenihAktivnosti[i]._datumIzvrsenja, p._listaIzvrsenihAktivnosti[i]._ocjena, p._listaIzvrsenihAktivnosti[i]._nazivOpis);
		}
		if (p._datumPolaganja != nullptr)
			DaLiJePolozen();
	}
	void Dealociraj() {
		if (_datumPolaganja != nullptr) {
			_datumPolaganja->Dealociraj();
			delete _datumPolaganja;
		}
		_datumPolaganja = nullptr;
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			_listaIzvrsenihAktivnosti[i].Dealociraj();
		delete[] _listaIzvrsenihAktivnosti;
		_listaIzvrsenihAktivnosti = nullptr;
	}
	void Ispis() {
		if (_datumPolaganja != nullptr)
			_datumPolaganja->Ispis();
		cout << PojasChar[_pojas] << endl;
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			_listaIzvrsenihAktivnosti[i].Ispis();
	}
};

struct Kandidat {
	char * _imePrezime;
	shared_ptr<KaratePojas> _pojasevi[6];
	void Unos(const char * imePrezime) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		for (size_t i = 0; i < 6; i++)
			_pojasevi[i] = nullptr;
	}
	bool DodajPojas(KaratePojas p) {
		auto GetLokaciju = [this, p]() {
			for (int i = 0; i < 6; i++)
			{
				if (_pojasevi[i] == nullptr) {
					if (i == (int)p._pojas) {
						return i;
					}
					return -1;
				}
			}
			return -1;
		};
		int lokacija = GetLokaciju();
		if (lokacija != -1) {
			_pojasevi[lokacija] = make_shared<KaratePojas>();
			_pojasevi[lokacija]->Kopiraj(p);
			return true;
		}
		return false;
	}
	pair<Pojas, float> GetNajbolji() {
		Pojas p;
		float prosjek = 0;
		for (size_t i = 0; i < 6; i++)
		{
			if (_pojasevi[i] != nullptr) {
				float prosjekTrenutno = 0;
				int brojacUspjesnihObaveza = 0;
				for (size_t j = 0; j < _pojasevi[i]->_trenutnoIzvrsenihAktivnosti; j++)
				{
					if (_pojasevi[i]->_listaIzvrsenihAktivnosti[j]._ocjena > 5) {
						prosjekTrenutno += _pojasevi[i]->_listaIzvrsenihAktivnosti[j]._ocjena;
						brojacUspjesnihObaveza++;
					}
				}
				prosjekTrenutno /= brojacUspjesnihObaveza;
				if (prosjek < prosjekTrenutno) {
					prosjek = prosjekTrenutno;
					p = _pojasevi[i]->_pojas;
				}
			}
		}
		return pair<Pojas, float>(p, prosjek);
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < 6; i++)
			if (_pojasevi[i] != nullptr) {
				_pojasevi[i]->Dealociraj();
				_pojasevi[i].reset();
			}
	}
	void Ispis() {
		cout << _imePrezime << endl;
		for (size_t i = 0; i < 6; i++)
			if (_pojasevi[i] != nullptr)
				_pojasevi[i]->Ispis();
	}
};

void main() {

	//BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA
	//STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
	Datum datumPolaganja1, datumPolaganja2, datumPolaganja3, datumPolaganja4;
	datumPolaganja1.Unos(10, 6, 2018);
	datumPolaganja2.Unos(18, 6, 2018);
	datumPolaganja3.Unos(22, 3, 2018);
	datumPolaganja4.Unos(22, 7, 2018);

	Aktivnost aktivnost1, aktivnost2, aktivnost3, aktivnost4, aktivnost5;
	aktivnost1.Unos(Tehnike, &datumPolaganja1, 6, "Tehnike za zuti pojas");
	aktivnost2.Unos(Kata, &datumPolaganja1, 8, "Taiki joko shodan - zuti pojas");
	aktivnost3.Unos(Borba, &datumPolaganja1, 2, "Jednostavne borbene tehnike sa partnerom");
	aktivnost4.Unos(Borba, &datumPolaganja1, 6, "Jednostavne borbene tehnike sa partnerom");
	aktivnost5.Unos(Borba, &datumPolaganja4, 6, "Jednostavne borbene tehnike sa partnerom");

	KaratePojas pojasZuti, pojasNarandzasti, pojasZeleni;
	pojasZuti.Unos(Zuti);
	pojasNarandzasti.Unos(Narandzasti);
	pojasZeleni.Unos(Zeleni);

	/*Na osnovu vrijednosti primljenog parametra osigurati dodavanje novoizvrsene aktivnosti za potrebe stjecanja odredjenog pojasa. Broj aktivnosti nije ogranicen.
	Identicna aktivnost se moze dodati jedino u slucaju kada je prethodna (identivna aktivnost po vrsti i datumu izvrsenja) imala ocjenu manju od 6.
	Uspjesnom aktivnoscu se smatraju one aktivnosti koje imaju ocjenu vecu od 5, a svaka naredna identicna aktivnost, bez obzira da li je uspjesna ili ne,
	moze biti dodana jedino ako je proslo najmanje 15 dana od izvrsenja prethodne. Onemoguciti dodavanje aktivnosti uspjesno polozenom pojasu.*/
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost1))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost2))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost3))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost4))//15 dana...
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost5))
		cout << "Aktivnost uspjesno dodana!" << endl;

	//Koristeci neki od obradjenih algoritama, po ocjeni sortirati aktivnosti u okviru odredjenog pojasa
	pojasZuti.Sortiraj();


	/* Karate pojas zahtijeva uspjesnu realizaciju svih planiranih aktivnosti, a one se za jedan povecavaju sa svakim novim pojasom, npr.
	zuti pojas: 1 x tehnika, 1 x kata, 1 x borba;	narandzasti pojas: 2 x tehnika, 2 x kata, 2 x borba; i td...
	Funkcija vraca false u slucaju da: su kandidatu u listu aktivnosti evidentirane tri negativno ocijenjene identicne vrste aktivnosti (npr. tri negativne ocjene iz borbi), onda se taj pojas ne moze smatrati uspjesno stecenim
	i, te ukoliko nedostaje bilo koja od aktivnosti zahtijevanih u okviru tog pojasa. Ukoliko je kandidat uspjesno realizovao sve aktivnost, datum polaganja se postavlja na datum posljednje uspjesno realizovane aktivnosti*/
	if (pojasZuti.DaLiJePolozen())
		pojasZuti.Ispis();

	/*Funkcija vraca prosjecnu ocjenu svih uspjesno realizovanih aktivnosti koja u nazivu ili opisu sadrze vrijednost primljenog parametra. Ukoliko smatrate da je potrebno, mozete dodati i druge parametre za potrebe implementacije ove funkcije*/
	cout << "Prosjecna ocjena za zuti pojas -> " << pojasZuti.PretragaRekrzivno("pojas") << endl;

	//ispisuje sve dostupne podatke o pojasu
	pojasZuti.Ispis();

	Kandidat jasmin;
	jasmin.Unos("Jasmin Azemovic");
	/*
	Karate pojasevi se moraju dodavati po redoslijedu tj. ne smije se dozvoliti dodavanje zelenog pojasa ukoliko prethodno nije dodan zuti i narandzasti. Za provjeru lokacije (unutar funkcije DodajPojas) na koju ce se dodati novi karate pojas, te da li su nizi pojasevi prethodno dodani koristiti lambda funkciju.
	*/
	if (jasmin.DodajPojas(pojasZuti))
		cout << "Pojas uspjesno dodan!" << endl;
	if (jasmin.DodajPojas(pojasZeleni))//prethodno je trebao biti dodan narandzasti pojas
		cout << "Pojas uspjesno dodan!" << endl;
	if (jasmin.DodajPojas(pojasNarandzasti))
		cout << "Pojas uspjesno dodan!" << endl;

	float prosjek = 0;
	Pojas pojas;
	//Funkcija GetNajbolji vraca par koji sadrzi oznaku i prosjecnu ocjenu (uspjesno okoncanih aktivnosti) pojasa sa najvecim prosjekom
	tie(pojas, prosjek) = jasmin.GetNajbolji();
	cout << "Najbolji rezultat od " << prosjek << " je ostvaren tokom stjecanja pojasa " << pojas << endl;


	datumPolaganja1.Dealociraj(), datumPolaganja2.Dealociraj(), datumPolaganja3.Dealociraj(), datumPolaganja4.Dealociraj();
	aktivnost1.Dealociraj(), aktivnost2.Dealociraj(), aktivnost3.Dealociraj(), aktivnost4.Dealociraj(), aktivnost5.Dealociraj();
	pojasZuti.Dealociraj(), pojasNarandzasti.Dealociraj(), pojasZeleni.Dealociraj();
	jasmin.Dealociraj();

	system("pause");
}