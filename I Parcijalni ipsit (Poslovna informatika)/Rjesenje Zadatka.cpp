#include<iostream>
#include <conio.h>
using namespace std;
const char * crt = "\n---------------------------\n";
struct Datum {
	int dan, mjesec, godina;
};
struct PolozeniPredmet {
	char * nazivPredmeta;
	int ocjena;
	Datum datumPolaganja;
	char * napomena;
};
struct Student {
	char * imePrezime;
	PolozeniPredmet polozeni[40];
	int brojPolozenih;
};
int PrikaziMenu() {
	int izbor;
	cout << crt << "\t::MENU::" << crt;
	cout << "1. Dodaj studenta" << endl;
	cout << "2. Dodaj polozeni predmet" << endl;
	cout << "3. Pretraga" << endl;
	cout << "4. Kraj" << crt;
	do {
		cout << "Vas izbor -> ";
		cin >> izbor;
	} while (izbor < 1 || izbor>4);
	system("cls");
	return izbor;
}
/*UKOLIKO SMATRATE DA JE POTREBNO ZADATAK MOZETE PROSIRITI DODATNIM FUNKCIONALNOSTIMA, ALI PROSIRENJA MORAJU PRATITI ADEKVATNI KOMENTARI*/
void Login(char *valid_pass) {
	char password[100];
	char znak; int brojac;
	do {
		brojac = 0;
		cout << "Unesite vas password -> ";
		do
		{
			znak = _getch();

			if ((int)znak != 13) {
				password[brojac++] = znak;
				cout << "-";
			}
		} while ((int)znak != 13);
		password[brojac] = '\0';
		if (strcmp(password, valid_pass) != 0) {
			system("cls");
			cout << "Pogresna sifra... Pokusajte ponovo" << endl;
		}
	} while (strcmp(valid_pass, password) != 0);
}
void DodajStudenta(Student *&student) {
	char temp[100];
	int ocjena;
	int broj_predmeta;
	student = new Student;

	cout << "Unesite ime i prezime -> ";
	cin.getline(temp, 100);
	int size = strlen(temp) + 1;
	int d, m, g;
	student->imePrezime = new char[size];
	strcpy_s(student->imePrezime, size, temp);

	cout << "Koliko predmeta dodajete -> ";
	cin >> broj_predmeta;
	cin.ignore();

	for (int i = 0; i < broj_predmeta; i++)
	{
		cout << "Naziv predmeta -> ";
		cin.getline(temp, 100);
		size = strlen(temp) + 1;
		student->polozeni[i].nazivPredmeta = new char[size];
		strcpy_s(student->polozeni[i].nazivPredmeta, size, temp);

		cout << "Datum Polaganja -> ";
		cin >> d >> m >> g;
		cin.ignore();
		student->polozeni[i].datumPolaganja.dan = d;
		student->polozeni[i].datumPolaganja.mjesec = m;
		student->polozeni[i].datumPolaganja.godina = g;

		cout << "Napomena -> ";
		cin.getline(temp, 100);
		size = strlen(temp) + 1;
		student->polozeni[i].napomena = new char[size];
		strcpy_s(student->polozeni[i].napomena, size, temp);


		cout << "Ocjena -> ";
		cin >> ocjena;
		cin.ignore();
		student->polozeni[i].ocjena = ocjena;

	}
		student->brojPolozenih = broj_predmeta;

}
void DodajPolozeniPredmet(Student *studenti[], int max) {
	char ime_prezime[100];
	char temp[100];
	int size, d, m, g, ocjena;
	cout << "Unesite ime i prezime -> ";
	cin.getline(ime_prezime, 100);
	for (int i = 0; i < max; i++)
	{
		if (strcmp(studenti[i]->imePrezime, ime_prezime) == 0) {
			cout << "Student pronadjen..." << endl;

			cout << "Naziv predmeta -> ";
			cin.getline(temp, 100);
			size = strlen(temp) + 1;
			studenti[i]->polozeni[studenti[i]->brojPolozenih].nazivPredmeta = new char[size];
			strcpy_s(studenti[i]->polozeni[studenti[i]->brojPolozenih].nazivPredmeta, size, temp);

			cout << "Datum Polaganja -> ";
			cin >> d >> m >> g;
			cin.ignore();
			studenti[i]->polozeni[studenti[i]->brojPolozenih].datumPolaganja.dan = d;
			studenti[i]->polozeni[studenti[i]->brojPolozenih].datumPolaganja.mjesec = m;
			studenti[i]->polozeni[studenti[i]->brojPolozenih].datumPolaganja.godina = g;

			cout << "Napomena -> ";
			cin.getline(temp, 100);
			size = strlen(temp) + 1;
			studenti[i]->polozeni[studenti[i]->brojPolozenih].napomena = new char[size];
			strcpy_s(studenti[i]->polozeni[studenti[i]->brojPolozenih].napomena, size, temp);

			cout << "Ocjena -> ";
			cin >> ocjena;
			cin.ignore();
			studenti[i]->polozeni[studenti[i]->brojPolozenih].ocjena = ocjena;

			studenti[i]->brojPolozenih++;

		}
	}
}
int PretvoriUDane(Datum d) {
	return d.godina * 365 + d.mjesec * 30 + d.dan;
}
void Pretraga(Student *studenti[], int max) {
	int izbor;
	cout << "Pratraga (1. Datumu polaganja, 2. Napomeni) ->";
	cin >> izbor;
	cin.ignore();
	if (izbor == 1) {
		int od_d, od_m, od_g;
		int do_d, do_m, do_g;
		cout << "Unesite datum od -> ";
		cin >> od_d >> od_m >> od_g;
		cout << "Unesite datum do -> ";
		cin >> do_d >> do_m >> do_g;
		Datum OD, DO;
		OD.dan = od_d, OD.mjesec = od_m, OD.godina = od_g;
		DO.dan = do_d, DO.mjesec = do_m, DO.godina = do_g;
		cin.ignore();
		cout << "U periodu " << od_d << "." << od_m << "." << od_g << " do " << do_d << "." << do_m << "." << do_g << "ispite su polozili sljedeci studenti: ";
		for (int i = 0; i < max; i++)
		{
			for (int j = 0; j < studenti[i]->brojPolozenih; j++)
			{
				if (PretvoriUDane(OD)<PretvoriUDane(studenti[i]->polozeni[j].datumPolaganja) && PretvoriUDane(DO) > PretvoriUDane(studenti[i]->polozeni[j].datumPolaganja)) {

					float prosjek = 0;
					for (int k = 0; k < studenti[i]->brojPolozenih; k++)
					{
						prosjek = prosjek + studenti[i]->polozeni[k].ocjena;
					}
					prosjek = prosjek / studenti[i]->brojPolozenih;
					cout << j + 1 << " " << studenti[i]->imePrezime << " -> " << studenti[i]->polozeni[j].nazivPredmeta << " (" << studenti[i]->polozeni[j].ocjena << ") " << "prosjek -> " << prosjek << endl;

				}
			}

		}
	}
	else {
		char trazi[100];
		cin.getline(trazi, 100);
		cout << "Trazeni sadrzaj 'nije sadrzavao' u napomeni posjeduju sljedeci studenti:" << endl;
		for (int i = 0; i < max; i++)
		{
			for (int j = 0; j < studenti[i]->brojPolozenih; j++)
			{
				if (strstr(studenti[i]->polozeni[j].napomena, trazi) != nullptr) {
					cout << studenti[i]->imePrezime << " -> predmet " << studenti[i]->polozeni[j].nazivPredmeta << endl;
				}
			}
		}
	}

}
void Dealociraj(Student *studenti[], int max) {
	for (int i = 0; i < max; i++)
	{
		delete[] studenti[i]->imePrezime; studenti[i]->imePrezime = nullptr;
		for (size_t j = 0; j < studenti[i]->brojPolozenih; j++)
		{
			delete[] studenti[i]->polozeni[j].nazivPredmeta; studenti[i]->polozeni[j].nazivPredmeta = nullptr;
			delete[] studenti[i]->polozeni[j].napomena; studenti[i]->polozeni[j].napomena = nullptr;
		}
		delete studenti[i];
		studenti[i] = nullptr;
	}
}
void main() {
	const int maxStudenata = 6;
	int trenutnoStudenata = 0, izbor = 0;
	Student * studentiIgodina[maxStudenata];
	char validPass[] = "ispitPRII2017";
	/*VRSITI VALIDACIJU KORISNIKOVIH UNOSA TE GA ONEMOGUCITI DA UNOSI VRIJEDNOSTI KOJE NISU VALIDNE*/
	do {
		/*
		za pristup aplikaciji zahtijevati logiranje,
		prilikom unosa password maskirati koristeći crticu (-)
		*/
		Login(validPass);

		izbor = PrikaziMenu();
		cin.ignore();
		if (izbor == 1) {
			DodajStudenta(studentiIgodina[trenutnoStudenata]);
			/*
			Ime i prezime -> Denis Mušić
			Koliko predmeta dodajete -> 1
			Naziv predmeta -> Uvod u informacijske tehnologije
			Datum polaganja -> 11 4 2017
			Ocjena -> 6
			Napomena -> seminarski rad nije sadrzavao sve zahtijevane funkcionalnosti
			*/
			trenutnoStudenata++;
		}
		else if (izbor == 2) {
			cout << studentiIgodina[0]->imePrezime << endl;
			DodajPolozeniPredmet(studentiIgodina, trenutnoStudenata);
			/*
			Ime i prezime -> Denis Music
			STUDENT PRONADJEN....
			Naziv predmeta -> Racunarske mreze
			Datum polaganja -> 10 4 2017
			Ocjena -> 7
			Napomena -> studentu dodijeljen seminarski rad na temu OSI modela
			*/
		}
		else if (izbor == 3) {
			Pretraga(studentiIgodina, trenutnoStudenata);
			/*
			Pratraga (1. Datumu polaganja, 2. Napomeni) -> 1
			Unesite datum OD -> 12 1 2017
			Unesite datum DO -> 14 4 2017
			U periodu od 12.1.2017 do 14.4.2017 ispite su polozili sljedeci studenti:
			1. Denis Music -> Racunarske mreze (7) Prosjek studenta: 6.5
			2. Denis Music -> Uvod u informacijske tehnologije (6) Prosjek studenta: 6.5
			3. Adel Handzic -> Baze podataka (9) Prosjek studenta: 8.7

			----------UKOLIKO SE ODABERE OPCIJA 2---------------
			Pratraga (1. Datumu polaganja, 2. Napomeni) -> 2
			Unesite tekst -> nije sadrzavao
			Trazeni sadrzaj 'nije sadrzavao' u napomeni posjeduju sljedeci studenti:
			1. Denis Music, predmet Uvod u informacijske tehnologije
			2. ....
			*/
		}

	} while (izbor != 4);


	/*oslobađa zauzetu memoriju*/
	Dealociraj(studentiIgodina, trenutnoStudenata);
}
