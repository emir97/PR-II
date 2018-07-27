#include <iostream>

using namespace std;

//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

enum enumKursevi { HtmlCSSJavaScript, SoftwareEngeneeringFundamentals, MasteringSQL, WindowsSecurity };
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
};

struct Kurs {
	enumKursevi _kurs;
	Datum _pocetak;
	Datum _kraj;
	char * _imePredavaca;
	bool _aktivan;//SVAKI KURS JE NA POCETKU AKTIVAN
	void Unos(enumKursevi kurs, Datum OD, Datum DO, const char *predavac) {
		_kurs = kurs;
		_pocetak.Unos(*OD._dan, *OD._mjesec, *OD._godina);
		_kraj.Unos(*DO._dan, *DO._mjesec, *DO._godina);
		int size = strlen(predavac) + 1;
		_imePredavaca = new char[size];
		strcpy_s(_imePredavaca, size, predavac);
		_aktivan = true;
	}
	void Dealociraj() {
		delete[] _imePredavaca; _imePredavaca = nullptr;
		_pocetak.Dealociraj();
		_kraj.Dealociraj();
	}
	void Ispis() {
		cout << "Datum pocetka -> ";
		_pocetak.Ispis();
		cout << "Datum kraja -> ";
		_kraj.Ispis();
		cout << "Predavac -> " << _imePredavaca << endl;
	}
};
struct Polaznik {
	int _polaznikID;
	char * _imePrezime;
	void Unos(int polaznikID, const char * imePrezime) {
		_polaznikID = polaznikID;
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
	}
	void Ispis() { cout << _polaznikID << " " << _imePrezime << endl; }
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
};
struct Polaganja {
	Polaznik _polaznik;
	Kurs _kurs;
	Datum _datumPolaganja;
	float _ostvareniUspjeh;
	void Unos(Polaznik p, Kurs k, Datum d, float uspjeh) {
		_polaznik.Unos(p._polaznikID, p._imePrezime);
		_kurs.Unos(k._kurs, k._pocetak, k._kraj, k._imePredavaca);
		_datumPolaganja.Unos(*d._dan, *d._mjesec, *d._godina);
		_ostvareniUspjeh = uspjeh;
	}
	void Dealociraj() {
		_polaznik.Dealociraj();
		_kurs.Dealociraj();
		_datumPolaganja.Dealociraj();
	}
	void Ispis() {
		cout << "Polaznik -> ";
		_polaznik.Ispis();
		cout << "Kurs -> ";
		_kurs.Ispis();
		cout << "Datum polaganja -> ";
		_datumPolaganja.Ispis();
		cout << "Ostavareni uspjeh -> " << _ostvareniUspjeh << endl;
	}
};
struct SkillsCentar {
	char * _nazivCentra;
	Kurs * _kursevi[50];//JEDAN EDUKACIJSKI CENTAR MOZE NUDITI NAJVISE 50 KURSEVA
	int _trenutnoKurseva;
	Polaganja * _polaganja;
	int _trenutnoPolaganja;
	void Unos(const char *ime_centra) {
		int size = strlen(ime_centra) + 1;
		_nazivCentra = new char[size];
		strcpy_s(_nazivCentra, size, ime_centra);
		_trenutnoKurseva = 0;
		_polaganja = nullptr;
		_trenutnoPolaganja = 0;
	}
	int PretvoriUDane(Datum d) {
		return *d._godina * 365 + *d._mjesec * 30 + *d._dan;
	}
	bool DodajKurs(Kurs k) {
		for (int i = 0; i < _trenutnoKurseva; i++)
		{
			if (_kursevi[i]->_kurs == k._kurs && PretvoriUDane(_kursevi[i]->_kraj) > PretvoriUDane(k._pocetak))
				return false;
		}
		_kursevi[_trenutnoKurseva] = new Kurs;
		_kursevi[_trenutnoKurseva++]->Unos(k._kurs, k._pocetak, k._kraj, k._imePredavaca);
		return true;
	}
	bool DodajPolaganje(Polaganja p) {
		bool moze_polagat = false;
		for (size_t i = 0; i < _trenutnoKurseva; i++)
		{
			if (_kursevi[i]->_kurs == p._kurs._kurs && _kursevi[i]->_aktivan && (PretvoriUDane(p._datumPolaganja) - PretvoriUDane(_kursevi[i]->_kraj)) < 5) {
				moze_polagat = true;
				break;
			}
		}
		if (!moze_polagat) return false;
		for (int i = 0; i < _trenutnoPolaganja; i++)
		{
			if (_polaganja[i]._kurs._kurs == p._kurs._kurs && _polaganja[i]._ostvareniUspjeh > p._ostvareniUspjeh)
				return false;
		}

		Polaganja *temp = new Polaganja[_trenutnoPolaganja + 1];
		for (size_t i = 0; i < _trenutnoPolaganja; i++)
		{
			temp[i].Unos(_polaganja[i]._polaznik, _polaganja[i]._kurs, _polaganja[i]._datumPolaganja, _polaganja[i]._ostvareniUspjeh);
			_polaganja[i].Dealociraj();
		}
		if (_polaganja != nullptr)
			delete[] _polaganja;
		temp[_trenutnoPolaganja].Unos(p._polaznik, p._kurs, p._datumPolaganja, p._ostvareniUspjeh);
		_polaganja = temp;
		_trenutnoPolaganja++;
		return true;
	}
	void Ispis() {
		cout << "Naziv centra -> " << _nazivCentra << endl;
		cout << "Kurseve koje nudi: ";
		for (int i = 0; i < _trenutnoKurseva; i++)
			_kursevi[i]->Ispis();

	}
	void Dealociraj() {
		delete[] _nazivCentra; _nazivCentra = nullptr;
		for (int i = 0; i < _trenutnoKurseva; i++)
		{
			_kursevi[i]->Dealociraj();	_kursevi[i] = nullptr;
		}
		for (int i = 0; i < _trenutnoPolaganja; i++)
			_polaganja[i].Dealociraj();
		delete[] _polaganja; _polaganja = nullptr;
	}
	Polaganja *PolaganjaByDatum(Datum OD, Datum DO, int &broj_polaganja) {
		broj_polaganja = 0;
		Polaganja *niz_polaganja = new Polaganja[_trenutnoPolaganja];
		for (size_t i = 0; i < _trenutnoPolaganja; i++)
		{
			if (PretvoriUDane(OD)< PretvoriUDane(_polaganja[i]._datumPolaganja) && PretvoriUDane(_polaganja[i]._datumPolaganja)<PretvoriUDane(DO))
			{
				niz_polaganja[broj_polaganja++].Unos(_polaganja[i]._polaznik, _polaganja[i]._kurs, _polaganja[i]._datumPolaganja, _polaganja[i]._ostvareniUspjeh);
			}
		}
		if (broj_polaganja == 0) return nullptr;
		return niz_polaganja;
	}
};
int PretragaRekurzivno(SkillsCentar centar, enumKursevi kurs, int brojac = 0, int suma = 0, int brojKurseva = 0) {
	if (centar._trenutnoPolaganja == brojac) {
		if (brojKurseva == 0) return 0;
		return (float)suma / brojKurseva;
	}
	if (centar._polaganja[brojac]._kurs._kurs == kurs)
		return PretragaRekurzivno(centar, kurs, brojac + 1, centar._polaganja[brojac]._ostvareniUspjeh + suma, brojKurseva + 1);
	return PretragaRekurzivno(centar, kurs, brojac + 1, suma, brojKurseva);
}
void main() {

	/*
	1. BROJ I TIPOVI PARAMETARA MORAJU BITI IDENTICNI ONIMA U TESTNOM CODE-U (OSIM UKOLIKO IMATE OPRAVDANU POTREBU ZA MODIFIKACIJOM). U SUPROTNOM SE RAD NEĆE BODOVATI
	2. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE. TAKOĐER, UKLONITE SVE DIJELOVE CODE-A KOJI NISU IMPLEMENTIRANI
	3. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
	4. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ĆETE KOPIRATI VAŠA RJEŠENJA)
	5. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
	*/

	Datum datum1, datum2, datum3, datum4, datum5, datum6;
	datum1.Unos(26, 11, 2015);
	datum2.Unos(29, 5, 2016);
	datum3.Unos(5, 6, 2016);
	datum4.Unos(15, 8, 2016);
	datum5.Unos(13, 7, 2016);
	datum6.Unos(22, 9, 2016);

	Kurs softverski, html, sql, security;
	softverski.Unos(SoftwareEngeneeringFundamentals, datum1, datum2, "Emina Junuz");
	html.Unos(HtmlCSSJavaScript, datum2, datum3, "Larisa Tipura");
	sql.Unos(MasteringSQL, datum3, datum4, "Jasmin Azemovic");
	security.Unos(WindowsSecurity, datum3, datum4, "Adel Handzic");

	Polaznik denis, zanin, indira;
	denis.Unos(1, "Denis Music");
	zanin.Unos(2, "Zanin Vejzovic");
	indira.Unos(3, "Indira Hamulic");

	Polaganja denisHtml, zaninHtml, indiraSql;
	denisHtml.Unos(denis, html, datum5, 61);
	//61 predstavlja ostvareni uspjeh/procenat, a uspjesno polozenim se smatra svaki kurs na kome je polaznik ostvari vise od 55%
	zaninHtml.Unos(zanin, html, datum6, 93);

	SkillsCentar mostar;
	mostar.Unos("Skills Center Mostar");
	//jedan edukacijski centar moze nuditi vise istih kurseva (npr. MasteringSQL), ali se oni moraju realizovati u razlictim periodima (pocetak-kraj) tj. ne smiju se preklapati
	//novi kurs istog tipa (npr. MasteringSQL) se mora realizovati nakon prethodno dodanog npr. ako prethodni kurs MasteringSQL traje 01.03.2016 - 01.04.2016, onda se novi kurs MasteringSQL moze dodati jedino ako pocinje nakon 01.04.2016
	if (mostar.DodajKurs(softverski)) {
		cout << crt << "KURS USPJESNO REGISTROVAN" << crt;
		softverski.Ispis();
		cout << crt;
	}
	if (mostar.DodajKurs(html)) {
		cout << crt << "KURS USPJESNO REGISTROVAN" << crt;
		softverski.Ispis();
		cout << crt;
	}
	//nije moguce dodati polaganje onog kursa koji nije evidentiran (registrovan, ponudjen) u tom edukacijskom centru
	//moguce je polagati samo aktivne kurseve i to najkasnije 5 dana od dana zavrsetka (kraja) posljednje dodanog (aktivnog) kursa iste vrste (npr. MasteringSQL)
	//jedan polaznik moze vise puta polagati isti kurs, ali novi procenat mora biti veci od prethodno dodanog (na istom kursu)
	if (mostar.DodajPolaganje(denisHtml)) {
		cout << crt << "POLAGANJE EVIDENTIRANO" << crt;
		denisHtml.Ispis();
	}
	Datum OD, DO;
	int brojPolaganja = 0;
	OD.Unos(1, 6, 2016);
	DO.Unos(1, 8, 2016);
	//PolaganjaByDatum – vraca niz polaganja koja su uspjesno realizovana u periodu OD-DO
	Polaganja * polaganjaByDatum = mostar.PolaganjaByDatum(OD, DO, brojPolaganja);
	cout << "U periodu od ";
	OD.Ispis();
	cout << " do ";
	DO.Ispis();
	cout << " uspjesno je realizovano " << brojPolaganja << " polaganja--->>>";
	for (size_t i = 0; i < brojPolaganja; i++)
		polaganjaByDatum[i].Ispis();

	//PretragaRekurzivno - rekurzivna funkcija pronalazi prosjecni uspjeh koji su polaznici tokom godine (npr.2016) ostvarili na odredjenom kursu (npr. MasteringSQL)
	cout << "Prosjecan uspjeh na kursu MasteringSQL u 2016 godini je " << PretragaRekurzivno(mostar, MasteringSQL) << endl;
	//Ispis - ispisuje sve informacije o edukacijskom centru. prije ispisa sortirati sve kurseve na osnovu pocetka odrzavanja kursa
	mostar.Ispis();

	//izvrsiti potrebne dealokacije
	// dealokacija datuma
	datum1.Dealociraj(); datum2.Dealociraj(); datum3.Dealociraj(); datum4.Dealociraj();	datum5.Dealociraj(); datum6.Dealociraj();
	OD.Dealociraj(); DO.Dealociraj();
	// dealokacija kurseva
	softverski.Dealociraj(); html.Dealociraj(); sql.Dealociraj(); security.Dealociraj();
	// dealokacija polaznika
	denis.Dealociraj(); indira.Dealociraj(); zanin.Dealociraj();
	// dealokacija polaganja
	denisHtml.Dealociraj(); zaninHtml.Dealociraj();
	for (size_t i = 0; i < brojPolaganja; i++)
		polaganjaByDatum[i].Dealociraj();
	delete[] polaganjaByDatum; polaganjaByDatum = nullptr;
	// dealokacija skill centra
	mostar.Dealociraj();
	system("pause");
}