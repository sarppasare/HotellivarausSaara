//Saara Pisilä
//8.12.2024

//C++ ohjelmoinnin perusteet, harjoitustyö
//4-5p

//hotellihuoneen varausjärjestelmä

#include <iostream> //syöte ja tulostus, cin ja cout
#include <vector> //vektori tietorakenteena varausten tallennukseen
#include <cstdlib> //satunnaislukujen generointi, rand
#include <ctime> //aikaleima satunnaistuksen alustamiseen, srand
#include <iomanip> //desimaalit, setprecision
#include <string> //merkkijonot

using namespace std; //standardi nimiavaruus käyttöön

//vakiot huoneiden hinnoille ja alennuksiin
const int YHDEN_HENGEN_HUONEEN_HINTA = 100; //1hh / 1 yö
const int KAHDEN_HENGEN_HUONEEN_HINTA = 150; //2hh / 1 yö
const double ALENNUS_KERROIN[] = { 1.0, 0.9, 0.8 }; // 0%, 10%, ja 20% alennus

//tallentaa yhden varauksen tiedot
struct varaus {
    int varausnumero;
    string nimi;
    int huoneenNumero;
    string huonetyyppi; //1hh tai 2hh
    int yöt;
    double hinta; //hinta alennuksen jälkeen
};

//funktio satunnaisen numeron generointiin
int generoiVapaaNumero(vector<int>& vapaatNumerot, int alku, int loppu) { //satunnainen luku väliltä alku-loppu, joka ei ole jo käytössä
    int numero;
    bool onVapaa = false;

    //toistetaan kunnes vapaa nro on löytynyt
    while (!onVapaa) {
        numero = alku + rand() % (loppu - alku + 1); //voidaan sijoittaa annettu väli, jolta numero arvotaan, alku-loppu
        onVapaa = true; //oletus: nro on vapaa

        //tarkistus
        for (int val : vapaatNumerot) {
            if (val == numero) { //nro löytyy listasta
                onVapaa = false; //nro ei ple vapaa
                break; //poistutaan loopista
            }
        }
    }
    //lísätään nro käytettyjen listaan (vektori)
    vapaatNumerot.push_back(numero);
    return numero;
}

//funktio hinnan laskemiseen alennuksella
double laskeHinta(int perushinta, int yöt) {
    int alennus = rand() % 3; //arpoo alennusprosentin 0%, 10%, tai 20%
    double hintaYhteensä = perushinta * yöt * ALENNUS_KERROIN[alennus];
    cout << "Alennus: " << (1 - ALENNUS_KERROIN[alennus]) * 100 << "%" << endl; //kerrotaan käyttäjälle paljonko alea
    return hintaYhteensä;
}

//funktio huoneen varaamiseen
void varaaHuone(vector<varaus>& varaukset, vector<bool>& huoneet, vector<int>& varausnumerot, string huonetyyppi, int yöt) { //listat kaikista tarvittavista tiedoista
    int perushinta;
    if (huonetyyppi == "1hh") {
        perushinta = YHDEN_HENGEN_HUONEEN_HINTA;
    }
    else {
        perushinta = KAHDEN_HENGEN_HUONEEN_HINTA;
    }
    //etsitään vapaa huone oikeasta kategoriasta
    int alku, loppu;
    if (huonetyyppi == "1hh") { //1hh eli ensimmäinen puolikas huoneista
        alku = 0;
        loppu = huoneet.size() / 2;
    }
    else { //2hh eli toinen puolikas huoneista
        alku = huoneet.size() / 2;
        loppu = huoneet.size();
    }
    int vapaaHuone = -1; //alustaan
    for (int i = alku; i < loppu; ++i) {
        if (!huoneet[i]) { //jos vapaa
            vapaaHuone = i + 1; //tallenetaan huoneen numero
            huoneet[i] = true; //merkitään huone varatuksi
            break; //lopettaa etsinnän
        }
    }

    //jos ei löydy vapaata huonetta
    if (vapaaHuone == -1) {
        cout << "Valitettavasti kaikki " << huonetyyppi << " huoneet on varattu." << endl;
        return;
    }

    //kerätään varaajan tiedot
    string nimi;
    cout << "Anna varaajan nimi: ";
    cin.ignore(); //tyhjentää syötepuskurin getlinea varten
    getline(cin, nimi); //getline mahdollistaa välilyönnin, etunimi sukunimi

    //lasketaan hinta yhteensä ja tehdään varaus
    int varausnumero = generoiVapaaNumero(varausnumerot, 10000, 99999); //arvotaan varausnumero käyttämällä generoiVapaaNumero-aliohjelmaa
    double hintaYhteensä = laskeHinta(perushinta, yöt); //laskee hinnan
    varaukset.push_back({ varausnumero, nimi, vapaaHuone, huonetyyppi, yöt, hintaYhteensä }); //tallentaa tiedot varauslistaan

    //tulostetaan varauksen tiedot näkyville
    cout << "Huone " << vapaaHuone << " varattu onnistuneesti!" << endl;
    cout << "Varausnumero: " << varausnumero << endl << "Hinta on yhteensä: " << fixed << setprecision(2) << hintaYhteensä << " euroa" << endl; //hinta ilmoitetaan aina 2 desimaalin tarkkuudella
}

