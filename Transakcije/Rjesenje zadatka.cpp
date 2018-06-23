/*

Pitanja na -> -> emir.veledar@edu.fit.ba

*/
#include<iostream>
using namespace std;
const char * crt = "\n---------------------------\n";
int ODG;

enum VrstaRacuna { TEKUCI, DEVIZNI, STEDNI };
enum VrstaTransakcije { UPLATA, ISPLATA };

// ovo zaboravite sluzi samo za "normalno" ispisivanje enumeracije umjesto integer-a
ostream &operator <<(ostream &COUT, const VrstaTransakcije &vrsta) {
	if (vrsta == 0)
		cout << "UPLATA";
	else if (vrsta == 1)
		cout << "ISPLATA";
	return COUT;
}
// ovo takodjer...
ostream &operator <<(ostream &COUT, const VrstaRacuna &vrsta) {
	if (vrsta == 0)
		cout << "TEKUCI";
	else if (vrsta == 1)
		cout << "DEVIZNI";
	else if (vrsta == 2)
		cout << "STEDNI";
	return COUT;
}

struct Datum {
	int _dan, _mjesec, _godina;
	/*	Unos / Ispis */
	void unos(int dan, int mjesec, int godina);
	void ispis();
};

void Datum::unos(int dan, int mjesec, int godina) {
	_dan = dan;	_mjesec = mjesec;	_godina = godina; // unos podataka u strukturu Datum
}
void Datum::ispis() {
	cout << _dan << "/" << _mjesec << "/" << _godina << ". godina" << endl; // ispis Datuma
}

struct Transakcija {
	char _TO[20];
	float _iznos;
	VrstaTransakcije _vrstaTransakcije; // ako je isplata vrijednost _TO postaviti na brojRacuna sa koga se transakcija vrsi
	bool _aktivna; //u slucaju da se transakcija izbrise ona postaje neaktivna, ali je bitno da o njoj ostane zapis
				   /*	Unos / Ispis */
	void Unos(char *brojRacuna, float iznos, VrstaTransakcije vrsta);
	void Ispis();
	void KopirajTransakcije(Transakcija t);
};

void Transakcija::Unos(char *brojRacuna, float iznos, VrstaTransakcije vrsta) {
	strcpy_s(_TO, 20, brojRacuna);
	_vrstaTransakcije = vrsta;
	if (iznos > 0)
		_iznos = iznos;
	else
		_iznos = 0;
	_aktivna = true;
}
// ispis transakcije
void Transakcija::Ispis() {
	cout << crt;
	cout << "Broja racuna -> " << _TO << endl;
	cout << "Vrsta transakcije -> " << _vrstaTransakcije << endl;
	cout << "Iznos -> " << _iznos << endl;
	if (_aktivna) cout << "Transakcija je aktivna" << endl;
	else cout << "Transkacija nije aktivna" << endl;
	cout << crt;
}
// kopiraj iz jedne transakciju u drugu
void Transakcija::KopirajTransakcije(Transakcija t) {
	strcpy_s(_TO, 20, t._TO);
	_vrstaTransakcije = t._vrstaTransakcije;
	_iznos = t._iznos;
	_aktivna = t._aktivna;
}

