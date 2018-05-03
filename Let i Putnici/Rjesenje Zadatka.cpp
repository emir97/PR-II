#include <iostream>
#include<tuple>
#include<iomanip>
const char * crt = "\n-------------------------------------\n";

using namespace std;

struct Putnik {
	char * _imePrezime;
	//pretpostavka je da ce se uvijek unijeti validna oznaka sjedista u formatu XXA gdje je XX dvocifreni broj, a A predstavlja veliko slovo. Jednocifrene oznake ce imati prefiks 0
	char _ozankaSjedista[4]; //npr. 02A, 12B, 14B, 28C -> broj predstavlja oznaku reda, a slovo oznaku kolone
};

struct Let {
	char * _relacija;//Mostar -> Sarajevo
	Putnik * _putnici;
	int _trenutnoPutnika;
	int _brojRedovaUAvionu;
	int _brojKolonaUAvionu;//broj kolona mora biti paran >=4 i <=10
};

void Dealociraj(Let & let) {
	delete[] let._relacija; let._relacija = nullptr;
	for (size_t i = 0; i < let._trenutnoPutnika; i++) {
		delete[] let._putnici[i]._imePrezime;
		let._putnici[i]._imePrezime = nullptr;
	}
	delete[] let._putnici;
	let._putnici = nullptr;
}

bool Unos(Let &l, const char *relacija, int brojRedova, int brojKolona) {
	if (brojKolona % 2 != 0 || brojKolona < 4 || brojKolona > 10) {
		return false;
	}
	int size = strlen(relacija) + 1;
	l._relacija = new char[size];
	strcpy_s(l._relacija, size, relacija);

	l._putnici = nullptr;
	l._trenutnoPutnika = 0;

	l._brojRedovaUAvionu = brojRedova;
	l._brojKolonaUAvionu = brojKolona;
	return true;
}

pair<int, int> GetPozicijuUReduIKoloni(const char *oznaka) {
	char red[3], kolona;
	strncpy_s(red, 3, oznaka, 2);
	kolona = oznaka[2];
	
	int oznakaReda, oznakaKolone;
	oznakaReda = atoi(red) - 1;
	oznakaKolone = (int)toupper(kolona) - 65;
	return pair<int, int>(oznakaReda, oznakaKolone);
}
Putnik GetNovogPutnika(const char *oznakaSjedista, const char *imePrezime) {
	Putnik p;
	int size = strlen(imePrezime) + 1;
	p._imePrezime = new char[size];
	strcpy_s(p._imePrezime, size, imePrezime);
	strncpy_s(p._ozankaSjedista, sizeof(p._ozankaSjedista), oznakaSjedista, _TRUNCATE);
	return p;
}
bool DodajPutnika(Let &l, Putnik p) {
	for (size_t i = 0; i < l._trenutnoPutnika; i++)
	{
		// provjera da li postoji putnik vec sa tim imenom i prezimenom
		if (strcmp(l._putnici[i]._imePrezime, p._imePrezime) == 0) return false;
		// provjera da li je oznaka sa sjedistem vec zauzeta
		if (strcmp(l._putnici[i]._ozankaSjedista, p._ozankaSjedista) == 0) return false;
	}
	// provjera da li postoji ta oznaka sjedista
	pair<int, int> pr = GetPozicijuUReduIKoloni(p._ozankaSjedista);
	if (pr.first > l._brojRedovaUAvionu || pr.second > l._brojKolonaUAvionu) return false;

	// Dodavanje putnika
	Putnik *putnici = new Putnik[l._trenutnoPutnika + 1];
	for (size_t i = 0; i < l._trenutnoPutnika; i++)
	{
		putnici[i] = l._putnici[i];
	}
	delete[] l._putnici;
	putnici[l._trenutnoPutnika] = p;
	l._putnici = putnici;
	l._trenutnoPutnika++;
	return true;
}
int rekBrojacKaraktera(Let l, int brojPutnika, int maxKaraktera) {
	if ((brojPutnika-1) < 0) {
		return maxKaraktera;
	}
	if (strlen(l._putnici[brojPutnika - 1]._imePrezime) > maxKaraktera)
		return rekBrojacKaraktera(l, brojPutnika - 1, strlen(l._putnici[brojPutnika - 1]._imePrezime));
	return rekBrojacKaraktera(l, brojPutnika - 1, maxKaraktera);
}
void PrikaziRasporedSjedenja(Let l) {
	int maxBrojKaraktera = rekBrojacKaraktera(l, l._trenutnoPutnika, 0);
	for (size_t i = 0; i < l._brojRedovaUAvionu; i++)
	{
		for (size_t j = 0; j < l._brojKolonaUAvionu*maxBrojKaraktera+8; j++)
			cout << "=";
		cout << endl;
		cout << "|";
		for (size_t j = 0; j < l._brojKolonaUAvionu; j++)
		{
			bool ispis = true;
			pair<int, int> p;
			for (size_t k = 0; k < l._trenutnoPutnika; k++)
			{
				p = GetPozicijuUReduIKoloni(l._putnici[k]._ozankaSjedista);
				if (p.first == i && p.second == j) {
					cout << setw(maxBrojKaraktera)<<l._putnici[k]._imePrezime << "|" << " ";
					ispis = false;
				}
			}
			if(ispis)
				cout<<setw(maxBrojKaraktera)<<" "<< "|"<< " ";
		}
		cout << endl;
	}
	for (size_t j = 0; j < l._brojKolonaUAvionu*maxBrojKaraktera+8; j++)
		cout << "=";
	cout << endl;
}

