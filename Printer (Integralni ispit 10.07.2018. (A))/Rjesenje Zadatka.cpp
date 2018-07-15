/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA, U SUPROTNOM SE RAD NECE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VASIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJESENJA VASIH ZADATAKA. NE PREDAVATI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORISTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI CETE KOPIRATI VASA RJESENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URADJENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/
#include<iostream>
#include<memory>
using namespace std;

//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

const char *crt = "\n-------------------------------------------\n";

enum vrstaDokumenta { PDF, DOC, TXT, HTML };
const char * vrstaDokumentaChar[] = { "PDF", "DOC", "TXT", "HTML" };

enum Prioritet { VISOK, SREDNJI, NIZAK };
const char * PrioritetChar[] = { "VISOK", "SREDNJI", "NIZAK" };

// nepotrebna funkcija 
char * AlocirajNizKaraktera(const char * sadrzaj) {
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

const int BROJ_ZNAKOVA_PO_STRANICI = 30;

struct DatumVrijeme {
	int *_dan, *_mjesec, *_godina, *_sati, *_minuti;
	void Unos(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 1, int minuti = 1) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);
	}
	void Dealociraj() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati; _sati = nullptr;
		delete _minuti; _minuti = nullptr;
	}
	void Ispis() {
		cout << *_dan << "." << *_mjesec << "." << *_godina << " " << *_sati << ":" << *_minuti << endl;
	}

	int GetDani() { return *_dan + *_mjesec * 30 + *_godina * 365; }
};

struct Filter {
	char * _sadrzaj;
	Prioritet _prioritet;
	void Unos(const char * sadrzaj, Prioritet prioritet) {
		_sadrzaj = AlocirajNizKaraktera(sadrzaj);
		_prioritet = prioritet;
	}
	void Dealociraj() {
		delete[] _sadrzaj; _sadrzaj = nullptr;
	}
	void Ispis() { cout << _sadrzaj << " (" << PrioritetChar[_prioritet] << ")" << endl; }
};

struct Dokument {
	vrstaDokumenta _vrsta;
	char * _naziv;
	char * _sadrzaj;
	unique_ptr<DatumVrijeme> _kreiran;
	int _brojStranica;

	void Unos(vrstaDokumenta vrsta, const char * naziv, DatumVrijeme kreiran) {
		_naziv = AlocirajNizKaraktera(naziv);
		_vrsta = vrsta;
		_kreiran = make_unique<DatumVrijeme>();
		_kreiran->Unos(*kreiran._dan, *kreiran._mjesec, *kreiran._godina, *kreiran._sati, *kreiran._minuti);
		_brojStranica = 0;//broj stranica se odredjuje prilikom svakog dodavanja novog sadrzaja dokumentu
		_sadrzaj = nullptr;
	}
	void DodajSadrzaj(const char *sadrzaj) {
		if (_sadrzaj != nullptr) {
			int size = strlen(sadrzaj) + strlen(_sadrzaj) + 1;
			char *temp = new char[size];
			strcpy_s(temp, size, _sadrzaj);
			delete[] _sadrzaj;
			strcat_s(temp, size, sadrzaj);
			_sadrzaj = temp;
			_brojStranica = strlen(_sadrzaj) / BROJ_ZNAKOVA_PO_STRANICI + ((strlen(_sadrzaj) % BROJ_ZNAKOVA_PO_STRANICI) > 0);
		}
		else {
			int size = strlen(sadrzaj) + 1;
			_sadrzaj = AlocirajNizKaraktera(sadrzaj);
			_brojStranica = strlen(_sadrzaj) / BROJ_ZNAKOVA_PO_STRANICI + ((strlen(_sadrzaj) % BROJ_ZNAKOVA_PO_STRANICI) > 0);
		}
	}
	void Dealociraj() {
		delete[] _naziv; _naziv = nullptr;
		delete[] _sadrzaj; _sadrzaj = nullptr;
		_kreiran->Dealociraj();
		_kreiran.reset();
	}
	void Ispis() {
		cout << crt << _naziv << " " << vrstaDokumentaChar[_vrsta] << " kreiran ";
		_kreiran->Ispis();
		cout << crt << _sadrzaj << crt << " br.stranica (" << _brojStranica << ")" << crt;
	}
};
struct Printer {
	char * _model;
	Dokument * _dokumenti;
	int _trenutnoDokumenata;
	Filter * _filteri;
	int _trenutnoFiltera;