struct Racun {
	VrstaRacuna _vrstaRacuna;
	char _brojRacuna[20];
	Transakcija * _transakcije; //za svaki racun ce se evidentirati transakcije
	int _brojTransakcija;
	bool _aktivan;
	/*
	Unos / Ispis / Dealociraj / DodajTransakciju / UkloniTransakciju
	*/
	void Unos(VrstaRacuna vrsta, char *brojRacuna);
	void Ispis();
	void Dealociraj();
	void DodajTransakciju(char *brojRacuna, float iznos, VrstaTransakcije vrsta);
	void KopirajRacune(Racun r);
	void UkloniTranskaciju(int redni_broj_transkacije);
};
// unos racuna
void Racun::Unos(VrstaRacuna vrsta, char *brojRacuna) {
	_vrstaRacuna = vrsta;
	strcpy_s(_brojRacuna, 20, brojRacuna);
	_transakcije = nullptr;
	_brojTransakcija = 0;
	_aktivan = true;
}
// ispis racuna
void Racun::Ispis() {
	cout << crt;
	cout << "Vrsta racuna -> " << _vrstaRacuna << endl;
	cout << "Broj racuna -> " << _brojRacuna << endl;
	cout << "Transakcije: ";
	for (int i = 0; i < _brojTransakcija; i++)
		_transakcije->Ispis();
	if (_aktivan) cout << "Racun je aktivan" << endl;
	else cout << "Racun nije aktivan" << endl;
	cout << crt;
}
// oslobadjanje memorije koju je zauzeo objekat tipa "Racun"...
void Racun::Dealociraj() {
	if (_transakcije != nullptr) {
		delete[] _transakcije;
	}
}
// dodavanje transakcije dinamickim prosirivanjem niza tipa "Transakcija"
void Racun::DodajTransakciju(char *brojRacuna, float iznos, VrstaTransakcije vrsta) {
	Transakcija *temp = new Transakcija[_brojTransakcija + 1];
	for (int i = 0; i < _brojTransakcija; i++)
		temp[i].KopirajTransakcije(_transakcije[i]);

	temp[_brojTransakcija].Unos(brojRacuna, iznos, vrsta);
	delete[] _transakcije;
	_transakcije = temp;
	_brojTransakcija++;
}
void Racun::UkloniTranskaciju(int redni_broj_transkacije) {
	_transakcije[redni_broj_transkacije]._aktivna = false; // Deaktiviranje transakcije
}
// kopiranje atributa iz jednog racuna u drugi
void Racun::KopirajRacune(Racun r) {
	_vrstaRacuna = r._vrstaRacuna;
	strcpy_s(_brojRacuna, 20, r._brojRacuna);
	_transakcije = new Transakcija[r._brojTransakcija];
	for (int i = 0; i < r._brojTransakcija; i++)
		_transakcije[i].KopirajTransakcije(r._transakcije[i]);
	_brojTransakcija = r._brojTransakcija;
	_aktivan = r._aktivan;
}
struct Korisnik {
	char _JMBG[14];
	char * _imePrezime;
	Racun * _racuni;
	int _trenutnoRacuna; //brojac
	bool _aktivan;

	/*
	Unos / Ispis / Dealociraj / DodajRacun / UkloniRacun /
	DodajTransakciju - pored iznosa, korisnik vrsi odabir vrste transakcije, te racuna koji zeli koristiti za tu transakciju; onemoguciti isplate vece kolicine novca nego sto korisnik posjeduje na racunu
	GetTransakcijeByIznos - vraca niz transakcija koje su imale iznos veci od trazenog
	GetTransakcije - vraca listu transakcija koje su izvrsena prema odredjenom racunu(_TO), ukljucujuci i neaktivne
	GetTransakcijeByDatum - vraca listu svih transakcija koje su obavljene
	*/
	void Unos(char *JMBG, char *ime_prezime);
	void Ispis();
	void Dealociraj();
	void DodajRacun(VrstaRacuna vrsta, char *brojRacuna);
	void UkloniRacun(int redni_broj_racuna);
	void KopirajKorisnika(Korisnik k);
	Transakcija *GetTransakcijeByIznos(float iznos, int &broj_transakcija);
	Transakcija *GetTransakcije(char *TO, int &broj_transakcija);
	Transakcija *GetTransakcijeByDatum(int &broj_transakcija);
};

// unos korisnika
void Korisnik::Unos(char *JMBG, char *ime_prezime) {
	strcpy_s(_JMBG, 14, JMBG);
	_imePrezime = new char[strlen(ime_prezime) + 1];
	strcpy_s(_imePrezime, strlen(ime_prezime) + 1, ime_prezime);
	_racuni = nullptr;
	_trenutnoRacuna = 0;
	_aktivan = true;
}
// ispis korisnika
void Korisnik::Ispis() {
	cout << "JMBG -> " << _JMBG << endl;
	cout << "Ime i Prezime -> " << _imePrezime << endl;
	for (int i = 0; i < _trenutnoRacuna; i++)
		_racuni[i].Ispis();
	if (_aktivan) cout << "Korisnik je aktivan" << endl;
	else cout << "Korisnik nije aktivan" << endl;
}
// oslobadjanje memorije koju je zauzeo objekat tipa "Korisnik"
void Korisnik::Dealociraj() {
	if (_racuni != nullptr) {
		for (int i = 0; i < _trenutnoRacuna; i++)
			_racuni[i].Dealociraj();
		delete[] _racuni; _racuni = nullptr;
	}
	delete[] _imePrezime; _imePrezime = nullptr;
}
// dodavanje racuna dinamickim prosirivanjem niza...
void Korisnik::DodajRacun(VrstaRacuna vrsta, char *brojRacuna) {
	Racun *temp = new Racun[_trenutnoRacuna + 1]; // kreiranje pomocnog niza
	for (int i = 0; i < _trenutnoRacuna; i++) // kopiranje vrijednosti objekata u pomocni 
		temp[i].KopirajRacune(_racuni[i]);
	temp[_trenutnoRacuna].Unos(vrsta, brojRacuna); // unos novog objekta
												   // brisanje starih racuna
	for (int i = 0; i < _trenutnoRacuna; i++)
		_racuni[i].Dealociraj();
	delete[] _racuni;

	_racuni = temp;	// preusmjeravanje pokazivaca "_racuni" na novi niz..
	_trenutnoRacuna++;	// povecanje brojaca racuna
}
void Korisnik::UkloniRacun(int redni_broj_racuna) {
	_racuni[redni_broj_racuna]._aktivan = false; // deaktiviranje racuna
}

