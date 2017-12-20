#include<iostream>
#include <memory>
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
		cout << _naziv << " (" << *_ocjena << ") "; _datumUnosa.Ispis();
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
		_predmeti = nullptr;
		_brojPredmeta = 0;
	}
	void Dealociraj() {
		for (size_t i = 0; i < _brojPredmeta; i++)
			_predmeti[i]->Dealociraj();
		delete[] _predmeti; _predmeti = nullptr;
	}
	void Ispis() {
		cout << crt << "Razred -> " << _razred << "Najbolji -> " << _najboljiUcenik << crt;
		for (size_t i = 0; i < _brojPredmeta; i++)
			_predmeti[i]->Ispis();
	}
	void DodajPredmet(Predmet p){
		shared_ptr<Predmet> *temp = new shared_ptr<Predmet>[_brojPredmeta + 1];
		for (size_t i = 0; i < _brojPredmeta; i++)
		{
			temp[i] = make_shared<Predmet>();
			temp[i]->Unos(_predmeti[i]->_naziv, *_predmeti[i]->_ocjena, _predmeti[i]->_datumUnosa);
			_predmeti[i]->Dealociraj();
		}
		if (_predmeti != nullptr) delete[] _predmeti;
		temp[_brojPredmeta] = make_shared<Predmet>();
		temp[_brojPredmeta]->Unos(p._naziv, *p._ocjena, p._datumUnosa);
		_predmeti = temp;
		_brojPredmeta++;
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
			if (_uspjeh[i] != nullptr){
				_uspjeh[i]->Dealociraj();
				delete _uspjeh[i];
			}
		}
	}
	void Ispis() {
		cout << crt << _imePrezime << " " << _nacinStudiranja;
		for (size_t i = 0; i < 4; i++)
		if (_uspjeh[i] != nullptr)
			_uspjeh[i]->Ispis();
	}
	// funckija za proglasavanje u kojem je razredu ucenik ostvario najbolji uspjeh
	void ProglasiNajboljeg(){
		// postavljanje svim razredima na false
		for (size_t i = 0; i < 4; i++)
			if (_uspjeh[i] != nullptr)
				_uspjeh[i]->_najboljiUcenik = false;

		float trenutni_uspjeh = 0;
		float naj_uspjeh = 0;
		Uspjeh *najUspjehPok = nullptr; // pokazivac na najbolji uspjeh preko koga cemo modifikovat vrijednost
		for (size_t i = 0; i < 4; i++)
		{
			trenutni_uspjeh = 0;
			if (_uspjeh[i] != nullptr){

				for (size_t j = 0; j < _uspjeh[i]->_brojPredmeta; j++)
				{
					trenutni_uspjeh = trenutni_uspjeh + *_uspjeh[i]->_predmeti[j]->_ocjena;
				}
				trenutni_uspjeh = trenutni_uspjeh / _uspjeh[i]->_brojPredmeta;
				if (trenutni_uspjeh > naj_uspjeh){
					naj_uspjeh = trenutni_uspjeh;
					najUspjehPok = _uspjeh[i];
				}
				
			}
		}
		// postavljanje _najboljiUcenik na true
		najUspjehPok->_najboljiUcenik = true;
	}
	// funkcija koja vraca broj dana na osnovu proslijedjenog datuma
	int VratiDane(Datum d){
		return d._dan + d._mjesec * 30 + d._godina * 365;
	}
	// funkcija za dodavanje predmeta
	bool DodajPredmet(eRazred razred, Predmet p){
		int brojac_predemta = 0;
		// brojanje koliko imamo dodani predmeta ovog naziva
		for (size_t i = 0; i < 4; i++)
		{
			if (_uspjeh[i] != nullptr){
				for (size_t j = 0; j < _uspjeh[i]->_brojPredmeta; j++)
				{
					if (strcmp(_uspjeh[i]->_predmeti[j]->_naziv, p._naziv) == 0)
						brojac_predemta++;
				}
			}
		}
		// u slucaju da imamo 2 predmeta sa ovim nazivom 
		// onemogucavamo unos ovog predmeta ponovo
		if (brojac_predemta >= 2) return false;
		// u slucaju da je rok za prijavu istekao onemogucavamo unos
		// OBJASNJENJE USLOVA -> ->
		//Ako neki datum ima vise dana u odnosu na rok za prijavu znaci da je taj datum poslije roka za prijavu 
		// sto znaci da je prosao
		// Primjer -> ->
		// rok za prijavu je 5.7.2017
		// a datum koji primamo je 6.7.2017 <- <- <- ovaj datum ima vise dana u odnosu na rok za prijavu sto znaci da je "istekao" 
		if (VratiDane(p._datumUnosa) >= VratiDane(rokZaPrijavu)) return false;
		// ukoliko se ni jedan od uslova ne ispuni znaci da mozemo dodati predmet 
		// provjera da li je uspjeh vec alociran u slucaju da jest nece se izvrsiti ponovna alokacija
		// u slucaju da nije vrsi se alokacija pa se dodaje predmet
		if (_uspjeh[(int)razred - 1] == nullptr){
			_uspjeh[(int)razred - 1] = new Uspjeh;
			// u pocetku stavljamo da je najbolji false
			_uspjeh[(int)razred - 1]->Unos(razred, false);
		}
		// dodavanje predmeta
		_uspjeh[(int)razred - 1]->DodajPredmet(p);
		// funkcija za proglasavanje u kojem se razredu nalazi najbolji ucenik
		ProglasiNajboljeg();
		
		return true;
	}
};

