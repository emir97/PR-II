/*
1. ZADATAK

Izvršiti definiciju funkcija na način koji odgovara opisu (komentarima) datom neposredno uz pozive ili nazive funkcija. Možete dati komentar na bilo koju liniju code-a koju smatrate da bi trebalo unaprijediti ili da će eventualno uzrokovati grešku prilikom kompajliranja. Također, možete dodati dodatne funkcije koje će vam olakšati implementaciju programa.


1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NEĆE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ĆETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/
#include<iostream>
using namespace std;

//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

const char *crt = "\n-------------------------------------------\n";

enum vrstaDokumenta { PDF, DOC, TXT, HTML };
enum prioritet { VISOK, SREDNJI, NIZAK };
struct DatumVrijeme {
	int *_dan, *_mjesec, *_godina, *_sati, *_minuti;
	void Unos(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0) {
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
	prioritet _prioritet;
	void Unos(char * sadrzaj, prioritet prioritet) {
		int vel = strlen(sadrzaj) + 1;
		_sadrzaj = new char[vel];
		strcpy_s(_sadrzaj, vel, sadrzaj);
		_prioritet = prioritet;
	}
	void Dealociraj() {
		delete[] _sadrzaj; _sadrzaj = nullptr;
	}
	void Ispis() { cout << _sadrzaj << " (" << _prioritet << ")" << endl; }

};

struct Dokument {
	vrstaDokumenta _vrsta;
	char * _naziv;
	char * _sadrzaj;
	DatumVrijeme _kreiran;
	int _brojStranica;

	void Unos(vrstaDokumenta vrsta, const char * naziv, DatumVrijeme kreiran) {
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);
		_vrsta = vrsta;
		_kreiran.Unos(*kreiran._dan, *kreiran._mjesec, *kreiran._godina, *kreiran._sati, *kreiran._minuti);
		_brojStranica = 0;//broj stranica se odredjuje prilikom dodavanja svakog novog sadrzaja dokumentu
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
			_brojStranica = strlen(_sadrzaj) / 30 + ((strlen(_sadrzaj) % 30) > 0);
		}
		else {
			int size = strlen(sadrzaj) + 1;
			_sadrzaj = new char[size];
			strcpy_s(_sadrzaj, size, sadrzaj);
			_brojStranica = strlen(_sadrzaj) / 30 + ((strlen(_sadrzaj) % 30) > 0);
		}
	}
	void Dealociraj() {
		delete[] _naziv; _naziv = nullptr;
		delete[] _sadrzaj; _sadrzaj = nullptr;
		_kreiran.Dealociraj();
	}

	void Ispis() {
		cout << crt << _naziv << " " << _vrsta << " kreiran ";
		_kreiran.Ispis();
		cout << crt << _sadrzaj << crt << " br.stranica (" << _brojStranica << ")" << crt;
	}
};
struct Printer {
	char * _proizvodjac;
	char * _model;
	Dokument * _dokumenti;
	int _trenutnoDokumenata;
	Filter * _filteri;
	int _trenutnoFiltera;

	void Unos(const char * proizvodjac, const char * model) {
		int vel = strlen(proizvodjac) + 1;
		_proizvodjac = new char[vel];
		strcpy_s(_proizvodjac, vel, proizvodjac);
		vel = strlen(model) + 1;
		_model = new char[vel];
		strcpy_s(_model, vel, model);
		_trenutnoDokumenata = 0;
		_trenutnoFiltera = 0;
		_filteri = nullptr;
		_dokumenti = nullptr;
	}
	bool DodajFilter(const char *sadrzajFiltera, prioritet p) {
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
		temp[_trenutnoFiltera].Unos((char*)sadrzajFiltera, p);
		_filteri = temp;
		_trenutnoFiltera++;
		return true;
	}
	void Printaj(Dokument d) {
		bool printaj = true;
		if (d._sadrzaj == nullptr) return;
		for (size_t i = 0; i < strlen(d._naziv); i++)
		{
			if ((d._naziv[i] < 65 || (d._naziv[i] > 90 && d._naziv[i] < 97) || d._naziv[i] > 122) && d._naziv[i] != 46)
				printaj = false;
		}
		if (d._vrsta != PDF && d._vrsta != DOC && d._vrsta != TXT && d._vrsta != HTML)
			printaj = false;
		for (size_t i = 0; i < _trenutnoFiltera; i++)
		{
			if (strstr(d._sadrzaj, _filteri[i]._sadrzaj) != nullptr) printaj = false;
		}
		if (printaj) {
			for (size_t i = 0; i < strlen(d._sadrzaj); i++)
			{
				cout << d._sadrzaj[i];
				if (i % 30 == 0)
					cout << crt;
			}
		}
		Dokument *temp = new Dokument[_trenutnoDokumenata + 1];
		for (size_t i = 0; i < _trenutnoDokumenata; i++)
		{
			temp[i].Unos(_dokumenti[i]._vrsta, _dokumenti[i]._naziv, _dokumenti[i]._kreiran);
			if (_dokumenti[i]._sadrzaj != nullptr)
				temp[i].DodajSadrzaj(_dokumenti[i]._sadrzaj);
			_dokumenti[i].Dealociraj();
		}
		delete[] _dokumenti;
		temp[_trenutnoDokumenata].Unos(d._vrsta, d._naziv, d._kreiran);
		if (d._sadrzaj != nullptr)
			temp[_trenutnoDokumenata].DodajSadrzaj(d._sadrzaj);
		_dokumenti = temp;
		_trenutnoDokumenata++;
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
	float GetProsjecanBrojStranicaPoDatumuIVrsti(DatumVrijeme OD, DatumVrijeme DO, vrstaDokumenta v) {
		int ukupanBrojStranica = 0;
		int brojPrintanihDokumenata = 0;
		for (size_t i = 0; i < _trenutnoDokumenata; i++)
		{
			if (OD.GetDani() <= _dokumenti[i]._kreiran.GetDani() && DO.GetDani() >= _dokumenti[i]._kreiran.GetDani()) {
				// provjera da li je dokument printan (moze se izdvojiti u zasebnu funkciju koja ce raditi provjeru printanja)
				bool printaj = true;
				for (size_t j = 0; j < strlen(_dokumenti[i]._naziv); j++)
				{
					if ((_dokumenti[i]._naziv[j] < 65 || (_dokumenti[i]._naziv[j] > 90 && _dokumenti[i]._naziv[j] < 97) || _dokumenti[i]._naziv[j] > 122) && _dokumenti[i]._naziv[j] != 46)
						printaj = false;
				}
				if (_dokumenti[i]._vrsta != PDF && _dokumenti[i]._vrsta != DOC && _dokumenti[i]._vrsta != TXT && _dokumenti[i]._vrsta != HTML)
					printaj = false;
				for (size_t j = 0; j < _trenutnoFiltera; j++)
				{
					if (strstr(_dokumenti[i]._sadrzaj, _filteri[j]._sadrzaj) != nullptr) printaj = false;
				}
				if (printaj) brojPrintanihDokumenata++;
				ukupanBrojStranica += _dokumenti[i]._brojStranica;
			}
		}
		if (!brojPrintanihDokumenata) return 0;
		return (float)ukupanBrojStranica / brojPrintanihDokumenata;
	}
	void Dealociraj() {
		delete[] _proizvodjac; _proizvodjac = nullptr;
		delete[] _model; _model = nullptr;
		for (size_t i = 0; i < _trenutnoDokumenata; i++)
			_dokumenti[i].Dealociraj();
		delete[]_dokumenti; _dokumenti = nullptr;
		for (size_t i = 0; i < _trenutnoFiltera; i++)
			_filteri[i].Dealociraj();
		delete[]_filteri; _filteri = nullptr;
	}
	void Ispis() {
		for (size_t i = 0; i < _trenutnoDokumenata; i++)
			_dokumenti[i].Ispis();
	}
};
void ZamijeniIPrintaj(Printer p, char karakter, int brojac = 0) {
	if (brojac != p._trenutnoDokumenata) {
		for (size_t i = 0; i < p._trenutnoFiltera; i++)
		{
			char *niz = strstr(p._dokumenti[brojac]._sadrzaj, p._filteri[i]._sadrzaj);
			if (niz != nullptr) {
				for (size_t j = 0; j < strlen(p._filteri[i]._sadrzaj); j++)
					niz[j] = karakter;
			}
		}
		return ZamijeniIPrintaj(p, karakter, brojac + 1);
	}
}

void main() {
	DatumVrijeme prije3Dana; prije3Dana.Unos(3, 2, 2017, 10, 15);
	DatumVrijeme danas1; danas1.Unos(6, 2, 2017, 10, 15);
	DatumVrijeme danas2; danas2.Unos(6, 2, 2017, 10, 16);
	DatumVrijeme za10Dana; za10Dana.Unos(16, 2, 2017, 10, 15);

	Dokument ispitPRII, ispitMAT, ispitUIT, ispitUITDrugiRok;
	ispitPRII.Unos(DOC, "ispitPRII.doc", prije3Dana);
	ispitMAT.Unos(DOC, "ispitMAT.doc", danas1);
	ispitUIT.Unos(DOC, "ispitUIT.doc", danas2);
	ispitUITDrugiRok.Unos(PDF, "ispitUITDrugiRok.pdf", za10Dana);

	/*BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREĐUJE PRILIKOM DODAVANJA SADRŽAJA. ZA POTREBE ISPITA PRETPOSTAVLJAMO DA NA JEDNU
	STRANICU MOŽE STATI 30 ZNAKOVA UKLJUČUJUĆI I RAZMAKE*/
	ispitPRII.DodajSadrzaj("Programiranje ili računarsko programiranje (engl. programming) jeste vještina pomoću koje ");
	ispitPRII.DodajSadrzaj("korisnik stvara i izvršava algoritme koristeći određene programske jezike da bi ... ");
	cout << "Broj stranica -> " << ispitPRII._brojStranica << endl;

	ispitPRII.Ispis();//ISPISUJE SVE PODATKE O DOKUMENTU

	ispitMAT.DodajSadrzaj("matematika se razvila iz potrebe da se obavljaju proračuni u trgovini, vrše mjerenja zemljišta i predviđaju ");
	ispitMAT.DodajSadrzaj("astronomski događaji, i ove tri primjene se mogu dovesti u vezu sa grubom podjelom matematike na RAT i ");

	Printer hp3200; hp3200.Unos("HP", "3200");
	/*PRINTER NEĆE DOZVOLITI PRINTANJE DOKUMENATA U ČIJEM SADRŽAJU SE NALAZI NEKA OD ZABRANJENIH RIJEČI DEFINISANIH FILTERIMA*/
	if (hp3200.DodajFilter("RE", NIZAK)) //VISOK :)
		cout << "Filter dodan!" << endl;
	if (hp3200.DodajFilter("RAT", VISOK))
		cout << "Filter dodan!" << endl;
	if (hp3200.DodajFilter("UBITI", VISOK))
		cout << "Filter dodan!" << endl;
	if (hp3200.DodajFilter("MRZITI", SREDNJI))
		cout << "Filter dodan!" << endl;
	if (hp3200.DodajFilter("RE", SREDNJI))/* ONEMOGUĆITI PONAVLJANJE FILTERA, BEZ OBZIRA STO SU RAZLICITOG PRIORITETA*/
		cout << "Filter dodan!" << endl;

	/*
	DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDEĆA PRAVILA:
	1. NAZIV DOKUMENTA MOŽE SADRŽAVATI SAMO SLOVA, A EKSTENZIJA MOŽE BITI SAMO NEKA OD OZNAČENIH SA vrstaDokumenta
	2. SADRŽAJ DOKUMENTA U SEBI NE SMIJE POSJEDOVATI ZABRANJENE RIJEČ DEFINISANE FILTERIMA OSIM AKO SU ONE OZNAČENE SA NISKIM PRIORITETOM (PRIRITET NIZAK)

	UKOLIKO NEKI OD NAVEDENIH USLOVA NIJE ZADOVOLJEN FUNKCIJA PRINTAJ DOKUMENT TREBA SAČUVATI U NIZ _dokumenti, ALI TAJ DOKUMENT NE TREBA PRINTATI.
	UKOLIKO DOKUMENT ISPUNJAVA SVE USLOVE ZA PRINTANJE POTREBNO JE NA KONZOLU ISPISATI SADRŽAJ DOKUMENTA KOJI SE PRINTA, A SA CRT LINIJOM ODVAJATI STRANICE DOKUMENTA
	*/
	hp3200.Printaj(ispitPRII);
	hp3200.Printaj(ispitMAT);
	hp3200.Printaj(ispitUIT);
	hp3200.Printaj(ispitUITDrugiRok);

	//PROSJEČAN BROJ STRANICA = UKUPAN BROJ STRANICA / BROJ PRINTANIH DOKUMENATA
	cout << "Prosjecan broj printanih stranica je -> " << hp3200.GetProsjecanBrojStranicaPoDatumuIVrsti(prije3Dana, danas2, DOC) << crt;
	//VRAĆA FILTER KOJI SE NAJČEŠĆE KORISTIO U DOKUMENTIMA
	Filter * f = hp3200.GetNakoristenijiFilter();
	cout << "Najcesce koristeni sadrzaj filtera je -> " << f->_sadrzaj << crt;
	//PORED VRIJEDNOSTI SVIH OSTALIH ATRIBUTA, ISPISUJE SAMO ONE DOKUMENTE KOJI ZADOVOLJAVAJU SVA PRETHODNO DEFINISANA PRAVILA 
	hp3200.Ispis();
	/*MIJENJA ZNAKOVE ZABRANJENOG SADRŽAJA DOKUMENTA SA POSLANIM KARAKTEROM
	(BEZ OBZIRA NA PRIORITET) I NAKON TOGA PRINTA SADRŽAJ DOKUMENATA; npr.
	UBITI POSTAJE -----.
	RIJEŠITI REKURZIJOM*/
	ZamijeniIPrintaj(hp3200, '-'); //OSTALE PARAMETRE DODAJTE PO POTREBI

	hp3200.Ispis();

	prije3Dana.Dealociraj(); danas1.Dealociraj(); danas2.Dealociraj(); za10Dana.Dealociraj();
	ispitMAT.Dealociraj(); ispitPRII.Dealociraj(); ispitUIT.Dealociraj(); ispitUITDrugiRok.Dealociraj();

	hp3200.Dealociraj();

	system("pause");
}