// kopiranje starog korisnika u novi..
void Korisnik::KopirajKorisnika(Korisnik k) {
	strcpy_s(_JMBG, 14, k._JMBG);
	_imePrezime = new char[strlen(k._imePrezime) + 1];
	strcpy_s(_imePrezime, strlen(k._imePrezime) + 1, k._imePrezime);
	_racuni = new Racun[k._trenutnoRacuna];
	for (int i = 0; i < k._trenutnoRacuna; i++)
		_racuni[i].KopirajRacune(k._racuni[i]);
	_trenutnoRacuna = k._trenutnoRacuna;
	_aktivan = k._aktivan;
}

Transakcija *Korisnik::GetTransakcijeByIznos(float iznos, int &broj_transakcija) {
	Transakcija *transakcije = new Transakcija[broj_transakcija];
	for (int i = 0; i < _trenutnoRacuna; i++)
	{
		for (int j = 0; j < _racuni[i]._brojTransakcija; j++) {
			if (_racuni[i]._transakcije[j]._iznos > iznos  && _racuni[i]._transakcije[j]._aktivna) {
				Transakcija *temp = new Transakcija[broj_transakcija + 1];
				for (int k = 0; k < broj_transakcija; k++)
					temp[k].KopirajTransakcije(transakcije[k]);
				delete[] transakcije;
				temp[broj_transakcija++].KopirajTransakcije(_racuni[i]._transakcije[j]);
				transakcije = temp;
			}
		}
	}
	if (broj_transakcija == 0) return nullptr;
	return transakcije;
}
Transakcija *Korisnik::GetTransakcije(char *TO, int &broj_transakcija) {
	Transakcija *transakcije = new Transakcija[broj_transakcija];
	for (int i = 0; i < _trenutnoRacuna; i++)
		for (int j = 0; j < _racuni[i]._brojTransakcija; j++)
			if (strcmp(_racuni[i]._transakcije[j]._TO, TO) == 0) {
				Transakcija *temp = new Transakcija[broj_transakcija + 1];
				for (int k = 0; k < broj_transakcija; k++)
					temp[k].KopirajTransakcije(transakcije[k]);
				delete[] transakcije;
				temp[broj_transakcija++].KopirajTransakcije(_racuni[i]._transakcije[j]);
				transakcije = temp;
			}
	if (broj_transakcija == 0) return nullptr;
	return transakcije;
}
Transakcija *Korisnik::GetTransakcijeByDatum(int &broj_transakcija) {
	Transakcija *transakcije = new Transakcija[broj_transakcija];
	for (int i = 0; i < _trenutnoRacuna; i++)
		for (int j = 0; j < _racuni[i]._brojTransakcija; j++)
			if (_racuni[i]._transakcije[j]._aktivna) {
				Transakcija *temp = new Transakcija[broj_transakcija + 1];
				for (int k = 0; k < broj_transakcija; k++)
					temp[k].KopirajTransakcije(transakcije[k]);
				delete[] transakcije;
				temp[broj_transakcija++].KopirajTransakcije(_racuni[i]._transakcije[j]);
				transakcije = temp;
			}
	if (broj_transakcija == 0) return nullptr;
	return transakcije;
}

int PrikaziMenu() {
	int izbor;
	cout << crt << "\t::MENU::" << crt;
	cout << "1. Dodaj korisnika" << endl;
	cout << "2. Dodaj racun" << endl;
	cout << "3. Pretraga" << endl;
	cout << "4. Deaktiviranje korisnika" << crt; // racuna ili korisnika
	cout << "5. Kraj" << crt;
	do {
		cout << "Vas izbor -> ";
		cin >> izbor;
	} while (izbor < 1 || izbor>5);
	system("cls");
	return izbor;
}

