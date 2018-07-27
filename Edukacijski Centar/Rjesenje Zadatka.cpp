#include <iostream>
using namespace std;
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

enum enumPredmeti { Matematika, Historija, Geografia, Hemija, Biologija, Engleski };
const char *enumPredmetiChar[] = { "Matematika", "Historija", "Geografia", "Hemija", "Biologija", "Engleski" };
enum enumRazredi { I, II, III, IV };
const char *crt = "\n--------------------------------------------\n";

struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() { delete _dan; delete _mjesec; delete _godina; }
	int GetDani() {
		return *_dan + *_mjesec * 30 + *_godina * 365;
	}
};

struct Kurs {
	enumPredmeti _predmet;
	enumRazredi _razredi;
	Datum * _pocetak;
	Datum * _kraj;
	char * _imePredavaca;
	void Unos(enumPredmeti predmet, enumRazredi razred, Datum pocetak, Datum kraj, const char * imePredavaca) {
		_pocetak = new Datum;
		_pocetak->Unos(*pocetak._dan, *pocetak._mjesec, *pocetak._godina);
		_kraj = new Datum;
		_kraj->Unos(*kraj._dan, *kraj._mjesec, *kraj._godina);
		int size = strlen(imePredavaca) + 1;
		_imePredavaca = new char[size];
		strcpy_s(_imePredavaca, size, imePredavaca);
		_predmet = predmet;
		_razredi = razred;
	}
	void Ispis() {
		cout << crt;
		cout << "Predmet -> " << enumPredmetiChar[_predmet] << endl;
		cout << "Razred -> " << _razredi << endl;
		cout << "Pocetak -> "; _pocetak->Ispis();
		cout << "Kraj -> "; _kraj->Ispis();
		cout << "Ime Predavaca -> " << _imePredavaca << endl;
		cout << crt;
	}
	void Dealociraj() {
		delete[] _imePredavaca; _imePredavaca = nullptr;
		_pocetak->Dealociraj();
		_kraj->Dealociraj();
		delete _pocetak; _pocetak = nullptr;
		delete _kraj; _kraj = nullptr;
	}
};
struct Polaznik {
	Datum * _datumRodjenja;
	char * _imePrezime;
	void Unos(Datum datumRodjenja, const char * imePrezime) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		_datumRodjenja = new Datum;
		_datumRodjenja->Unos(*datumRodjenja._dan, *datumRodjenja._mjesec, *datumRodjenja._godina);
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		_datumRodjenja->Dealociraj();
		delete _datumRodjenja; _datumRodjenja = nullptr;
	}
	void Ispis() {
		cout << "Datum Rodjenja -> "; _datumRodjenja->Ispis();
		cout << "Ime i prezime -> " << _imePrezime << endl;
	}
};
struct Polaganja {
	Polaznik * _polaznik;
	Kurs * _kurs;
	Datum * _datumPolaganja;
	int _ocjena; //ocjene su u rasponu od 1 do 5	
	void Unos(Polaznik p, Kurs k, Datum d, int ocjena) {
		_polaznik = new Polaznik;
		_polaznik->Unos(*p._datumRodjenja, p._imePrezime);
		_kurs = new Kurs;
		_kurs->Unos(k._predmet, k._razredi, *k._pocetak, *k._kraj, k._imePredavaca);
		_datumPolaganja = new Datum;
		_datumPolaganja->Unos(*d._dan, *d._mjesec, *d._godina);
		_ocjena = ocjena;
	}

	void Dealociraj() {
		_polaznik->Dealociraj();
		delete _polaznik; _polaznik = nullptr;
		_kurs->Dealociraj();
		delete _kurs; _kurs = nullptr;
		_datumPolaganja->Dealociraj();
		delete _datumPolaganja; _datumPolaganja = nullptr;
	}