//funktio varausten hakuvalikkoon
void haeVaraus(const vector<varaus>& varaukset) {
    cout << "Hae varauksia:" << endl << "1. Varausnumerolla" << endl << "2. Varaajan nimellä" << endl << "Valinta: ";
    int valinta;
    cin >> valinta; //valitsee haluaako etsiä varauksia numerolla vai nimellä
    cout << endl;

    if (valinta == 1) { //haku varausnumerolla
        int numero;
        cout << "Anna varausnumero: ";
        cin >> numero;
        for (const auto& varaus : varaukset) { //käy läpi varaukset
            if (varaus.varausnumero == numero) { //tarkistaa vastaavuuden
                cout << "Varaaja: " << varaus.nimi << ", Huonetyyppi: " << varaus.huonetyyppi
                    << ", Huonenumero: " << varaus.huoneenNumero << ", Yöt: " << varaus.yöt << ", Hinta: " << varaus.hinta << " euroa" << endl; //tulostaa varauksen tiedot näytölle
                return; //jos varaus löytyy
            }
        }
        cout << "Varausta ei löytynyt." << endl;
    }
    else if (valinta == 2) { //haku varaajan nimen perusteella
        string nimi;
        cout << "Anna varaajan nimi: ";
        cin.ignore();
        getline(cin, nimi);
        for (const auto& varaus : varaukset) { //käy läpi varaukset
            if (varaus.nimi == nimi) { //tarkistaa vastaavuuden
                cout << "Varausnumero: " << varaus.varausnumero << ", Huonetyyppi: " << varaus.huonetyyppi
                    << ", Huonenumero: " << varaus.huoneenNumero << ", Yöt: " << varaus.yöt << ", Hinta: " << varaus.hinta << " euroa" << endl; //tulostaa varauksen tiedot näytölle
                return; //jos varaus löytyy
            }
        }
        cout << "Varausta ei löytynyt." << endl;
    }
    else if (cin.fail()) { //käyttäjä valitsi jotain muuta kuin 1 tai 2
        cin.clear(); //nollaa virhetilan
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //tyhjentää virheelliset tiedot puskurista
        cout << "Virheellinen valinta." << endl;
    }
}

//pääohjelma alkaa
int main() {
    setlocale(LC_ALL, "fi_FI"); //ääkköset käyttöön

    srand(time(0)); //alustaa satunnaislukugeneraattorin

    //arvotaan huoneiden määrä 
    int huoneidenMaara = (rand() % (300 - 40 + 1) + 40) / 2 * 2; //huoneita 40–300kpl, parillinen
    vector<bool> huoneet(huoneidenMaara, false); //kaikki huoneet alussa vapaita
    vector<varaus> varaukset; //lista varauksista
    vector<int> varausnumerot; //lista varausnumeroista

    cout << "Tervetuloa hotellihuoneen varauspalveluun!" << endl;
    cout << "Hotellissamme on " << huoneidenMaara / 2 << " yhden hengen huonetta ja "
        << huoneidenMaara / 2 << " kahden hengen huonetta." << endl; //tulostaa näytölle huoneiden määrän arvonnan tuloksen

    //ensimmäinen valikko
    while (true) {
        cout << "" << endl << "Valikko:" << endl << "1. Varaa huone" << endl << "2. Hae varausta" << endl << "3. Lopeta" << endl << "Valinta: ";
        int valinta;
        cin >> valinta; //käyttäjä valitsee joko 1, 2 tai 3 riippuen siitä mitä haluaa tehdä ohjelmalla
        cout << endl;

        if (valinta == 1) { //varataan huone kysymällä tarvittavat tiedot
            cout << "Valitse huonetyyppi (1hh tai 2hh): ";
            string huonetyyppi;
            cin >> huonetyyppi;
            cout << "Anna öiden määrä: ";
            int yöt;
            cin >> yöt;
            if (huonetyyppi != "1hh" && huonetyyppi != "2hh") { //syötteentarkistus
                cout << "Virheellinen huonetyyppi." << endl;
                continue; //palaa valikkoon
            }
            varaaHuone(varaukset, huoneet, varausnumerot, huonetyyppi, yöt); //kutsutaan varausfunktio
        }
        else if (valinta == 2) { // haetaan olemassa olevaa varausta
            haeVaraus(varaukset); //aliohjelma
        }
        else if (valinta == 3) { //lopetetaan ohjelma
            cout << "Ohjelma lopetettu." << endl;
            break;
        }
        else if (cin.fail()) { //käyttäjä valitsi jotain muuta kuin 1 tai 2 tai 3
            cin.clear(); //nollaa virhetilan
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //tyhjentää virheelliset tiedot puskurista
            cout << "Virheellinen valinta." << endl;
        }
    }

    return 0; //ohjelma päättyy
}