void OcistiEkran() {
	system("cls");
}

void main() {
	int trenutnoKorisnika = 0, izborMenu = 0;
	Korisnik * korisnici = nullptr;
	int broj_transakcija = 0;
	Transakcija *trazene_transakcije = nullptr;


	do {
		izborMenu = PrikaziMenu();
		cin.ignore();
		if (izborMenu == 1) { // dodavanje korisnika
							  // mehanizam prosirivanja dinamickog niza ...
							  // 1 -> kreiranje pomocnog koji je za jedan veci od postojeceg zbog unosa zeljenok korisnika
			Korisnik *temp = new Korisnik[trenutnoKorisnika + 1];
			// 2 -> kopiranje vrijednosti korisnika iz starog niza u novi...
			for (int i = 0; i < trenutnoKorisnika; i++)
				temp[i].KopirajKorisnika(korisnici[i]);
			// 3 -> brisanje starog niza korisnika (oslobadjanje memorije)
			if (korisnici != nullptr) {
				for (int i = 0; i < trenutnoKorisnika; i++)
					korisnici[i].Dealociraj();
				delete[] korisnici;
			}

			// 4 -> unos novog korisnika 
			char unosTemp[20], ime_prezime_temp[40];

			cout << "Unesite JMBG -> ";
			cin.getline(unosTemp, 20);

			cout << "Unesite ime i prezime -> ";
			cin.getline(ime_prezime_temp, 40);

			temp[trenutnoKorisnika].Unos(unosTemp, ime_prezime_temp);

			// 5 -> spremanje u pokazivac adrese tog niza 
			korisnici = temp;

			// 6 -> uvecanje brojaca korisnika
			trenutnoKorisnika++;
		}
		else if (izborMenu == 2) {
			if (korisnici != nullptr) {	// provjera da li postoji korisnika ako ne postoji ne moguce je dodati racun....
				int temp, vrsta_racuna;
				VrstaRacuna enum_vrsta;
				char broj_racuna[20];
				// ispis svih korisnika
				for (int i = 0; i < trenutnoKorisnika; i++)
				{
					cout << crt;
					cout << "Redni broj -> " << i + 1 << endl;
					korisnici[i].Ispis();
					cout << crt;
				}
				// izbor korisnika
				cout << "Unesite redni broj korisnika kojem zelite dodati racun -> ";
				do {
					cin >> temp;
					if (temp<1 || temp>trenutnoKorisnika)
						cout << "Pogresan unos, unesite ponovo -> ";

				} while (temp<1 || temp>trenutnoKorisnika);
				OcistiEkran(); // ciscenje ekrana
							   // izbor vrste racuna
				cout << "Unesi vrstu racuna 1. TEKUCI, 2. DEVIZNI, 3. STEDNI" << endl;
				do
				{
					cin >> vrsta_racuna;
					if (vrsta_racuna<1 || vrsta_racuna>3)
						cout << "Pogresan unos, unosite ponovo -> ";
				} while (vrsta_racuna<1 || vrsta_racuna>3);

				enum_vrsta = (VrstaRacuna)--vrsta_racuna;
				cin.ignore();
				OcistiEkran(); // ciscenje ekrana
							   // unos broja racuna
				cout << "Unesite broj racuna -> ";
				cin.getline(broj_racuna, 20);

				// dodavanje racuna korisniku
				korisnici[--temp].DodajRacun(enum_vrsta, broj_racuna);
			}
		}
		else if (izborMenu == 3) {
			if (korisnici != nullptr) {	// provjera da li postoji korisnika ako ne postoji ne moguce je dodati racun....
				char unos_korisnika[20];
				cout << "Unesite ime ili prezime korisnika -> ";
				cin.getline(unos_korisnika, 20);
				for (int i = 0; i < trenutnoKorisnika; i++)
				{
					OcistiEkran(); // ciscenje ekrana
					char *pok = strstr(korisnici[i]._imePrezime, unos_korisnika);
					if (pok != nullptr) {
						char karakter;
						cout << crt;  korisnici[i].Ispis(); cout << crt;
						cout << "Da li zelite ovom korisniku dodati transakciju (D/N) -> ";
						do
						{
							cin >> karakter;
							cin.ignore();
							if (toupper(karakter) != 'D' && toupper(karakter) != 'N')
								cout << "Pogresan unos, unesite ponovo -> ";
						} while (toupper(karakter) != 'D' && toupper(karakter) != 'N');
						if (toupper(karakter) == 'D') {

							int unos; float iznos;
							VrstaTransakcije v;
							char broj_racuna[20];
							cout << "Odaberite vrstu transakcije (1. UPLATA, 2. ISPLATA) -> ";
							do {
								cin >> unos;
								if (unos != 1 && unos != 2)
									cout << "Pogresan unos, unesite ponovo -> ";
							} while (unos != 1 && unos != 2);

							v = (VrstaTransakcije) --unos;
							OcistiEkran(); // ciscenje ekrana
										   // izbor racuna koji se zeli koristiti za transakciju
										   // ispis racuna
							for (int j = 0; j < korisnici[i]._trenutnoRacuna; j++)
							{
								cout << crt << "Redni broj " << j + 1 << endl;
								korisnici[i]._racuni[j].Ispis();
								cout << crt;
							}
							// izbor racuna
							cout << "Unesite redni broj racuna koji ce se koristiti za transakciju -> ";
							do
							{
								cin >> unos;
								if (unos<1 || unos>korisnici[i]._trenutnoRacuna)
									cout << "Pogresan unos, unesite ponovo";
							} while (unos<1 || unos>korisnici[i]._trenutnoRacuna);
							unos--;
							OcistiEkran(); // ciscenje ekrana
							cout << "Unesite iznos za " << v << " -> ";
							cin >> iznos;

							korisnici[i]._racuni[unos].DodajTransakciju(korisnici[i]._racuni[unos]._brojRacuna, iznos, v);
						}
					}
				}
			}

		}
		else if (izborMenu == 4) {
			if (korisnici != nullptr) { // u slucaju da nema korisnika onemoguceno izvrsavanje ove funkcije
				int izbor_temp, izbor_korisnika_temp, izbor_racuna_temp;
				cout << "Sta zelite deaktivirat: 1. KORISNIKA, 2. RACUN -> ";
				// unos izbora brisanja
				// 1->Brisanje korisnika
				// 2 -> Brisanje racuna
				do
				{
					cin >> izbor_temp;
					if (izbor_temp != 1 && izbor_temp != 2)
						cout << "Pogresan unos, unesite ponovo -> ";
				} while (izbor_temp != 1 && izbor_temp != 2);

				// ispis svih korisnika
				for (int i = 0; i < trenutnoKorisnika; i++)
				{
					cout << crt << i + 1 << endl;
					korisnici[i].Ispis();
					cout << crt;
				}

				cout << "Unesite redni broj korisnika -> ";
				do
				{
					cin >> izbor_korisnika_temp;	//izbor korisnika
					if (izbor_korisnika_temp<1 || izbor_korisnika_temp>trenutnoKorisnika)
						cout << "Pogresan unos, unesite ponovo -> ";

				} while (izbor_korisnika_temp<1 || izbor_korisnika_temp>trenutnoKorisnika);	// provjera validnosti unosa
				izbor_korisnika_temp--;
				if (izbor_temp == 1) {
					korisnici[izbor_korisnika_temp]._aktivan = false; // deaktiviranje korisnika
				}
				else {
					system("cls"); // ciscenje ekrana
					if (korisnici[izbor_korisnika_temp]._trenutnoRacuna > 0) { // u slucaju da izabrani korisnik nema kreirani racuna onemoguceno izvrsavanje ove funkcije....

						for (int i = 0; i < korisnici[izbor_korisnika_temp]._trenutnoRacuna; i++) // ispis svih racuna korisnika
						{
							cout << crt << "Redni broj " << i + 1 << endl;
							korisnici[izbor_korisnika_temp]._racuni[i].Ispis();
							cout << crt;
						}
						cout << "Unesite redni broj racuna -> ";
						do {
							cin >> izbor_racuna_temp;	// izbor racuna
							if (izbor_racuna_temp<1 || izbor_racuna_temp>korisnici[izbor_korisnika_temp]._trenutnoRacuna)
								cout << "Pogresan unos, unesite ponovo -> ";

						} while (izbor_racuna_temp<1 || izbor_racuna_temp>korisnici[izbor_korisnika_temp]._trenutnoRacuna); // provjera validnosti unosa

						korisnici[izbor_korisnika_temp]._racuni[--izbor_racuna_temp]._aktivan = false; // deaktiviranje racuna
					}
				}
			}
		}

		system("cls");
	} while (izborMenu != 5);

	// oslobadjanje svih zauzetih resursa memorije
	if (korisnici != nullptr) {
		for (int i = 0; i < trenutnoKorisnika; i++)
			korisnici[i].Dealociraj();
		delete[] korisnici; korisnici = nullptr;
	}

}