	void Unos(const char * model) {
		_model = AlocirajNizKaraktera(model);
		_filteri = nullptr;
		_dokumenti = nullptr;
		_trenutnoDokumenata = 0;
		_trenutnoFiltera = 0;
	}
	bool DodajFilter(const char *sadrzajFiltera, Prioritet p) {
		for (size_t i = 0; i < _trenutnoFiltera; i++)
		{
			if (strcmp(_filteri[i]._sadrzaj, sadrzajFiltera) == 0)
				return false;
		}
		Filter *temp = new Filter[_trenutnoFiltera + 1];
		for (size_t i = 0; i < _trenutnoFiltera; i++)
		{
			temp[i].Unos(_filteri[i]._sadrzaj, _filteri[i]._prioritet);
			_filteri[i].Dealociraj();
		}
		delete[] _filteri;
		temp[_trenutnoFiltera].Unos(sadrzajFiltera, p);
		_filteri = temp;
		_trenutnoFiltera++;
		return true;
	}
	bool isPrintat(Dokument &d) {
		for (size_t i = 0; i < strlen(d._naziv); i++)
		{
			if ((d._naziv[i] < 65 || (d._naziv[i] > 90 && d._naziv[i] < 97) || d._naziv[i] > 122) && d._naziv[i] != 46)
				return false;
		}
		if (d._vrsta != PDF && d._vrsta != DOC && d._vrsta != TXT && d._vrsta != HTML)
			return false;

		int brojacFiltera[3] = { 0 };
		for (size_t i = 0; i < _trenutnoFiltera; i++)
		{
			if (strstr(d._sadrzaj, _filteri[i]._sadrzaj) != nullptr) {
				brojacFiltera[(int)_filteri[i]._prioritet]++;
			}
		}
		if (brojacFiltera[0] > 0 || brojacFiltera[1] > 2 || brojacFiltera[2] > 3)
			return false;
		return true;
	}
	// Objekat se mora primiti po referenci (U slucaju da saljete objekat po vrijednosti to znaci da se kopira sve sto se nalazi u 
	// objektu iz "main" funkcije u ovaj iz parametra pa tako i adresa na koju pokazuje unique_ptr i dobivamo 2 unique_ptr koja pokazuju na jedan objekat sto
	// rusi osobinu unique_ptr da samo on moze cuvati adresu objekta na koji "pokazuje") u suprotnom morate poznavati gradivo koje izlazi iz okvira i PR III.
	bool Printaj(Dokument &d) {

		if (d._sadrzaj == nullptr) return false; // nema sadrzaja za printanje

		bool printaj = isPrintat(d);

		if (printaj) {
			for (size_t i = 0; i < strlen(d._sadrzaj); i++)
			{
				if (i % BROJ_ZNAKOVA_PO_STRANICI == 0)
					cout << crt;
				cout << d._sadrzaj[i];
			}
		}
		Dokument *temp = new Dokument[_trenutnoDokumenata + 1];
		for (size_t i = 0; i < _trenutnoDokumenata; i++)
		{
			temp[i].Unos(_dokumenti[i]._vrsta, _dokumenti[i]._naziv, *_dokumenti[i]._kreiran);
			if (_dokumenti[i]._sadrzaj != nullptr)
				temp[i].DodajSadrzaj(_dokumenti[i]._sadrzaj);
			_dokumenti[i].Dealociraj();
		}
		delete[] _dokumenti;
		temp[_trenutnoDokumenata].Unos(d._vrsta, d._naziv, *d._kreiran);
		if (d._sadrzaj != nullptr)
			temp[_trenutnoDokumenata].DodajSadrzaj(d._sadrzaj);
		_dokumenti = temp;
		_trenutnoDokumenata++;

		return printaj;
	}
	Filter *GetNakoristenijiFilter() {
		int brojPonavljanjaFiltera = 0;
		Filter *f = nullptr;
		for (size_t i = 0; i < _trenutnoFiltera; i++)
		{
			int trenutnoPonavljanjeFiltera = 0;
			for (size_t j = 0; j < _trenutnoDokumenata; j++)
			{
				if (strstr(_dokumenti[j]._sadrzaj, _filteri[i]._sadrzaj) != nullptr)
					trenutnoPonavljanjeFiltera++;
			}
			if (trenutnoPonavljanjeFiltera > brojPonavljanjaFiltera) {
				brojPonavljanjaFiltera = trenutnoPonavljanjeFiltera;
				f = &_filteri[i];
			}
		}
		return f;
	}
	float GetProsjecanBrojStranicaUPeriodu_Od_Do(DatumVrijeme OD, DatumVrijeme DO, int brojDokumenata = 0, int ukupanBrojStranica = 0, int brojPrintanihDokumenata = 0) {
		if (_trenutnoDokumenata == brojDokumenata) {
			return (float)ukupanBrojStranica / brojPrintanihDokumenata;
		}
		if (_dokumenti[brojDokumenata]._kreiran->GetDani() >= OD.GetDani() && _dokumenti[brojDokumenata]._kreiran->GetDani() <= DO.GetDani()) {
			// provjera da li je printan
			if (isPrintat(_dokumenti[brojDokumenata])) {
				return GetProsjecanBrojStranicaUPeriodu_Od_Do(OD, DO, brojDokumenata + 1, ukupanBrojStranica + _dokumenti[brojDokumenata]._brojStranica, brojPrintanihDokumenata + 1);
			}
			return GetProsjecanBrojStranicaUPeriodu_Od_Do(OD, DO, brojDokumenata + 1, ukupanBrojStranica + _dokumenti[brojDokumenata]._brojStranica, brojPrintanihDokumenata);
		}
		return GetProsjecanBrojStranicaUPeriodu_Od_Do(OD, DO, brojDokumenata + 1, ukupanBrojStranica, brojPrintanihDokumenata);
	}
	void Dealociraj() {
		delete[] _model; _model = nullptr;
		for (size_t i = 0; i < _trenutnoDokumenata; i++)
			_dokumenti[i].Dealociraj();
		delete[]_dokumenti; _dokumenti = nullptr;
		for (size_t i = 0; i < _trenutnoFiltera; i++)
			_filteri[i].Dealociraj();
		delete[]_filteri; _filteri = nullptr;
	}
	void Ispis() {
		cout << _model << crt;
		for (size_t i = 0; i < _trenutnoDokumenata; i++)
			_dokumenti[i].Ispis();
	}
};


