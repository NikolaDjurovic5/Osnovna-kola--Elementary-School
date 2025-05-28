
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime> //moram da koristim zbog rand broja
#include <string>

using namespace std;
		/*			obavezno ispitivanje gresaka citanja ili upisa		*/
					enum Greska{Greska_citanja, greska_upisa};
					char *poruke_o_greskama[] = {
					"\nGreske pri otvaranju datoteke za citanje\n",
					"\nGreske pri otvaranju datoteke za upis\n"
						};
					void greska(Greska status);
					void greska(Greska status){
					cerr<<poruke_o_greskama[status];
					exit(1);
						}


// Struktura za cuvanje reci
struct Rec {
    char rec[13];
};

// Struktura za cuvanje informacija o igracu
struct Igrac {
    char ime[50];
    char prezime[50];
};

// Struktura za cuvanje pitanja
struct Pitanje {
    char pitanje[100];
    char odgovori[2][50];
    int tacanOdgovor;
};

/*		Funkcija za otvaranje datoteke, deklarisem objekat datoteka, 
		otvaram datoteku reci.dat, ako se datoteka ne otvori ispisuje gresku.*/

fstream otvoriDatoteku(const char* imeDatoteke) {
    fstream datoteka;
    const char* putanja = "C:\\Users\\Djura\\Desktop\\FAKULTET\\DRUGA GODINA\\TRECI SEMESTAR\\Programski jezici\\PJ\\seminarski rad\\seminarski\\Debug\\reci.dat";
    datoteka.open(putanja, ios::in | ios::out | ios::binary);
    if (!datoteka) {
       greska(Greska_citanja);
    }
    return datoteka;
}

/*		Funkcija za inicijalizaciju strukture Rec, koja prima referencu na objekat tipa Rec.
		Imam niz od 4 reci. Generisem random od 0  do maks elemenata - 1.
		Kopiram zatim u Rec.rec strukture Rec, i na kraju recu stavljam kraj \0				 */

void inicijalizujRec(Rec& rec) {
    const char* moguceReci[] = {
        "laboratorija",
        "kolektivnost",
        "aerodinamika",
        "antievropski"
    };

    // Generisanje nasumicnog indeksa kako bismo izabrali rec iz niza mogucih reci
    int indeksNasumicneReci = rand() % (sizeof(moguceReci) / sizeof(moguceReci[0]));

    // Kopiranje izabrane reci  u polje rec u strukturi Rec
    strncpy_s(rec.rec, moguceReci[indeksNasumicneReci], sizeof(rec.rec) - 1);

    // Postavljanje  znaka na kraju kopirane reci 
    rec.rec[sizeof(rec.rec) - 1] = '\0';
}

/*		Funkcija za prikaz trenute reci sa pogodjenim slovima,
		provera se da li je dat tacan odg, ako jeste prikazuje slovo,
		ako nije stavlja donju crtu.													*/
void prikaziRec(const char* rec, const char* pogodjenaSlova) {
    cout << "Rec: "; 
    for (size_t i = 0; i < strlen(rec); ++i) {
        if (pogodjenaSlova[i] != 0) {
            cout << rec[i] << " ";
        } else {
            cout << "_ ";
        }
    }
    cout << endl;
}

/* Funkcija za postavljanje pitanja i proveru odgovora, skupljamo sve u odgvor*/
char pitajIgraca() {
    char odgovor;
    cout << "Unesite tacan odgovor (1/2): ";
    cin >> odgovor;
    return odgovor;
}

/*		Deklarisem funkciju igraj igru, koja prima niz pitanja i ukupan broj pitanja,
		Deklarisem objekat tipa Rec navan rec, zatim inicijalizujem nasuimicno jednu rec.
		Zatim pratimo u niz pogodjena slova, gledam pomocu memset, sve elemente postavljamo na 0.
		Pozivamo opet prikazi rec, da bi videlei trenutno stanje u reci.
		Prikazuje se pitanje i dva  ponudjenja odgovora, i pozivamo funkciju pitajIgraca.
		Posle devet pitanja , igracu se nudi da pokusa da pogodi koja je rec u pitanju,
		ako odgovori tacno igra je zavrsena, ako ne ispisace mu koja je rec u pitanju i zavrsice igricu.			*/

