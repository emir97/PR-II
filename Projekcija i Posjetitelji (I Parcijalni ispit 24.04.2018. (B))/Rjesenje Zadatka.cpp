#include <iostream>
#include<tuple>
#include <iomanip>
const char * crt = "\n-------------------------------------\n";
using namespace std;

const int brojRedova = 10; // broj redova je isti u svim salama

struct Posjetitelj {
	char * _imePrezime;
	//pretpostavka je da ce se uvijek unijeti validna oznaka sjedista u formatu A-XX gdje je XX dvocifreni broj, a A predstavlja veliko slovo. Jednocifrene oznake ce imati prefiks 0
	char _ozankaSjedista[5]; //npr. A-01, B-17, C-23, -> slovo predstavlja oznaku reda, a broj oznaku kolone
};

struct  Projekcija {
	char * _film;//Leo da Vinci: Mission Mona Lisa
				 //svaki red ce cuvati informacije o posjetiteljima u tom redu
	Posjetitelj * _posjetitelji[brojRedova];
	int _brojPosjetiteljaURedu[brojRedova];//cuva informaciju koliko svaki red ima dodatih posjetitelja
										   /*ako zelite, nacin cuvanja informacije o broju elemenata u pojedinom redu mozete implementirati i na neki drugi nacin npr. postavljajuci neke defaultne vrijednosti atributa objekta*/
	int _brojKolonaUSali;//broj kolona mora biti paran >=5 i <=10
};

void Dealociraj(Projekcija & projekcija) {
	delete[] projekcija._film; projekcija._film = nullptr;
	for (size_t i = 0; i < brojRedova; i++) {
		for (size_t j = 0; j < projekcija._brojPosjetiteljaURedu[i]; j++) {
			delete[] projekcija._posjetitelji[i][j]._imePrezime;
			projekcija._posjetitelji[i][j]._imePrezime = nullptr;
		}
		delete[]  projekcija._posjetitelji[i];
		projekcija._posjetitelji[i] = nullptr;
	}
}

bool Unos(Projekcija &p, const char *naziv, int broj_kolona) {
	if (broj_kolona % 2 != 0 || broj_kolona < 5 || broj_kolona > 10) return false;
	int size = strlen(naziv) + 1;
	p._film = new char[size];
	strcpy_s(p._film, size, naziv);
	p._brojKolonaUSali = broj_kolona;
	for (size_t i = 0; i < brojRedova; i++) {
		p._posjetitelji[i] = new Posjetitelj[p._brojKolonaUSali];
		p._brojPosjetiteljaURedu[i] = 0;
	}
	return true;
}

pair<int, int> GetPozicijuUReduIKoloni(const char *oznaka) {
	int red = (int)(toupper(oznaka[0]) - 65);
	int kolona = atoi(&oznaka[2]) - 1;
	return pair<int, int>(red, kolona);
}


Posjetitelj GetNovogPosjetitelja(const char *oznakaSjedista, const char *imePrezime) {
	Posjetitelj p;
	int size = strlen(imePrezime) + 1;
	p._imePrezime = new char[size];
	strcpy_s(p._imePrezime, size, imePrezime);
	strncpy_s(p._ozankaSjedista, 5, oznakaSjedista, _TRUNCATE);
	return p;
}

bool DodajPosjetitelja(Projekcija &pr, Posjetitelj ps) {
	pair<int, int> oznakaSjedista = GetPozicijuUReduIKoloni(ps._ozankaSjedista);
	// provjera da li postoji vec taj posjetitelj
	for (size_t i = 0; i < brojRedova; i++)
	{
		for (size_t j = 0; j < pr._brojPosjetiteljaURedu[i]; j++)
		{
			// provjera da li postoji vec taj posjetitelj
			if (strcmp(pr._posjetitelji[i][j]._imePrezime, ps._imePrezime) == 0) return false;
			// provjera da li je sjediste vec zauzeto
			if (strcmp(pr._posjetitelji[i][j]._ozankaSjedista, ps._ozankaSjedista) == 0) return false;
		}
	}
	// provjera da li oznaka sjedista validna
	pair<int, int> oznakaSedista = GetPozicijuUReduIKoloni(ps._ozankaSjedista);
	if (oznakaSedista.first > brojRedova || oznakaSjedista.second > pr._brojKolonaUSali) return false;
	pr._posjetitelji[oznakaSjedista.first][pr._brojPosjetiteljaURedu[oznakaSjedista.first]] = ps;
	pr._brojPosjetiteljaURedu[oznakaSjedista.first]++;
	return true;
}