char *GetOznakaSjedista(Let l) {
	int red = 0;
	int kolona = 0;
	bool flag = true;
	pair<int, int> p;

	// provjera da li postoij slobodno mjesto
	if (l._trenutnoPutnika == l._brojRedovaUAvionu * l._brojKolonaUAvionu) {
		return nullptr; // nema slobodnog mjesta
	}

	do
	{
		flag = true;
		for (size_t i = 0; i < l._trenutnoPutnika; i++)
		{
			p = GetPozicijuUReduIKoloni(l._putnici[i]._ozankaSjedista);
			if (p.first == red && p.second == kolona) {
				flag = false;
			}
		}
		// mjesto nije zauzeto te kreiramo oznako i vracamo je iz funkcije
		if (flag) {
			char redChar[3];
			char kolonaChar = (char)(kolona + 65); // dodamo 65 da bi pretvorili ovaj broj u veliko slovo
			_itoa_s((red+1), redChar, 3, 10);
			if ((red + 1) < 10) { // u slucaju da je jednocifren moramo dodati 0 na pocetak
				redChar[1] = redChar[0];
				redChar[0] = '0';
				redChar[2] = '\0'; // kako smo poremetili null-terminirajuci karakter moramo ga staviti na kraj
			}
			
			char *oznakaSjedista = new char[4];
			strcpy_s(oznakaSjedista, 4, redChar);
			oznakaSjedista[2] = kolonaChar;
			oznakaSjedista[3] = '\0';
			return oznakaSjedista;
			
		}
		// znaci mjesto je zauzeto  i trazimo iduce slobodno
		kolona++;
		if (kolona == l._brojKolonaUAvionu) {
			kolona = 0;
			red++;
		}
	} while (!flag);

	return nullptr;
}
int main() {
	int oznakaR = 0, oznakaK = 0;
	Let mostar_sarajevo;
	//inicijalizuje vrijednosti atributa objekta mostar_sarajevo
	Unos(mostar_sarajevo, "Mostar -> Sarajevo", 10, 4);//relacija, broj_redova, broj_kolona
	
													   //GetPozicijuUReduIKoloni - na osnovu oznake sjedista vraca pair red i kolona tj. poziciju u matrici
	tie(oznakaR, oznakaK) = GetPozicijuUReduIKoloni("15B");
	cout << crt << "GetPozicijuUReduIKoloni(15B) ->" << oznakaR << "/" << oznakaK << endl;//ispisuje 14/1 
	tie(oznakaR, oznakaK) = GetPozicijuUReduIKoloni("01B");
	cout << "GetPozicijuUReduIKoloni(01B) ->" << oznakaR << "/" << oznakaK << crt;//ispisuje 0/1
																				  /*
																				  GetNovogPutnika - vraca objekat (ne pokazivac) tipa Putnik koji je inicijalizovan vrijednsotima proslijedjenih parametara
																				  DodajPutnika - podatke o novom putniku dodaje u listu (niz) putnika na proslijedjenom letu. Onemoguciti dodavanje putnika sa istim imenom i prezimenom,
																				  nepostojecom lokacijom sjedista ili u slucaju da su sva mjesta popunjena
																				  */
	if (DodajPutnika(mostar_sarajevo, GetNovogPutnika("01A", "Denis Music")))
		cout << crt << "Putnik uspjesno dodan!" << crt;
	if (DodajPutnika(mostar_sarajevo, GetNovogPutnika("07C", "Zanin Vejzovic")))
		cout << crt << "Putnik uspjesno dodan!" << crt;
	if (DodajPutnika(mostar_sarajevo, GetNovogPutnika("10D", "Adel Handzic")))
		cout << crt << "Putnik uspjesno dodan!" << crt;
	//PrikaziRasporedSjedenja - na osnovu oznake sjedista prikazuje raspored sjedenja u avionu za let koji je proslijedjen kao parametar
	PrikaziRasporedSjedenja(mostar_sarajevo);

	//rekBrojacKaraktera - rekurzivna funkcija koja vraca maksimalan broj karaktera u imenu i prezimenu putnika na odredjenom letu
	cout << crt << "Maksimalna broj karaktera u imenu i prezimenu putnika je -> " << rekBrojacKaraktera(mostar_sarajevo,
		mostar_sarajevo._trenutnoPutnika, 0) << crt; //broj 0 je pocetno stanje prilikom brojanja max karaktera

													 /*
													 DODATNO:
													 1. prilikom ispisa, sirinu kolone prilagoditi maksimalnom broju karaktera u imenu i prezimenu
													 2. automatski generisati oznaku sjedista na osnovu narednog slobodnog mjesta na letu (povratnom vrijednoscu sugerisati na slucaj u kome su sva mjesta na letu vec zauzeta). ovu funkciju nije potrebno integrisati u postojece rjesenje, vec se moze i zasebno testirati
													 */


	cout << "Generisano slobodno mjesto -> " << GetOznakaSjedista(mostar_sarajevo) << endl;
	
	Dealociraj(mostar_sarajevo);
	system("pause>0");
	return 0;
}