Kandidat *rekNajboljiProsjek(Kandidat *kandidati, int brojKandidata, eRazred razred, Kandidat *najboljiKandidat = nullptr, float prosjekNajboljegKandidata = 0){
	if (brojKandidata - 1 <= 0)
		return najboljiKandidat;
	if (kandidati[brojKandidata - 1]._uspjeh[(int)razred - 1] == nullptr)
		return rekNajboljiProsjek(kandidati, brojKandidata - 1, razred, najboljiKandidat, prosjekNajboljegKandidata);
	// racunanje prosjeka
	float prosjek = 0;
	for (size_t i = 0; i < kandidati[brojKandidata-1]._uspjeh[(int)razred-1]->_brojPredmeta; i++)
	{
		prosjek = prosjek + *kandidati[brojKandidata - 1]._uspjeh[(int)razred - 1]->_predmeti[i]->_ocjena;
	}
	prosjek = prosjek / kandidati[brojKandidata - 1]._uspjeh[(int)razred - 1]->_brojPredmeta;
	if (najboljiKandidat == nullptr)
		return rekNajboljiProsjek(kandidati, brojKandidata - 1, razred, &kandidati[brojKandidata - 1], prosjek);
	if (prosjek>prosjekNajboljegKandidata)
		return rekNajboljiProsjek(kandidati, brojKandidata - 1, razred, &kandidati[brojKandidata - 1], prosjek);
	return rekNajboljiProsjek(kandidati, brojKandidata - 1, razred, najboljiKandidat, prosjekNajboljegKandidata);
	
}

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
	auto najboljiUspjeh = [prijave2017, brojKandidata](eNacinStudiranja nacin, bool najbolji){
		
		float trenutniProsjek = 0;
		float najboljiProsjek = 0;
		Uspjeh *najboljiUspjeh = nullptr;
		
		for (size_t i = 0; i < brojKandidata; i++)
		{
			if (prijave2017[i]._nacinStudiranja == nacin){ // provjera za nacin studiranja koji je proslijedjen
				for (size_t j = 0; j < 4; j++)
				{
					trenutniProsjek = 0;
					if (prijave2017[i]._uspjeh[j] != nullptr && prijave2017[i]._uspjeh[j]->_najboljiUcenik == najbolji){ // provjera samo za one razrede u kojima je kandidat proglasen najboljim
						for (size_t k = 0; k < prijave2017[i]._uspjeh[j]->_brojPredmeta; k++)
						{
							trenutniProsjek = trenutniProsjek + *prijave2017[i]._uspjeh[j]->_predmeti[k]->_ocjena;
						}
						trenutniProsjek = trenutniProsjek / prijave2017[i]._uspjeh[j]->_brojPredmeta;
						if (trenutniProsjek > najboljiProsjek){
							najboljiProsjek = trenutniProsjek;
							najboljiUspjeh = prijave2017[i]._uspjeh[j];
						}
					}
				}
			}
		}

		return najboljiUspjeh;
	};
	Uspjeh * najbolji = najboljiUspjeh(DL, true);
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