	void Ispis() {
		cout << crt;
		cout << "Polaznik -> "; _polaznik->Ispis();
		cout << "Kurs -> "; _kurs->Ispis();
		cout << "Datum polaganja -> "; _datumPolaganja->Ispis();
		cout << "Ocjena -> " << _ocjena << endl;
		cout << crt;
	}
};

struct EdukacijskiCentar {
	char * _nazivCentra;
	Kurs * _kursevi[50];//JEDAN EDUKACIJSKI CENTAR MOZE NUDITI NAJVISE 50 KURSEVA
	int _trenutnoKurseva;
	Polaganja * _polaganja;
	int _trenutnoPolaganja;

	void Unos(const char *naziv) {
		int size = strlen(naziv) + 1;
		_nazivCentra = new char[size];
		strcpy_s(_nazivCentra, size, naziv);
		_trenutnoKurseva = 0;
		_trenutnoPolaganja = 0;
		_polaganja = nullptr;
	}

	bool DodajKurs(Kurs k) {
		for (size_t i = 0; i < _trenutnoKurseva; i++)
		{
			if (_kursevi[i]->_kraj->GetDani() > k._pocetak->GetDani())
				return false;
		}
		_kursevi[_trenutnoKurseva] = new Kurs;
		_kursevi[_trenutnoKurseva++]->Unos(k._predmet, k._razredi, *k._pocetak, *k._kraj, k._imePredavaca);
		return true;
	}

	bool DodajPolaganje(Polaganja p) {
		bool postojiKurs = false;
		for (size_t i = 0; i < _trenutnoKurseva; i++)
		{
			if (_kursevi[i]->_predmet == p._kurs->_predmet && (p._datumPolaganja->GetDani() - _kursevi[i]->_kraj->GetDani()) < 15) {
				postojiKurs = true;
				break;
			}
		}
		if (!postojiKurs) return false;
		for (size_t i = 0; i < _trenutnoPolaganja; i++)
		{
			if (strcmp(_polaganja[i]._polaznik->_imePrezime, p._polaznik->_imePrezime) == 0 &&
				_polaganja[i]._kurs->_predmet == p._kurs->_predmet &&
				_polaganja[i]._ocjena > p._ocjena) {

				return false;

			}
		}

		Polaganja *temp = new Polaganja[_trenutnoPolaganja + 1];
		for (size_t i = 0; i < _trenutnoPolaganja; i++)
		{
			temp[i].Unos(*_polaganja[i]._polaznik, *_polaganja[i]._kurs, *_polaganja[i]._datumPolaganja, _polaganja[i]._ocjena);
			_polaganja[i].Dealociraj();
		}
		delete[] _polaganja;
		temp[_trenutnoPolaganja].Unos(*p._polaznik, *p._kurs, *p._datumPolaganja, p._ocjena);
		_polaganja = temp;
		_trenutnoPolaganja++;
		return true;
	}

	Polaganja *PolaganjaByDatumIRazred(Datum OD, Datum DO, enumRazredi razred, int &brojPolaganja) {
		brojPolaganja = 0;
		for (size_t i = 0; i < _trenutnoPolaganja; i++)
		{
			if (_polaganja[i]._datumPolaganja->GetDani() > OD.GetDani() && DO.GetDani() > _polaganja[i]._datumPolaganja->GetDani() && _polaganja[i]._kurs->_razredi == razred && _polaganja[i]._kurs->_razredi == razred && _polaganja[i]._ocjena > 1) {
				brojPolaganja++;
			}
		}
		if (brojPolaganja > 0) {
			Polaganja *polaganja = new Polaganja[brojPolaganja];
			brojPolaganja = 0;
			for (size_t i = 0; i < _trenutnoPolaganja; i++)
			{
				if (_polaganja[i]._datumPolaganja->GetDani() >= OD.GetDani() && DO.GetDani() > _polaganja[i]._datumPolaganja->GetDani() && _polaganja[i]._kurs->_razredi == razred && _polaganja[i]._ocjena > 1) {
					polaganja[brojPolaganja++].Unos(*_polaganja[i]._polaznik, *_polaganja[i]._kurs, *_polaganja[i]._datumPolaganja, _polaganja[i]._ocjena);
				}
			}
			return polaganja;
		}
		return nullptr;
	}