void PrikaziRasporedSjedenja(Projekcija p) {
	cout << crt << "RASPORED SJEDENJA" << crt << endl;
	for (size_t i = 0; i < 10; i++)
	{
		for (size_t i = 0; i < p._brojKolonaUSali*16; i++)
			cout << "-";
		cout << endl;
		for (size_t j = 0; j < p._brojKolonaUSali; j++)
		{
			bool ispis = true;
			for (size_t k = 0; k < p._brojPosjetiteljaURedu[i]; k++)
			{
				pair<int, int> pr = GetPozicijuUReduIKoloni(p._posjetitelji[i][k]._ozankaSjedista);
				if (pr.first == j) {
					cout << "|" << setw(15) << p._posjetitelji[i][k]._imePrezime;
					ispis = false;
				}
			}
			if (ispis) {
				cout << "|" << setw(15) << " ";
			}
		}
		cout << "|" << endl;
	}
	for (size_t i = 0; i < p._brojKolonaUSali * 16; i++)
		cout << "-";
	cout << endl;
}

char *GenerisiOznakuSjedista(Projekcija p) {
	// provjera da li su sva mjesta zauzeta
	int sum = 0;
	for (size_t i = 0; i < brojRedova; i++)
		sum = sum + p._brojPosjetiteljaURedu[i];
	if (sum == brojRedova * p._brojKolonaUSali) return nullptr; // sva mjesta su zauzeta
	
	
	int pronasao = false;
	int red = 0;
	int kolona = 1;
	char *oznakaSjedista = new char[5];
	do {
		pronasao = false;
		char redChar = (char) red + 65;
		char kolonaChar[3];
		oznakaSjedista[0] = redChar;
		oznakaSjedista[1] = '-';
		oznakaSjedista[2] = '\0';
		if (kolona < 10) {
			kolonaChar[0] = '0';
			_itoa_s(kolona, &kolonaChar[1], 2, 10);
		}
		else {
			_itoa_s(kolona, kolonaChar, 2, 10);
		}
		strcat_s(oznakaSjedista, 5, kolonaChar);
		for (size_t i = 0; i < p._brojPosjetiteljaURedu[red]; i++)
		{
			if (strcmp(p._posjetitelji[red][i]._ozankaSjedista, oznakaSjedista) == 0) {
				pronasao = true;
			}
		}
		if (!pronasao) {
			return oznakaSjedista;
		}
		kolona++;
		if (kolona == p._brojKolonaUSali) {
			kolona = 1;
			red++;
		}
	} while (pronasao && red < brojRedova);
}
int main() {

	int oznakaR = 0, oznakaK = 0;
	Projekcija Leo; //Leo da Vinci: Mission Mona Lisa
					//inicijalizuje vrijednosti atributa objekta osvetnici
	Unos(Leo, "Leo da Vinci: Mission Mona Lisa", 6);//film, broj_kolona

	//GetPozicijuUReduIKoloni - na osnovu oznake sjedista vraca pair red i kolona tj. poziciju u matrici
	tie(oznakaR, oznakaK) = GetPozicijuUReduIKoloni("B-15");
	cout << crt << "GetPozicijuUReduIKoloni(B-15) ->" << oznakaR << "/" << oznakaK << endl;//ispisuje 1/14 
	tie(oznakaR, oznakaK) = GetPozicijuUReduIKoloni("C-01");
	cout << "GetPozicijuUReduIKoloni(C-01) ->" << oznakaR << "/" << oznakaK << crt;//ispisuje 2/0
	/*
	GetNovogPosjetitelja - vraca objekat (ne pokazivac) tipa Posjetitelj koji je inicijalizovan vrijednostima proslijedjenih parametara
	DodajPosjetitelja - podatke o novom posjetitelju dodaje u listu (niz) posjetitelja za odredjenu projekciju.
	Onemoguciti dodavanje posjetitelja sa istim imenom i prezimenom, nepostojecom lokacijom sjedista ili u slucaju da su sva mjesta popunjena
	*/

	if (DodajPosjetitelja(Leo, GetNovogPosjetitelja("A-02", "Denis Music")))
		cout << crt << "Posjetitelj uspjesno dodan!" << crt;
	if (DodajPosjetitelja(Leo, GetNovogPosjetitelja("C-03", "Zanin Vejzovic")))
		cout << crt << "Posjetitelj uspjesno dodan!" << crt;
	if (DodajPosjetitelja(Leo, GetNovogPosjetitelja("D-05", "Adel Handzic")))
		cout << crt << "Posjetitelj uspjesno dodan!" << crt;

	//PrikaziRasporedSjedenja - na osnovu oznake sjedista prikazuje raspored sjedenja posjetitelja za vrijeme projekcije
	PrikaziRasporedSjedenja(Leo);

	/*
	DODATNO:
	1. prilikom ispisa, sirinu kolone prilagoditi maksimalnom broju karaktera u imenu i prezimenu posjetioca
	2. automatski generisati oznaku sjedista na osnovu prvog slobodnog mjesta na projekciji. ovu funkciju nije potrebno integrisati u postojece rjesenje, vec se moze i zasebno testirati
	*/

	cout << "Generisana oznaka -> "<<GenerisiOznakuSjedista(Leo) << endl;


	Dealociraj(Leo);

	system("pause>0");
	return 0;
}
