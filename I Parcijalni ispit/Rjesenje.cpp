#include<iostream>
using namespace std;
char * crt = "\n---------------------------\n";
struct Odgovor {
	bool tacan;
	char * tekstOdgovora;
};
struct Pitanje {
	char * tekstPitanja;
	Odgovor odgovori[5];
	int brojOdgovora;
	int brojBodova;
};
int PrikaziMenu() {
	int izbor;
	cout << crt << "\t::MENU::" << crt;
	cout << "1. Dodaj pitanje" << endl;
	cout << "2. Promijeni odgovor na pitanje" << endl;
	cout << "3. Pokreni kviz" << endl;
	cout << "4. Prikazi rezultat" << crt;
	cout << "5. Kraj" << crt;
	do {
		cout << "Vas izbor -> ";
		cin >> izbor;
	} while (izbor < 1 || izbor>5);
	system("cls");
	return izbor;
}
void DodajPitanje(Pitanje *&p){
	
	char temp[1000];
	int temp_tacan;
	p = new Pitanje;
	
	cout << "Unesite tekst pitanja -> ";
	cin.getline(temp, 1000);
	int size = strlen(temp) + 1;
	p->tekstPitanja = new char[size];
	strcpy_s(p->tekstPitanja, size, temp);

	cout << "Unesite broj odgovora -> ";
	cin >> p->brojOdgovora;
	cin.ignore();

	for (int i = 0; i < p->brojOdgovora; i++)
	{
		cout << "Odgovor " << i + 1 << " -> ";
		cin.getline(temp, 1000);
		size = strlen(temp) + 1;
		p->odgovori[i].tekstOdgovora = new char[size];
		strcpy_s(p->odgovori[i].tekstOdgovora, size, temp);

		p->odgovori[i].tacan = false;
	}
	cout << "Koji od unesenih odgovora je tacan -> ";
	do{
		cin >> temp_tacan;
		if (temp_tacan<1 || temp_tacan>p->brojOdgovora)
			cout << "Pogresan unos pokusajte ponovo ";
	} while (temp_tacan<1 || temp_tacan>p->brojOdgovora);
	
	p->odgovori[--temp_tacan].tacan = true;

	cout << "Koliko bodova nosi ovo pitanje -> ";
	cin >> p->brojBodova;
	cin.ignore();
}
void PromijeniOdgovor(Pitanje *pitanje[], int brojPitanja){
	char temp[100];
	char unos;
	cout << "Unesite odgovor ili dio odgovora za pretragu -> ";
	cin.getline(temp, 100);
	for (int i = 0; i < brojPitanja; i++)
	{
		for (int j = 0; j < pitanje[i]->brojOdgovora; j++)
		{
			char *nadji = strstr(pitanje[i]->odgovori[j].tekstOdgovora, temp);
			if (nadji != nullptr){

				cout << "Odgovor " << pitanje[i]->odgovori[j].tekstOdgovora << " je pronadjen u pitanju " <<
					pitanje[i]->tekstPitanja<<endl;

				cout << "Da li je ovo odgovor koji biste zeljeli promijeniti (d/n) -> ";
				do{
					cin >> unos;
					
					if (unos != 'd' && unos != 'n')
						cout << "Pogresan unos pokusajte ponovo ";
				} while (unos != 'd' && unos != 'n');
				cin.ignore();
				if (unos == 'd'){
					char temp_unos[100];

					cout << "Unesite novi odgovor -> ";
					cin.getline(temp_unos, 100);

					int size = strlen(temp_unos) + 1;

					delete[] pitanje[i]->odgovori[j].tekstOdgovora;

					pitanje[i]->odgovori[j].tekstOdgovora = new char[size];
					strcpy_s(pitanje[i]->odgovori[j].tekstOdgovora, size, temp_unos);

					cout << "Da li zelite postaviti novi odgovor kao tacan (d/n) -> ";
					do{
						cin >> unos;

						if (unos != 'd' && unos != 'n')
							cout << "Pogresan unos pokusajte ponovo ";
					} while (unos != 'd' && unos != 'n');
					if (unos == 'd'){
						for (int k = 0; k < pitanje[i]->brojOdgovora; k++)
						{
							pitanje[i]->odgovori[k].tacan = false;
						}
						pitanje[i]->odgovori[j].tacan = true;
					}
				}
			}
		}
	}
}
int *PokreniKviz(Pitanje *pitanja[], int max){
	int *odgovori = new int[max];
	int temp_unos;
	for (int i = 0; i < max; i++)
	{
		if (pitanja[i] != nullptr){
			cout << crt<<i+1<<pitanja[i]<<crt;
			for (int j = 0; j < pitanja[i]->brojOdgovora; j++)
			{
				cout <<j+1<<" "<< pitanja[i]->odgovori[j].tekstOdgovora << endl;
			}
			cout << crt << "Vas odgovor -> ";
			
			do{
				cin >> temp_unos;
				if (temp_unos<1 || temp_unos > pitanja[i]->brojOdgovora)
					cout << "Pogresan unos ";
			} while (temp_unos<1 || temp_unos > pitanja[i]->brojOdgovora);

			odgovori[i] = --temp_unos;
		}
	}
	return odgovori;
}
void PrikaziRezultat(Pitanje *pitanja[], int *odg, int broj_pitanja){
	int bodovi = 0;
	int ukupno_bodova = 0;
	cout << crt << "REZULTATI" << crt << endl;
	for (int i = 0; i < broj_pitanja; i++)
	{
		if (pitanja[i] != nullptr){
			if (pitanja[i]->odgovori[odg[i]].tacan == true){
				cout << i + 1 << ". pitanje -> TACNO"<<endl;
				bodovi = bodovi + pitanja[i]->brojBodova;
				ukupno_bodova = ukupno_bodova + pitanja[i]->brojBodova;
			}
			else{
				cout << i + 1 << ". pitanje -> NETACNO"<<endl;
				ukupno_bodova = ukupno_bodova + pitanja[i]->brojBodova;
			}
		}
	}
	cout << "	USPJEH: " << bodovi << "/" << ukupno_bodova << "BODOVA (" << float(bodovi) / ukupno_bodova * 100 << "%)";
}
void Dealociraj(Pitanje *pitanja[], int *&odgovori, int broj_pitanja){
	if (odgovori != nullptr){	delete[] odgovori;	 odgovori = nullptr;	}
	for (int i = 0; i < broj_pitanja; i++)
	{
		if (pitanja[i] != nullptr){
			delete[] pitanja[i]->tekstPitanja; pitanja[i]->tekstPitanja = nullptr;
			for (int j = 0; j < pitanja[i]->brojOdgovora;j++){
				delete[] pitanja[i]->odgovori[j].tekstOdgovora; pitanja[i]->odgovori[j].tekstOdgovora = nullptr;
			}
			delete pitanja[i]; pitanja[i] = nullptr;
		}
	}
}
void main(){
	const int maxPitanja = 6;
	int trenutnoPitanja = 0, izbor = 0;
	Pitanje * ispitPRII[maxPitanja];
	for (int i = 0; i < maxPitanja; i++)
		ispitPRII[i] = nullptr;
	int * odgovoriNaPitanja = nullptr;
	/*VRSITI VALIDACIJU KORISNIKOVIH UNOSA TE GA ONEMOGUCITI DA UNOSI VRIJEDNOSTI KOJE NISU VALIDNE*/
	do{
		izbor = PrikaziMenu();
		cin.ignore();
		if (izbor == 1) {
			if (trenutnoPitanja<maxPitanja)
				DodajPitanje(ispitPRII[trenutnoPitanja]);
			/*
			Unesite tekst pitanja -> U kojem gradu se nalazi FIT?
			Koliko odgovora unosite -> 4
			Odgovor 1 -> Mostar
			Odgovor 2 -> Sarajevo
			Odgovor 3 -> Tuzla
			Odgovor 4 -> Bihac
			Koji od unesenih je tacan odgovor -> 1
			Koliko bodova nosi ovo pitanje -> 3
			*/
			trenutnoPitanja++;
		}
		else if (izbor == 2) {
			/*PITANJA SE MOGU MIJENJATI SAMO AKO KVIZ NIJE URADJEN!*/
			PromijeniOdgovor(ispitPRII, trenutnoPitanja);
			/*
			Unesite odgovor ili dio odgovora za pretragu -> ac
			Odgovor Bihac pronadjen u pitanju: U kojem gradu se nalazi FIT?
			Da li je ovo odgovor koji biste zeljeli promijeniti (d/n) -> d
			Unesite novi odgovor -> Zenica
			Da li zelite postaviti novi odgovor kao tacan (d/n) -> d
			*/
		}
		else if (izbor == 3) {
			/*ONEMOGUCITI VISESTRUKO POKRETANJE KVIZA!*/
			if (trenutnoPitanja > 3) /*KVIZ MORA IMAT NAJMANJE 3 PITANJA*/
				odgovoriNaPitanja = PokreniKviz(ispitPRII, maxPitanja);
			/*
			-------------------------------
			1. U kojem gradu se nalazi FIT?
			-------------------------------
			1. Mostar
			2. Sarajevo
			3. Tuzla
			4. Zenica
			-------------------------------
			Vas odgovor -> 1
			-------------------------------
			2. Naredno pitanje...
			*/
		}
		else if (izbor == 4){
			/*REZULTATI SE PRIKAZUJU SAMO AKO JE KVIZ URADJEN*/
			if (odgovoriNaPitanja != nullptr)
				PrikaziRezultat(ispitPRII, odgovoriNaPitanja, maxPitanja);
			/*
			-------------------------------
			REZULTATI
			-------------------------------
			1. Pitanje -> TACNO
			2. Pitanje -> NETACNO
			3. Pitanje -> NETACNO
			...
			-------------------------------
			USPJEH: 10/20 BODOVA (50%)
			-------------------------------
			*/
		}

	} while (izbor != 5);


	/*oslobađa zauzetu memoriju*/
	Dealociraj(ispitPRII, odgovoriNaPitanja, trenutnoPitanja);
}
