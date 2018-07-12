#include <iostream>
#include <fstream>

using namespace std;

//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

enum Opstina { Opstina1, Opstina2, Opstina3, Opstina4 };
const char * OpstineChar[] = { "Opstina1", "Opstina2", "Opstina3", "Opstina4" };
enum Kandidati { Kandidat1, Kandidat2, Kandidat3, Kandidat4 };
const char * KandidatiChar[] = { "Kandidat1", "Kandidat2", "Kandidat3", "Kandidat4" };

struct Datum {
	int _dan, _mjesec, _godina;
	void Unos(int d, int m, int g) { _dan = d; _mjesec = m; _godina = g; }
	void Ispis() { cout << _dan << "/" << _mjesec << "/" << _godina << endl; }
	int GetDays() { return _dan + _mjesec * 30 + _godina * 365; }
};

Datum datumIzbora;

struct Osoba {
	Datum _datumRodjenja;
	char * _imePrezime;
	char _JMBG[14];
	Opstina _Prebivaliste;
	void unos(Datum d, const char * ip, const char jmbg[], Opstina p) {
		_datumRodjenja = d;
		_imePrezime = new char[strlen(ip) + 1];
		strcpy_s(_imePrezime, strlen(ip) + 1, ip);
		strcpy(_JMBG, jmbg);
		_Prebivaliste = p;
	}
	void Dealociraj() { delete[] _imePrezime; _imePrezime = nullptr; }
	void Info() {
		_datumRodjenja.Ispis();
		cout << _imePrezime << " " << _JMBG << " " <<
			OpstineChar[_Prebivaliste] << endl;
	}
};
struct Glas {
	Osoba * _glasac;
	Kandidati * _glasZa;
	void Unos(Osoba o, Kandidati k) {
		_glasac = new Osoba;
		_glasac->unos(o._datumRodjenja, o._imePrezime, o._JMBG, o._Prebivaliste);
		_glasZa = new Kandidati(k);
	}
	void Ispis() {
		_glasac->Info();
		cout << KandidatiChar[*_glasZa] << endl;
	}
	void Dealociraj() { _glasac->Dealociraj(); }
};
struct Izbori2016 {
	Opstina * _glasackoMjesto;
	Glas * _glasovi[1000];
	int _doSadaGlasalo;

	/*
	1. Unos - na osnovu vrijednosti primljenih parametara, inicijalizovati vrijednosti atributa strukture.
	2. DodajGlas - funkcija treba da omoguci dodavanje informacija o novom glasu i tom prilikom onemoguciti: da ista osoba glasa vise puta, glasanje osobama mladjim od 18 godina (uzeti u obzir samo mjesec i godinu rodjenja), glasanje osobama iz drugih opstina. U zavisnosti od uspjesnosti operacije funkcija vraca true ili false
	3. Ispis - u zavisnosti od postignutog uspjeha funkcija treba ispisati listu kandidata sortiranu opadajucim redoslijedom, a uz svakog kandidata je potrebno ispisati i osvojeni procenat glasova.
	Funkcija vraca opstinu u kojoj je pobjednicki kandidat ostvario najveci broj glasova, a ukoliko je pobjednicki kandidat ostvario podjednak broj glasova u vise opstina, funkcija vraca prvu koja se nalazi u nizu na koji pokazuje pokazivac izbornaMjesta
	4. BrojGlasacaPoRodjenju - funkcija vraca broj glasaca koji su do tog momenta glasali, a koji su rodjeni u parametrima definisanom periodu (parametri su tipa datum, period OD i DO)
	*/
	void Unos(Opstina o) {
		_glasackoMjesto = new Opstina(o);
		_doSadaGlasalo = 0;
	}

	bool DodajGlas(Glas g) {
		for (size_t i = 0; i < _doSadaGlasalo; i++)
			if (strcmp(_glasovi[i]->_glasac->_JMBG, g._glasac->_JMBG) == 0)
				return false;
		if (g._glasac->_Prebivaliste != *_glasackoMjesto)
			return false;
		if ((datumIzbora.GetDays() - g._glasac->_datumRodjenja.GetDays()) < 18 * 365)
			return false;
		_glasovi[_doSadaGlasalo] = new Glas;
		_glasovi[_doSadaGlasalo]->Unos(*g._glasac, *g._glasZa);
		_doSadaGlasalo++;
		return true;
	}

