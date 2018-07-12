#include <iostream>

using namespace std;
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

enum enumKursevi { HtmlCSSJavaScript, SoftwareEngeneeringFundamentals, MasteringSQL, WindowsSecurity };
const char *enumKurseviChar[] = { "HtmlCSSJavaScript", "SoftwareEngeneeringFundamentals", "MasteringSQL", "WindowsSecurity" };
const char *crt = "\n--------------------------------------------\n";

struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina; }
	void Dealociraj() { delete _dan; delete _mjesec; delete _godina; }
	int GetDays() { return *_dan + *_mjesec * 30 + *_godina * 365; }
};

struct Kurs {
	enumKursevi _kurs;
	Datum * _pocetak;
	Datum * _kraj;
	char * _imePredavaca;
	float _cijenaKursa;

	void Unos(enumKursevi kurs, Datum pocetak, Datum kraj, const char * imePredavaca, float cijenaKursa) {
		_kurs = kurs;
		_pocetak = new Datum;
		_pocetak->Unos(*pocetak._dan, *pocetak._mjesec, *pocetak._godina);
		_kraj = new Datum;
		_kraj->Unos(*kraj._dan, *kraj._mjesec, *kraj._godina);
		int size = strlen(imePredavaca) + 1;
		_imePredavaca = new char[size];
		strcpy_s(_imePredavaca, size, imePredavaca);
		_cijenaKursa = cijenaKursa;
	}
	void Ispis() {
		cout << _kurs << " " << _imePredavaca << " " << _cijenaKursa << " (";
		_pocetak->Ispis(); cout << " - "; _kraj->Ispis(); cout << ")";
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
	char * _imePrezime;
	Datum * _datumRodjenja;
	void Unos(const char * imePrezime, Datum datumRodjenja) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		_datumRodjenja = new Datum;
		_datumRodjenja->Unos(*datumRodjenja._dan, *datumRodjenja._mjesec, *datumRodjenja._godina);
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		delete _datumRodjenja; _datumRodjenja = nullptr;
	}
	void Ispis() { cout << _imePrezime << "("; _datumRodjenja->Ispis(); cout << ")"; }
};
struct Prijava {
	Polaznik * _polaznik; //prijava moze biti pojedinacna ili grupna
	float * _uplaceniIznos;
	/*za svakog polaznika ce se cuvati podatak o iznosu uplate. uplaceni iznos svakog pojedinog polaznika treba biti jednak punoj cijeni kursa osim u sljedecim slucajevima:
	1. prijave 2 ili 3 polaznika dobijaju 10% popusta na ukupnu cijenu kursa
	2. prijave za vise od 3 polaznika dobijaju 15% na ukupnu cijenu kursa
	*/
	int _trenutnoPolaznika;
	Kurs * _kurs; //prijava treba cuvati adresu (pokazivati) na postojeci kurs, a ne kreirati novi	
	void Unos(Kurs *k) {
		_polaznik = nullptr;
		_trenutnoPolaznika = 0;
		_uplaceniIznos = nullptr;
		_kurs = k;
	}
	bool DodajPolaznika(Polaznik p, float uplata) {
		for (size_t i = 0; i < _trenutnoPolaznika; i++)
		{
			if (strcmp(_polaznik[i]._imePrezime, p._imePrezime) == 0)
				return false;
		}
		float potrebnoUplatiti = _kurs->_cijenaKursa;
		if (_trenutnoPolaznika >= 2 && _trenutnoPolaznika <= 3)
			potrebnoUplatiti -= potrebnoUplatiti * 0.1; // 10 % popusta
		else if (_trenutnoPolaznika > 3)
			potrebnoUplatiti -= potrebnoUplatiti * 0.15; // 15 % popusta

		if (uplata < potrebnoUplatiti / 2) return false;

		Polaznik *temp = new Polaznik[_trenutnoPolaznika + 1];
		float *tempUplata = new float[_trenutnoPolaznika + 1];
		for (size_t i = 0; i < _trenutnoPolaznika; i++)
		{
			temp[i].Unos(_polaznik[i]._imePrezime, *_polaznik[i]._datumRodjenja);
			tempUplata[i] = _uplaceniIznos[i];
			_polaznik[i].Dealociraj();
		}
		delete[] _polaznik;
		delete[] _uplaceniIznos;
		temp[_trenutnoPolaznika].Unos(p._imePrezime, *p._datumRodjenja);
		tempUplata[_trenutnoPolaznika] = uplata;
		_polaznik = temp;
		_uplaceniIznos = tempUplata;
		_trenutnoPolaznika++;
		return true;
	}