	void Dealociraj() {
		for (size_t i = 0; i < _trenutnoKurseva; i++)
		{
			_kursevi[i]->Dealociraj();
			delete _kursevi[i];
			_kursevi[i] = nullptr;
		}
		for (size_t i = 0; i < _trenutnoPolaganja; i++)
		{
			_polaganja[i].Dealociraj();
		}
		delete[] _polaganja; _polaganja = nullptr;
		delete[] _nazivCentra; _nazivCentra = nullptr;
	}

	void Ispis() {
		cout << "Naziv edukacijskog centra -> " << _nazivCentra << endl;
		cout << "Kursevi koje nudi edukacijski centar: " << endl << crt;
		for (size_t i = 0; i < _trenutnoKurseva; i++)
			_kursevi[i]->Ispis();
		cout << "Polaganja: " << endl << crt;
		for (size_t i = 0; i < _trenutnoPolaganja; i++)
			_polaganja[i].Ispis();
	}
};

float PretragaRekurzivno(EdukacijskiCentar centar, enumPredmeti predmet, int godina, int brojacPolaganja = 0, float sumaUspjeha = 0, int brojUspjeha = 0) {
	if (centar._trenutnoPolaganja == brojacPolaganja) {
		if (brojUspjeha == 0) return 0;
		return (float)sumaUspjeha / brojUspjeha;
	}
	if (centar._polaganja[brojacPolaganja]._kurs->_predmet == predmet && *centar._polaganja[brojacPolaganja]._datumPolaganja->_godina == godina)
		return PretragaRekurzivno(centar, predmet, godina, brojacPolaganja + 1, sumaUspjeha + centar._polaganja[brojacPolaganja]._ocjena, ++brojUspjeha);
	return PretragaRekurzivno(centar, predmet, godina, brojacPolaganja += 1, sumaUspjeha, brojUspjeha);
}