	Opstina Ispis() {
		// potrebno je napraviti strukturu da bi se preborajali glasovi Kandidata
		// struktura ce sadrzavati atribute Kandidat i njegov broj glasova
		struct KandidatProcenat {
			Kandidati _kandidat;
			int _brojGlasova;
			void Unos(Kandidati k) {
				_kandidat = k;
				_brojGlasova = 0;
			}
		};

		// Napravit cemo niz od 4 objekta KandidatProcenat u svrhu sortiranja (svaki kandidat iz enumeracije ce nam predstavljat jedan clan niza
		KandidatProcenat kandidati[4];
		for (size_t i = 0; i < 4; i++)
			kandidati[i].Unos((Kandidati)i);

		int ukupnoGlasova = 0;

		for (size_t i = 0; i < _doSadaGlasalo; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				if (kandidati[j]._kandidat == *_glasovi[i]->_glasZa) {
					kandidati[j]._brojGlasova++;
					ukupnoGlasova++;
				}
			}
		}
		//sortiranje kandidata
		bool prolaz = false;
		do
		{
			prolaz = false;
			for (size_t i = 0; i < 3; i++)
			{
				if (kandidati[i]._brojGlasova < kandidati[i + 1]._brojGlasova) {
					KandidatProcenat temp;
					temp = kandidati[i];
					kandidati[i] = kandidati[i + 1];
					kandidati[i + 1] = temp;
					prolaz = true;
				}
			}
		} while (prolaz);
		for (size_t i = 0; i < 4; i++)
		{
			cout << KandidatiChar[kandidati[i]._kandidat] << " - " << ((float)kandidati[i]._brojGlasova / ukupnoGlasova) * 100 << " %" << endl;
		}
		return *_glasackoMjesto;
	}

	int BrojGlasacaPoRodjenju(Datum OD, Datum DO) {
		int brojGlasaca = 0;
		for (size_t i = 0; i < _doSadaGlasalo; i++)
		{
			if (_glasovi[i]->_glasac->_datumRodjenja.GetDays() >= OD.GetDays() && _glasovi[i]->_glasac->_datumRodjenja.GetDays() <= DO.GetDays())
				brojGlasaca++;
		}
		return brojGlasaca;
	}
};
/*
1. UpisiUBinarniFajl - objekat koji je primljen kao parametar upisuje u binarni fajl. Naziv fajla treba biti identican nazivu opstine u kojoj se odrzavaju izbori
2. UcitajIzBinarnogFajla - u objekat koji je primljen kao parametar ucitava sadrzaj binarnog fajla (naziv fajla je takodjer proslijedjen kao parametar)
3. PretragaRekurzivno - rekurzivna funkcija koja treba da vrati broj glasova koje je na izborima ostvario odredjeni kandidat (kandidat se proslijedjuje kao parametar)
*/
int PretragaRekurzivno(Izbori2016 i, Kandidati k, int brojGlasova = 0) {
	if (brojGlasova == i._doSadaGlasalo) {
		return 0;
	}
	if (*i._glasovi[brojGlasova]->_glasZa == k)
		return 1 + PretragaRekurzivno(i, k, brojGlasova + 1);
	return PretragaRekurzivno(i, k, brojGlasova + 1);
}

void UpisiUBinarniFajl(Izbori2016 &izbori) {
	char nazivFajla[100];
	strncpy_s(nazivFajla, 100, OpstineChar[*izbori._glasackoMjesto], _TRUNCATE);
	strncat_s(nazivFajla, 100, ".bin", _TRUNCATE);

	ofstream upisUFajl(nazivFajla, ios::binary);
	if (upisUFajl.good())
		upisUFajl.write((char*)&izbori, sizeof(Izbori2016));
	upisUFajl.close();
}

void UcitajIzBinarnogFajla(Izbori2016 &izbori, const char *naziv) {
	char nazivFajla[100];
	strncpy_s(nazivFajla, 100, naziv, _TRUNCATE);
	strncat_s(nazivFajla, 100, ".bin", _TRUNCATE);

	ifstream ucitavanjaIzFajla(nazivFajla, ios::binary);
	if (ucitavanjaIzFajla.good())
		ucitavanjaIzFajla.read((char*)&izbori, sizeof(Izbori2016));
	ucitavanjaIzFajla.close();
}
void main() {
	/*
	1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NEÆE BODOVATI
	2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
	3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
	4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
	5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ÆETE KOPIRATI VAŠA RJEŠENJA)
	6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
	*/

	// Posto je datum izbora kreiran u "main", a nije proslijedjen kao parametar funkciji "DodajGlas" zato nemamo mogucnosti da 
	// provjerimo da li je glasac mladji od 18 god. da bi to mogli uraditi potrebno je da deklaraciju za "datumIzbora" 
	//prebacite iznad strukture "Izbori2016" ili da napravite objekat datum globalan takodjer iznad strukture "Izbori2016" 
	//i u "main" funkciji stavite "temp = datumIzbora"
	Datum datumGlasac1, datumGlasac2, datumGlasac3, datumGlasac4;
	datumIzbora.Unos(14, 7, 2016);
	datumGlasac1.Unos(18, 5, 1990);
	datumGlasac2.Unos(5, 3, 1982);
	datumGlasac3.Unos(12, 8, 1958);
	datumGlasac4.Unos(22, 6, 2000);

	Osoba a, b, c, d, e;
	a.unos(datumGlasac1, "Denis Music", "1111111111111", Opstina1);
	b.unos(datumGlasac2, "Jasmin Azemovic", "1111111111112", Opstina1);
	c.unos(datumGlasac3, "Indira Hamulic", "1111111111113", Opstina1);
	d.unos(datumGlasac4, "Adel Handzic", "1111111111114", Opstina2);
	e.unos(datumGlasac4, "Zanin Vejzovic", "1111111111115", Opstina1);


	Glas g1, g2, g3, g4, g5;
	g1.Unos(a, Kandidat1);
	g2.Unos(b, Kandidat1);
	g3.Unos(c, Kandidat2);
	g4.Unos(d, Kandidat1);
	g5.Unos(e, Kandidat2);

	Izbori2016 izbori, izbori_FromFile;
	izbori.Unos(Opstina1);

	izbori.DodajGlas(g1);
	izbori.DodajGlas(g2);
	izbori.DodajGlas(g3);
	izbori.DodajGlas(g4);
	izbori.DodajGlas(g5);
	izbori.Ispis();

	Datum OD, DO;
	OD.Unos(17, 7, 1986);
	DO.Unos(17, 7, 1998);
	cout << "Glasaca u starost od 18 - 30 godina->" << izbori.BrojGlasacaPoRodjenju(OD, DO) << endl;
	cout << "Broj glasova za Kandidat1->" << PretragaRekurzivno(izbori, Kandidat1) << endl;
	UpisiUBinarniFajl(izbori);
	UcitajIzBinarnogFajla(izbori_FromFile, "Opstina1");
	izbori_FromFile.Ispis();

	system("pause");
}