	bool DodajUplatu(Polaznik p, float iznosUplate) {
		for (size_t i = 0; i < _trenutnoPolaznika; i++)
		{
			if (strcmp(p._imePrezime, _polaznik[i]._imePrezime) == 0) {
				_uplaceniIznos[i] += iznosUplate;
				return true;
			}
		}
		return false;
	}

	float StatusUplate(Polaznik p) {
		float potrebnoUplatiti = _kurs->_cijenaKursa;
		if (_trenutnoPolaznika >= 2 && _trenutnoPolaznika <= 3)
			potrebnoUplatiti -= potrebnoUplatiti * 0.1; // 10 % popusta
		else if (_trenutnoPolaznika > 3)
			potrebnoUplatiti -= potrebnoUplatiti * 0.15; // 15 % popusta
		for (size_t i = 0; i < _trenutnoPolaznika; i++)
		{
			if (strcmp(p._imePrezime, _polaznik[i]._imePrezime) == 0) {
				if (_uplaceniIznos[i] < potrebnoUplatiti) return (potrebnoUplatiti - _uplaceniIznos[i]) * -1;
				else return (potrebnoUplatiti - _uplaceniIznos[i]);
			}
		}
		return 0;
	}
	void Ispis() {
		cout << "Kurs -> ";
		_kurs->Ispis();
		cout << crt << endl;
		for (size_t i = 0; i < _trenutnoPolaznika; i++)
		{
			cout << "Polaznik -> ";
			_polaznik[i].Ispis();
			cout << "Uplaceni iznos -> " << _uplaceniIznos[i] << endl;
			cout << crt;
		}
		cout << crt;
	}
	void Kopiraj(Prijava &p) {
		_kurs = p._kurs;
		_polaznik = new Polaznik[p._trenutnoPolaznika];
		_uplaceniIznos = new float[p._trenutnoPolaznika];
		for (size_t i = 0; i < p._trenutnoPolaznika; i++)
		{
			_polaznik[i].Unos(p._polaznik[i]._imePrezime, *p._polaznik[i]._datumRodjenja);
			_uplaceniIznos[i] = p._uplaceniIznos[i];
		}
		_trenutnoPolaznika = p._trenutnoPolaznika;
	}
	void Dealociraj() {
		for (size_t i = 0; i < _trenutnoPolaznika; i++)
			_polaznik[i].Dealociraj();
		delete[] _polaznik; _polaznik = nullptr;
		delete[] _uplaceniIznos; _uplaceniIznos = nullptr;
	}
};