void main() {

	/*
	1. BROJ I TIPOVI PARAMETARA MORAJU BITI IDENTICNI ONIMA U TESTNOM CODE-U (OSIM UKOLIKO IMATE OPRAVDANU POTREBU ZA MODIFIKACIJOM). U SUPROTNOM SE RAD NEÆE BODOVATI
	2. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA.
	NE PREDAVATI .TXT ILI .CPP FAJLOVE. TAKOÐER, UKLONITE SVE DIJELOVE CODE-A KOJI NISU IMPLEMENTIRANI
	3. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
	4. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ÆETE KOPIRATI VAŠA RJEŠENJA)
	5. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
	*/

	Datum datum1, datum2, datum3, datum4, datum5, datum6;
	datum1.Unos(26, 11, 2015);
	datum2.Unos(29, 5, 2016);
	datum3.Unos(5, 6, 2016);
	datum4.Unos(15, 8, 2016);
	datum5.Unos(13, 7, 2016);
	datum6.Unos(22, 9, 2016);

	Kurs matematika, hemija, biologija, engleski;
	matematika.Unos(Matematika, III, datum1, datum2, "Nina Bijedic");
	hemija.Unos(Hemija, III, datum2, datum3, "Murat Praso");
	biologija.Unos(Biologija, IV, datum3, datum4, "Jasmin Azemovic");
	engleski.Unos(Engleski, II, datum3, datum4, "Iris Memic");

	Datum rodjenje1, rodjenje2;
	rodjenje1.Unos(22, 8, 1982);
	rodjenje2.Unos(22, 8, 1982);
	Polaznik denis, zanin, indira;
	denis.Unos(rodjenje1, "Denis Music");
	zanin.Unos(rodjenje2, "Zanin Vejzovic");


	Polaganja denisMatematika, zaninHemija, denisBiologija;
	denisMatematika.Unos(denis, matematika, datum5, 3);//3 predstavlja ostvareni uspjeh/ocjenu, a uspjesno polozenim se smatra svaki kurs na kome je polaznik ostvario ocjenu veæu od 1
	zaninHemija.Unos(zanin, hemija, datum6, 93);

	EdukacijskiCentar mostar;
	mostar.Unos("ECM - Edukacijski Center Mostar");
	//jedan edukacijski centar moze nuditi vise istih kurseva (npr. Matematika), ali se oni moraju realizovati u razlictim periodima (pocetak-kraj) tj. ne smiju se preklapati
	//novi kurs istog tipa (npr. Matematika) se mora realizovati nakon prethodno dodanog npr. ako prethodni kurs Matematika traje 01.03.2016 - 01.04.2016, onda se novi kurs Matematika moze dodati jedino ako pocinje nakon 01.04.2016
	if (mostar.DodajKurs(matematika)) {
		cout << crt << "KURS USPJESNO REGISTROVAN" << crt;
		matematika.Ispis();
		cout << crt;
	}
	//nije moguce dodati polaganje onog kursa koji nije evidentiran (registrovan, ponudjen) u tom edukacijskom centru
	//kurseve je moguce najkasnije 15 dana od dana zavrsetka (kraja) posljednje dodanog kursa
	//jedan polaznik moze vise puta polagati isti kurs, ali nova ocjena mora biti veca od prethodne ocjene na tom istom kursu
	if (mostar.DodajPolaganje(denisMatematika)) {
		cout << crt << "POLAGANJE EVIDENTIRANO" << crt;
		denisMatematika.Ispis();
	}
	Datum OD, DO;
	int brojPolaganja = 0;
	OD.Unos(1, 6, 2016);
	DO.Unos(1, 8, 2016);
	//PolaganjaByDatumIRazred - funkcija vraca niz uspjesnih polaganja koja su realizovana u periodu OD - DO, a odnose se na bilo koji predmet u definisanom razredu (npr. II)
	Polaganja * polaganjaByDatumiRazred = mostar.PolaganjaByDatumIRazred(OD, DO, II, brojPolaganja);
	cout << "U periodu od ";
	OD.Ispis();
	cout << " do ";
	DO.Ispis();
	cout << " uspjesno je polozeno " << brojPolaganja << " predmeta --->>> ";
	for (size_t i = 0; i < brojPolaganja; i++)
		polaganjaByDatumiRazred[i].Ispis();

	//PretragaRekurzivno - rekurzivna funkcija koja pronalazi prosjecni uspjeh koji su polaznici tokom godine (npr.2016) ostvarili na odredjenom kursu (npr. Matematika)
	cout << "Prosjecan uspjeh na kursu iz predmeta Matematika u 2016 godini je " << PretragaRekurzivno(mostar, Matematika, 2016) << endl;
	//ispisuje sve informacije o edukacijskom centru. prije ispisa sortirati sve kurseve
	mostar.Ispis();

	//izvrsiti potrebne dealokacije
	datum1.Dealociraj(); datum2.Dealociraj(); datum3.Dealociraj(); datum4.Dealociraj(); datum5.Dealociraj(); datum6.Dealociraj();
	matematika.Dealociraj(); hemija.Dealociraj(); biologija.Dealociraj(); engleski.Dealociraj();
	rodjenje1.Dealociraj(); rodjenje2.Dealociraj();
	OD.Dealociraj(); DO.Dealociraj();
	denis.Dealociraj(); zanin.Dealociraj();
	denisMatematika.Dealociraj(); zaninHemija.Dealociraj();
	mostar.Dealociraj();
	if (polaganjaByDatumiRazred != nullptr) {
		for (size_t i = 0; i < brojPolaganja; i++)
			polaganjaByDatumiRazred[i].Dealociraj();
		delete[] polaganjaByDatumiRazred; polaganjaByDatumiRazred = nullptr;
	}
	system("pause");
}