void igrajIgru(Pitanje* pitanja, size_t brojPitanja) {
    Rec rec;
    inicijalizujRec(rec);

    char pogodjenaSlova[13];

    // Postavljanje svih elemenata niza pogodjenaSlova na vrednost 0
    memset(pogodjenaSlova, 0, sizeof(pogodjenaSlova));

    cout << "=================================================================" << endl;
    cout << "                   Rec sadrzi odredjen broj slova:               " << endl;

    prikaziRec(rec.rec, pogodjenaSlova);
    cout << "=================================================================" << endl;
    for (size_t i = 0; i < brojPitanja; ++i) {
        const Pitanje& pitanje = pitanja[i];

        cout << "Pitanje: " << pitanje.pitanje << endl;
        cout << "1." << pitanje.odgovori[0] << endl;
        cout << "2." << pitanje.odgovori[1] << endl;

        char odgovor = pitajIgraca();

        if ((odgovor == '1' && pitanje.tacanOdgovor == 1) || (odgovor == '2' && pitanje.tacanOdgovor == 2)) {
            pogodjenaSlova[i] = rec.rec[i];
        }

        cout << "Rec: ";
        for (size_t j = 0; j < strlen(rec.rec); ++j) {
            if (pogodjenaSlova[j] != 0) {
                cout << rec.rec[j] << " ";
            } else {
                cout << "_ ";
            }
        }
        cout << endl;
        cout << "==============================================================" << endl;
        cout << endl;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	//cistimo ulazni bafer, da bi opet pripremili za odgovor

    cout << "Unesite konacnu rec: ";
    char unetaRec[13];
    cin.getline(unetaRec, sizeof(unetaRec));

    if (strcmp(rec.rec, unetaRec) == 0) {
        cout << "==============================================================" << endl;
        cout << "                     Bravo, tacno je!               " << endl;
    } else {
        cout << "==============================================================" << endl;
        cout << "     Netacno. Ispravna rec je:    " << rec.rec << endl;
    }
		cout << "==============================================================" << endl;
		cout << "                     Igra zavrsena!                " << endl;
		cout << "==============================================================" << endl;
}

/*		Funkcija za mesanje pitanja, kupiamo radnom pitanja,
		swap koristimo da zamenili pozicije na pitanjima i i j.		*/

void promesajPitanja(Pitanje* pitanja, int brojPitanja) {
    for (int i = brojPitanja - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        swap(pitanja[i], pitanja[j]);
    }
}

/* Glavna funkcija programa, ipsisujem poruke, kreiram objekat igrac, 
unos imena i prezimena, zatim dobro dosilica sa imenom i prezimenom igraca,
postavljanje seed za pseudoslucajne brojeve, zatim otvaranje datoteke,
ucitavanje 9 pitanja, mesanje pitanja pre pocetka igre,
pozivam funkciju igraj igru, i zatvaranje datoteke.*/
int main() {
    cout << "=================================================================" << endl;
    cout << "                Dobro dosli u igricu leteca zagonetka         " << endl;
    cout << "=================================================================" << endl;
    cout << "          Krecemo sa devet pitanja iz opste informisanosti    " << endl;
    cout << "=================================================================" << endl;
    cout << "            Ali mi morate prvo odgovoriti na dva pitanja      " << endl;
    cout << "=================================================================" << endl;

	srand(time(0));

    // Kreiranje objekta tipa Igrac
    Igrac* igrac = new Igrac;
    if(igrac == NULL) {
        exit(1);
    }

    cout << "\t\t\tUnesite ime" << endl << "\t\t\t";
    cin >> igrac->ime;

    cout << "\t\t\tUnesite prezime" << endl << "\t\t\t";
    cin >> igrac->prezime;
	
    cout << "=================================================================" << endl;
    cout << " \t                Dobro dosli          " << endl;
    cout << "\t\t\t" << igrac->ime << " " << igrac->prezime << endl;
    cout << "=================================================================" << endl;
	
    srand(time(0)); // Postavljanje seed-a za generisanje pseudoslucajnih brojeva
    fstream datoteka = otvoriDatoteku("reci.dat");
	
    // Inicijalizacija niza struktura Pitanje
    Pitanje svaPitanja[50];
    for (size_t i = 0; i < 50; ++i) {
        // Ucitavanje pitanja iz datoteke
        datoteka.getline(svaPitanja[i].pitanje, sizeof(svaPitanja[i].pitanje), ';');
        datoteka.getline(svaPitanja[i].odgovori[0], sizeof(svaPitanja[i].odgovori[0]), ';');
        datoteka.getline(svaPitanja[i].odgovori[1], sizeof(svaPitanja[i].odgovori[1]), ';');

        char tacanOdgovorStr[2];
        datoteka.getline(tacanOdgovorStr, sizeof(tacanOdgovorStr), ';');
        svaPitanja[i].tacanOdgovor = atoi(tacanOdgovorStr);

        datoteka.ignore();
		
    }
	
    // Mesanje pitanja pre igranja igre
    promesajPitanja(svaPitanja, 50);

    // Poziv funkcije za igranje igre
    igrajIgru(svaPitanja, 9);
	
    datoteka.close();
   
    return 0;
}