struct SkillsCentar {
	char * _nazivCentra;
	Kurs * _kursevi[50];//JEDAN EDUKACIJSKI CENTAR MOZE NUDITI NAJVISE 50 KURSEVA
	int _trenutnoKurseva;
	Prijava * _prijave;
	int _trenutnoPrijava;
	void Unos(const char *naziv) {
		int size = strlen(naziv) + 1;
		_nazivCentra = new char[size];
		strcpy_s(_nazivCentra, size, naziv);
		_trenutnoKurseva = 0;
		_trenutnoPrijava = 0;
		_prijave = nullptr;
	}
	bool DodajKurs(Kurs k) {
		for (size_t i = 0; i < _trenutnoKurseva; i++)
		{
			if (_kursevi[i]->_kurs == k._kurs && (k._pocetak->GetDays() - _kursevi[i]->_kraj->GetDays()) < 15)
				return false;
		}
		_kursevi[_trenutnoKurseva] = new Kurs;
		_kursevi[_trenutnoKurseva]->Unos(k._kurs, *k._pocetak, *k._kraj, k._imePredavaca, k._cijenaKursa);
		_trenutnoKurseva++;
		return true;
	}
	bool DodajPrijavu(Prijava p) {
		bool postoji = false;
		for (size_t i = 0; i < _trenutnoKurseva; i++)
		{
			if (_kursevi[i]->_kurs == p._kurs->_kurs)
				postoji = true;
		}
		if (!postoji) return false;
		float ukupnaUplata = p._kurs->_cijenaKursa;
		if (p._trenutnoPolaznika >= 2 && p._trenutnoPolaznika <= 3)
			ukupnaUplata -= ukupnaUplata * 0.1; // 10 % popusta
		else if (p._trenutnoPolaznika > 3)
			ukupnaUplata -= ukupnaUplata * 0.15; // 15 % popusta
		ukupnaUplata = (p._trenutnoPolaznika * ukupnaUplata)*0.8; // 80% ukupne uplate za prijavu
		float uplataPolaznika = 0;
		for (size_t i = 0; i < p._trenutnoPolaznika; i++)
			uplataPolaznika += p._uplaceniIznos[i];
		if (uplataPolaznika < ukupnaUplata) return false;

		Prijava *temp = new Prijava[_trenutnoPrijava + 1];
		for (size_t i = 0; i < _trenutnoPrijava; i++)
		{
			temp[i].Kopiraj(_prijave[i]);
			_prijave[i].Dealociraj();
		}
		delete[] _prijave;
		temp[_trenutnoPrijava].Kopiraj(p);
		_prijave = temp;
		_trenutnoPrijava++;
		return true;
	}
	Polaznik *PolazniciDugovanja(int &brojac) {
		brojac = 0;
		
		for (size_t i = 0; i < _trenutnoPrijava; i++)
		{
			float potrebnoUplatiti = _prijave[i]._kurs->_cijenaKursa;
			if (_prijave[i]._trenutnoPolaznika >= 2 && _prijave[i]._trenutnoPolaznika <= 3)
				potrebnoUplatiti -= potrebnoUplatiti * 0.1; // 10 % popusta
			else if (_prijave[i]._trenutnoPolaznika > 3)
				potrebnoUplatiti -= potrebnoUplatiti * 0.15; // 15 % popusta

			for (size_t j = 0; j < _prijave[i]._trenutnoPolaznika; j++)
			{
				if (_prijave[i]._uplaceniIznos[j] < potrebnoUplatiti)
					brojac++;
			}
		}
		Polaznik *p = new Polaznik[brojac];
		brojac = 0;
		for (size_t i = 0; i < _trenutnoPrijava; i++)
		{
			float potrebnoUplatiti = _prijave[i]._kurs->_cijenaKursa;
			if (_prijave[i]._trenutnoPolaznika >= 2 && _prijave[i]._trenutnoPolaznika <= 3)
				potrebnoUplatiti -= potrebnoUplatiti * 0.1; // 10 % popusta
			else if (_prijave[i]._trenutnoPolaznika > 3)
				potrebnoUplatiti -= potrebnoUplatiti * 0.15; // 15 % popusta

			for (size_t j = 0; j < _prijave[i]._trenutnoPolaznika; j++)
			{
				if (_prijave[i]._uplaceniIznos[j] < potrebnoUplatiti) {
					p[brojac].Unos(_prijave[i]._polaznik[j]._imePrezime, *_prijave[i]._polaznik[j]._datumRodjenja);
					brojac++;
				}
			}
		}
		if (brojac == 0) return nullptr;
		return p;
	}
	void Ispis() {
		cout << crt << _nazivCentra << crt;
		cout << "Kursevi -> " << endl;
		for (size_t i = 0; i < _trenutnoKurseva; i++)
			_kursevi[i]->Ispis();
		cout << crt << endl;
		cout << "Polaznici -> " << endl;
		for (size_t i = 0; i < _trenutnoPrijava; i++)
			_prijave[i].Ispis();
		cout << crt << endl;
	}
	void Dealociraj() {
		for (size_t i = 0; i < _trenutnoKurseva; i++)
		{
			_kursevi[i]->Dealociraj();
			_kursevi[i] = nullptr;
		}
		for (size_t i = 0; i < _trenutnoPrijava; i++)
			_prijave[i].Dealociraj();
		delete[] _prijave; _prijave = nullptr;
	}
};