int main() {
	DatumVrijeme prije3Dana; prije3Dana.Unos(3, 2, 2018, 10, 15);
	DatumVrijeme danas1; danas1.Unos(6, 2, 2018, 10, 15);
	DatumVrijeme danas2; danas2.Unos(6, 2, 2018, 10, 16);
	DatumVrijeme za10Dana; za10Dana.Unos(16, 2, 2018, 10, 15);

	Dokument ispitPRII, ispitMAT, ispitUIT, ispitUITDrugiRok;
	ispitPRII.Unos(DOC, "ispitPRII.doc", prije3Dana);
	ispitMAT.Unos(DOC, "ispitMAT.doc", danas1);
	ispitUIT.Unos(DOC, "ispitUIT.doc", danas2);
	ispitUITDrugiRok.Unos(PDF, "ispitUITDrugiRok.pdf", za10Dana);

	/*BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREDJUJE PRILIKOM DODAVANJA SADRZAJA. ZA POTREBE ISPITA PRETPOSTAVLJAMO DA NA JEDNU
	STRANICU MOZE STATI BROJ_ZNAKOVA_PO_STRANICI ZNAKOVA UKLJUCUJUCI RAZMAKE I DRUGE VRSTE ZNAKOVA*/
	ispitPRII.DodajSadrzaj("Programiranje ili racunarsko programiranje (engl. programming) jeste vjestina pomocu koje ");
	ispitPRII.DodajSadrzaj("korisnik stvara i izvrsava algoritme koristeci odredjene programske jezike da bi ... ");
	cout << "Broj stranica -> " << ispitPRII._brojStranica << endl;

	ispitPRII.Ispis();//ISPISUJE SVE PODATKE O DOKUMENTU

	ispitMAT.DodajSadrzaj("Matematika se razvila iz potrebe da se obavljaju proracuni u trgovini, vrse mjerenja zemljista i predvidjaju ");
	ispitMAT.DodajSadrzaj("astronomski dogadjaji, i ove tri primjene se mogu dovesti u vezu sa grubom podjelom matematike ");

	Printer hp3200; hp3200.Unos("HP 3200");
	/*PRINTER NECE DOZVOLITI PRINTANJE DOKUMENATA U CIJEM SADRZAJU SE NALAZI NEKA OD ZABRANJENIH RIJECI DEFINISANIH FILTERIMA*/
	if (hp3200.DodajFilter("RE", NIZAK)) //VISOK :)
		cout << "Filter dodan!" << endl;
	if (hp3200.DodajFilter("RAT", VISOK))
		cout << "Filter dodan!" << endl;
	if (hp3200.DodajFilter("UBITI", VISOK))
		cout << "Filter dodan!" << endl;
	if (hp3200.DodajFilter("MRZITI", SREDNJI))
		cout << "Filter dodan!" << endl;
	if (!hp3200.DodajFilter("RE", SREDNJI))/* ONEMOGUCITI PONAVLJANJE FILTERA, BEZ OBZIRA STO SU RAZLICITOG PRIORITETA*/
		cout << "Filter nije dodan!" << endl;

	/*
	DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDECA PRAVILA:
	1. NAZIV DOKUMENTA MOZE SADRZAVATI SAMO SLOVA, A EKSTENZIJA MOZE BITI IDENTICNA ONOJ DEFINISANOJ VRIJEDNOSCU ATRIBUTA vrstaDokumenta
	2. SADRZAJ DOKUMENTA MOZE POSJEDOVATI RIJECI DEFINISANE FILTERIMA, ALI PREMA SLJEDECIM PRAVILIMA:
	- NITI JEDNU RIJEC OZNACENU FILTEROM PRIORITETA VISOK
	- NAJVISE 2 RIJECI OZNACENE FILTEROM PRIORITETA SREDNJI
	- NAJVISE 3 RIJECI OZNACENE FILTEROM PRIORITETA NIZAK
	UKOLIKO NEKI OD NAVEDENIH USLOVA NIJE ZADOVOLJEN FUNKCIJA PRINTAJ DOKUMENT TREBA SACUVATI U NIZ _dokumenti, ALI TAJ DOKUMENT NE TREBA PRINTATI I FUNKCIJA VRACA FALSE.
	UKOLIKO DOKUMENT ISPUNJAVA SVE USLOVE ZA PRINTANJE POTREBNO JE NA KONZOLU ISPISATI SADRZAJ DOKUMENTA KOJI SE PRINTA, A SA CRT LINIJOM ODVAJATI STRANICE DOKUMENTA
	*/
	if (hp3200.Printaj(ispitPRII))
		cout << "Printam -> " << ispitPRII._naziv << endl;
	if (hp3200.Printaj(ispitMAT))
		cout << "Printam -> " << ispitMAT._naziv << endl;
	if (hp3200.Printaj(ispitUIT))
		cout << "Printam -> " << ispitUIT._naziv << endl;
	if (hp3200.Printaj(ispitUITDrugiRok))
		cout << "Printam -> " << ispitUITDrugiRok._naziv << endl;

	//PROSJECAN BROJ STRANICA = UKUPAN BROJ STRANICA / BROJ PRINTANIH DOKUMENATA
	//REKURZIVNA FUNKCIJA VRACA PROSJECAN BROJ ISPRINTANIH STRANICA (KOJE SU ZADOVOLJILE POSTAVLJENE FILTERE) U PERIODU OD (npr.prije3Dana) - DO (npr.danas2), UKLJUCUJUCI I TAJ PERIOD
	cout << "Prosjecan broj printanih stranica je -> " << hp3200.GetProsjecanBrojStranicaUPeriodu_Od_Do(prije3Dana, danas2) << crt;
	//VRACA SADRZAJ FILTERA KOJI SE NAJCESCE KORISTIO U DOKUMENTIMA
	Filter * f = hp3200.GetNakoristenijiFilter();
	if (f != nullptr && f->_sadrzaj != nullptr)
		cout << "Najcesce koristeni sadrzaj filtera je -> " << f->_sadrzaj << crt;
	//PORED VRIJEDNOSTI SVIH OSTALIH ATRIBUTA, ISPISUJE SAMO ONE DOKUMENTE KOJI ZADOVOLJAVAJU SVA PRETHODNO DEFINISANA PRAVILA
	hp3200.Ispis();

	/*NAPISATI LAMBDA FUNKCIJU ZamijeniIPrintaj KOJA U SADRZAJU DOKUMENTA MIJENJA PRVI POSLATI PARAMETAR SA VRIJEDNOSCU DRUGOG PARAMETRA, TE VRACA BROJ ZAMIJENJENIH ZNAKOVA*/
	auto ZamijeniIPrintaj = [&hp3200](char karakterKojiSeMjenja, char karakterSaKojimSeMijenja, DatumVrijeme datum)->int {
		int brojZamjenjenihKaraktera = 0;
		for (size_t i = 0; i < hp3200._trenutnoDokumenata; i++)
		{
			if (hp3200.isPrintat(hp3200._dokumenti[i]) && hp3200._dokumenti[i]._kreiran->GetDani() == datum.GetDani()) {
				for (size_t j = 0; j < strlen(hp3200._dokumenti[i]._sadrzaj); j++)
				{
					if (hp3200._dokumenti[i]._sadrzaj[j] == karakterKojiSeMjenja) {
						hp3200._dokumenti[i]._sadrzaj[j] = karakterSaKojimSeMijenja;
						brojZamjenjenihKaraktera++;
					}
				}
			}
		}
		return brojZamjenjenihKaraktera;
	};
	//RAZMAK MIJENJA ZNAKOM CRTICA U SADRZAJU DOKUMENATA KOJI SU PRINTANI danas1
	int zamijenjeno = ZamijeniIPrintaj(' ', '-', danas1);
	cout << "Zamijenjeno -> " << zamijenjeno << " znakova" << endl;
	hp3200.Ispis();

	prije3Dana.Dealociraj(); danas1.Dealociraj(); danas2.Dealociraj(); za10Dana.Dealociraj();
	ispitMAT.Dealociraj(); ispitPRII.Dealociraj(); ispitUIT.Dealociraj(); ispitUITDrugiRok.Dealociraj();

	hp3200.Dealociraj();

	system("pause");
	return 0;
}