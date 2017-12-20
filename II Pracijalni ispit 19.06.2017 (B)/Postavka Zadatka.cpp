#include<iostream>
using namespace std;
/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NEÆE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB160061.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ÆETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/

//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

char *crt = "\n-------------------------------------------\n";

enum eNacinStudiranja { REDOVAN, DL };
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };

struct Datum {
	int _dan, _mjesec, _godina;
	void Unos(int d, int m, int g) { _dan = d; _mjesec = m; _godina = g; }
	void Ispis() { cout << _dan << "/" << _mjesec << "/" << _godina << endl; }
};

const Datum rokZaPrijavu = { 5, 7, 2017 };

char * errVecDodan = "Istoimeni predmet se moze dodati najvise dva puta!";
char * errProsaoRok = "Zao nam je. Rok za prijavu je vec prosao!";
char * uspjesnoDodan = "Predmet uspjesno dodan!";

struct Predmet {
	char * _naziv;
	int * _ocjena;
	Datum _datumUnosa;
	void Unos(char * naziv, int ocjena, Datum datumUnosa) {
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);
		_ocjena = new int(ocjena);
		_datumUnosa.Unos(datumUnosa._dan, datumUnosa._mjesec, datumUnosa._godina);
	}
	void Dealociraj() {
		delete[] _naziv; _naziv = nullptr;
		delete _ocjena; _ocjena = nullptr;
	}

	void Ispis() {
		cout << _naziv << " (" << _ocjena << ") "; _datumUnosa.Ispis();
	}
	void PromijeniOcjenu(int ocjena) { *_ocjena = ocjena; }
};

struct Uspjeh {
	eRazred  _razred;
	bool _najboljiUcenik; // ako je kandidat u tom razredu proglasen najboljim ucenikom
	shared_ptr<Predmet> * _predmeti;
	int _brojPredmeta;
	void Unos(eRazred razred, bool najboljiUcenik) {
		_razred = razred;
		_najboljiUcenik = najboljiUcenik;
		_brojPredmeta = 0;
	}
	void Dealociraj() {

	}
	void Ispis() {
		cout << crt << "Razred -> " << _razred << "Najbolji -> " << _najboljiUcenik << crt;
		for (size_t i = 0; i < _brojPredmeta; i++)
			_predmeti[i]->Ispis();
	}
};

struct Kandidat {
	eNacinStudiranja _nacinStudiranja;
	char * _imePrezime;
	Uspjeh * _uspjeh[4];

	void Unos(eNacinStudiranja nacinStudiranja, char * imePrezime) {
		int vel = strlen(imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, imePrezime);
		_nacinStudiranja = nacinStudiranja;
		for (size_t i = 0; i < 4; i++)
			_uspjeh[i] = nullptr;
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < 4; i++){
			_uspjeh[i]->Dealociraj();
			delete _uspjeh[i];
		}
	}
	void Ispis() {
		cout << crt << _imePrezime << " " << _nacinStudiranja;
		for (size_t i = 0; i < 4; i++)			
			_uspjeh[i]->Ispis();
	}	
};

void main(){
	Datum datum19062017, datum20062017, datum30062017, datum05072017;
	datum19062017.Unos(19, 6, 2017);
	datum20062017.Unos(20, 6, 2017);
	datum30062017.Unos(30, 6, 2017);
	datum05072017.Unos(5, 7, 2017);

	Predmet Matematika, Fizika, Hemija, Engleski;
	//2 - ocjena na predmetu; datum - datum evidentiranja uspjeha na predmetu jer postoji krajnji rok za evidentiranje
	Matematika.Unos("Matematika", 2, datum19062017);
	Fizika.Unos("Fizika", 5, datum20062017);
	Hemija.Unos("Hemija", 2, datum20062017);
	Engleski.Unos("Engleski", 5, datum05072017);

	int brojKandidata = 2;

	Kandidat * prijave2017 = new Kandidat[brojKandidata];
	prijave2017[0].Unos(DL, "Jasmin Azemovic");
	prijave2017[1].Unos(REDOVAN, "Indira Hamulic");
	/*
uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda. prilikom dodavanja onemoguciti:
	- istoimeni predmet se moze dodati samo dva puta, bez obzira sto se dodaje na razlicitim godinama,
	- dodavanje predmeta nakon dozvoljenog roka za prijavu (rokZaPrijavu).
	razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.). Funkcija vraca neku od predefinisanih poruka u zavisnosti od (ne)uspjesnost izvrsenja
	*/
	if (prijave2017[0].DodajPredmet(DRUGI, Engleski)) //ne bi trebao dodati jer je prosao postavljeni rok za dodavanje predmeta
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[0].DodajPredmet(DRUGI, Matematika))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[0].DodajPredmet(PRVI, Fizika))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[0].DodajPredmet(PRVI, Hemija))
		cout << "Predmet uspjesno dodan!" << crt;

	Matematika.PromijeniOcjenu(5);
	Hemija.PromijeniOcjenu(3);
	
	if (prijave2017[1].DodajPredmet(PRVI, Matematika))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(DRUGI, Matematika))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(TRECI, Matematika))//ne bi trebalo ponovo dodati Matematiku!
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(TRECI, Hemija))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(DRUGI, Engleski))
		cout << "Predmet uspjesno dodan!" << crt;

	/*
koristeci Lambda izraz kreirati funkciju koja ce vratiti uspjeh kandidata koji je ostvario najveci prosjek (na nivou razreda, a ne ukupni prosjek).funkcija prima parametre tipa eNacinStudiranja (pretrazuje samo kandidate za Redovan ili DL nacin studiranja) i bool kojim se omogucava pretraga samo onih razreda na kojim su kandidati bili najbolji ucenici (ako se funkciji proslijedi vrijednost true, ona ce uzeti u obzir samo one razrede kada su kandidati oznaceni kao najbolji). ukoliko vise kandidata ima isti prosjek funkcija vraca uspjeh (najboljeg razreda) prvog pronadjenog kandidata
	*/	
	auto najboljiUspjeh = /*...*/;
	Uspjeh * najbolji = najboljiUspjeh(DL,true);
	najbolji->Ispis();
	
	/*
napisati rekurzivnu funkciju koja ce vratiti pokazivac na kandidata sa najvecim prosjekom u razredu koji je proslijedjen kao parametar. ukoliko je vise kandidata ostvarilo isti prosjek, funkcija treba da vrati posljednje pronadjenog kandidata. u slucaju da niti jedan kandidat nije evidentirao uspjeh u trazenom razredu, funkcija vraca nullptr. u nastavku je prikazan primjer poziva rekurzivne funkcije, a ostale parametre dodajte po potrebi. */
	Kandidat * kandidatSaNajboljimProsjekom = rekNajboljiProsjek(prijave2017, brojKandidata, DRUGI);
		
	for (size_t i = 0; i < brojKandidata; i++)
	{
		prijave2017[i].Ispis();
		prijave2017[i].Dealociraj();
	}
	system("pause>0");
}