// pojednostavljena rekurzija rekurzivno cemo prolaziti samo kroz prijave
float PretragaRekurzivno(SkillsCentar sc, int brojPrijava = 0, int dug = 0) {
	if (brojPrijava == sc._trenutnoPrijava)
		return dug;
	float najveca = 0;
	for (size_t i = 0; i < sc._prijave[brojPrijava]._trenutnoPolaznika; i++)
	{
		float trenutniDug = sc._prijave[brojPrijava]._kurs->_cijenaKursa - sc._prijave[brojPrijava]._uplaceniIznos[i];
		if (najveca < trenutniDug)
			najveca = trenutniDug;
	}
	if (najveca > dug)
		return PretragaRekurzivno(sc, brojPrijava + 1, najveca);
	return PretragaRekurzivno(sc, brojPrijava + 1, dug);
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

	Datum datum1, datum2, datum3, datum4, datum5, datum6, rodjendan;
	datum1.Unos(26, 11, 2015);
	datum2.Unos(29, 5, 2016);
	datum3.Unos(5, 6, 2016);
	datum4.Unos(15, 8, 2016);
	datum5.Unos(13, 7, 2016);
	datum6.Unos(22, 9, 2016);
	rodjendan.Unos(15, 6, 1995);

	Kurs softverski, html, sql, security;
	softverski.Unos(SoftwareEngeneeringFundamentals, datum1, datum2, "Emina Junuz", 1800);
	html.Unos(HtmlCSSJavaScript, datum2, datum3, "Larisa Tipura", 1000);
	sql.Unos(MasteringSQL, datum3, datum4, "Jasmin Azemovic", 2230);
	security.Unos(WindowsSecurity, datum3, datum4, "Adel Handzic", 4000);

	Polaznik denis, zanin, indira, dubravka;
	denis.Unos("Denis Music", rodjendan);
	zanin.Unos("Zanin Vejzovic", rodjendan);
	indira.Unos("Indira Hamulic", rodjendan);
	dubravka.Unos("Dubravka Plosko", rodjendan);


	Prijava denisZaninDubravkaHtml, zaninIndiraHtml, indiraSql;
	denisZaninDubravkaHtml.Unos(&html);
	//700 predstavlja uplatu polaznika koji se prijavljuje na kurs
	if (denisZaninDubravkaHtml.DodajPolaznika(denis, 700))//tokom prijave polaznika nije potrebno uplatiti kompletan iznos, ali on mora biti veci od 50% iznosa kojeg je u tom momentu potrebno platiti (u zavisnosti od broja prijavljenih tj. ostvarenog popusta)
		cout << "Polaznik uspjesno dodan!" << endl;
	if (denisZaninDubravkaHtml.DodajPolaznika(zanin, 650))
		cout << "Polaznik uspjesno dodan!" << endl;
	if (denisZaninDubravkaHtml.DodajPolaznika(zanin, 650))//onemoguciti dupliranje polaznika na istoj prijavi
		cout << "Polaznik uspjesno dodan!" << endl;
	if (denisZaninDubravkaHtml.DodajPolaznika(dubravka, 430))//uplata ne zadovoljava kriterije
		cout << "Polaznik uspjesno dodan!" << endl;

	if (denisZaninDubravkaHtml.DodajUplatu(zanin, 200))//polaznik mora biti prethodno dodan prijavi da bi mogao dodati novu uplatu
		cout << "Uplata uspjesno dodana" << endl;

	zaninIndiraHtml.Unos(&html);
	if (zaninIndiraHtml.DodajPolaznika(zanin, 800))
		cout << "Polaznik uspjesno dodan!" << endl;
	if (zaninIndiraHtml.DodajPolaznika(indira, 750))
		cout << "Polaznik uspjesno dodan!" << endl;

	float stanjeUplate = denisZaninDubravkaHtml.StatusUplate(zanin);//ako polaznik duguje novca funkcija vraca negativan iznos potrebne uplate
	cout << "Polaznik "; zanin.Ispis();
	if (stanjeUplate > 0)
		cout << " je u pretplati " << stanjeUplate << " KM" << endl;
	else
		cout << " je duzan uplatiti " << stanjeUplate * -1 << " KM" << endl;

	SkillsCentar mostar;
	mostar.Unos("Skills Center Mostar");
	//jedan edukacijski centar moze moze nuditi vise istih kurseva (npr. MasteringSQL), ali novi kurs moze poceti tek 15 dana nakon zavrsetka prethodnog kursa npr. 
	//ako prethodni kurs MasteringSQL traje 01.03.2016 - 01.04.2016, onda se novi kurs MasteringSQL moze poceti nakon 15.04.2016
	if (mostar.DodajKurs(softverski)) {
		cout << crt << "KURS USPJESNO REGISTROVAN" << crt;
		softverski.Ispis();
		cout << crt;
	}
	if (mostar.DodajKurs(html)) { // dodat cemo i html da bi mogli testirati implementirane funkcionalnosti
		cout << crt << "KURS USPJESNO REGISTROVAN" << crt;
		html.Ispis();
		cout << crt;
	}
	//nije moguce dodati prijavu onog kursa koji nije evidentiran (registrovan, ponudjen) u tom edukacijskom centru
	//moguce je dodati samo one prijave koje kod kojih ukupna uplata iznosi vise od 80% cijene kursa (za prijavljeni broj polaznika) 
	if (mostar.DodajPrijavu(denisZaninDubravkaHtml)) {
		cout << crt << "PRIJAVA EVIDENTIRANA" << crt;
		denisZaninDubravkaHtml.Ispis();
	}
	if (mostar.DodajPrijavu(zaninIndiraHtml)) {
		cout << crt << "PRIJAVA EVIDENTIRANA" << crt;
		zaninIndiraHtml.Ispis();
	}
	int brojPolaznika = 0;
	//vraca niz polaznika koji nisu izmirili uplate
	Polaznik * polazniciDugovanja = mostar.PolazniciDugovanja(brojPolaznika);

	cout << "Pronadjeno " << brojPolaznika << " polaznika koji nisu kompletirali svoje uplate --->>> ";
	for (size_t i = 0; i < brojPolaznika; i++)
		polazniciDugovanja[i].Ispis();

	//PretragaRekurzivno - rekurzivna funkcija pronalazi najveci iznos duga ostvarenog u odredjenom edukacijskom centru
	cout << "Najveci dug iznosi " << PretragaRekurzivno(mostar) << endl;
	//ispisuje sve informacije o edukacijskom centru. prije ispisa sortirati sve prijave po visini dugovanja njenih polaznika
	mostar.Ispis();

	//izvrsiti potrebne dealokacije
	mostar.Dealociraj();
	datum1.Dealociraj(); datum2.Dealociraj(); datum3.Dealociraj(); datum4.Dealociraj(); datum5.Dealociraj(); datum6.Dealociraj(); rodjendan.Dealociraj();
	softverski.Dealociraj(); html.Dealociraj(); sql.Dealociraj(); security.Dealociraj();
	denisZaninDubravkaHtml.Dealociraj(); zaninIndiraHtml.Dealociraj();
	delete[] polazniciDugovanja; polazniciDugovanja = nullptr;
	system("pause");
}