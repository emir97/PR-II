#include<iostream>
using namespace std;
enum Kompanija{ Pegasus, TurkishAirlines, AustrianAirlines, FlyEmirates };
int ID = 1;
struct Putnik{
int _putnikID;
char * _imePrezime;
float _brojPredjenihMilja;
void Unos(char * imePrezime){
int size = strlen(imePrezime) + 1;
_imePrezime = new char[size];
strcpy_s(_imePrezime, size, imePrezime);
_brojPredjenihMilja = 0;
_putnikID = ID++;
}
void Info(){ cout << "[" << _putnikID<<"] "<< _imePrezime <<
" (" << _brojPredjenihMilja << " milja)" << endl; }
void Dealociraj(){ delete[] _imePrezime; _imePrezime = nullptr; }
void DodajPredjeneMilje(int brojMilja){ _brojPredjenihMilja += brojMilja; }
};
struct Rezervacija{
Putnik * _putnik;
int _oznakaSjedista;
float _cijena;
void Unos(Putnik putnik, int oznaka, float cijena){
_putnik.Unos(putnik._imePrezime);
_oznakaSjedista = oznaka;
_cijena = cijena;
}
void Info(){
_putnik.Info();
cout << "Sjediste: " << _oznakaSjedista << " Cijena: "
<< _cijena << endl;
}
void Dealociraj(){ _putnik.Dealociraj(); }
};
struct Let{
Kompanija * _kompanija;
char * _pocetak; //pocetna lokacija
char * _destinacija;
Rezervacija * _rezervacije;
int _brojMjesta; //maksimalan broj mjesta na letu
float _brojMilja; //odnosi se na duzinu leta – broj predjenih milja
float _cijenaKarte;
void Unos(Kompanija kompanija, char * pocetak, char * destinacija,
int brojMjesta, float brojMilja, float cijena){
_kompanija = kompanija;
int size = strlen(pocetak) + 1;
_pocetak = new char[size];
strcpy_s(_pocetak, size, pocetak);
size = strlen(destinacija) + 1;
_destinacija = new char[size];
strcpy_s(_destinacija, size, destinacija);
_brojMjesta = brojMjesta;
_rezervacije = new Rezervacija[_brojMjesta];
Putnik * temp;
temp.Unos("<SLOBODNO MJESTO>");
for (int i = 0; i < _brojMjesta; i++)
_rezervacije[i].Unos(temp, i, 0);
_brojMilja = brojMilja;
_cijenaKarte = cijena;
}
void Dealociraj(){
/*definisati funkciju vodeci racuna o oslobadjanju svih resursa koji su alocirani
za potrebe objekta tipa Let*/ }
bool AddRezervaciju(Putnik * p){
/*na samom pocetku, funkcija treba ispisati listu svih SLOBODNIH sjedista na letu, te omoguciti odabir nekog do njih. prilikom formiranja cijene karte voditi racuna o broju predjenih milja; ako je putnik presao od 10000 do 50000 milja onda ostvaruje bool RemoveRezervacija(int oznakaSjedista){
/*funkcija uklanja rezervaciju na sjedistu cija je oznaka proslijedjena kao parametar funkcije, te vraca vrijednost. U slucaju da oznaceno mjesto nije rezervisano ili ne postoji na tom letu, funkcija vraca vrijednost false */}
};
float GetMiljeByKompanija(Let * letovi, int maxLetova, Kompanija k, Putnik p){
/*funkcija vraca broj milja koje je putnik p ostvario putujuci sa kompanijom k*/
}
void main(){
/*provjeriti validnost izvrsenja svih dostupnih funkcionalnosti. vrijednosti koje koristite prilikom testiranja mogu biti predefinisane tj. ne morate omogucavati korisniku da ih inicijalizuje.*/
}