//Saara Pisil�
//8.12.2024

//C++ ohjelmoinnin perusteet, harjoitusty�
//4-5p

//hotellihuoneen varausj�rjestelm�

#include <iostream> //sy�te ja tulostus, cin ja cout
#include <vector> //vektori tietorakenteena varausten tallennukseen
#include <cstdlib> //satunnaislukujen generointi, rand
#include <ctime> //aikaleima satunnaistuksen alustamiseen, srand
#include <iomanip> //desimaalit, setprecision
#include <string> //merkkijonot

using namespace std; //standardi nimiavaruus k�ytt��n

//vakiot huoneiden hinnoille ja alennuksiin
const int YHDEN_HENGEN_HUONEEN_HINTA = 100; //1hh / 1 y�
const int KAHDEN_HENGEN_HUONEEN_HINTA = 150; //2hh / 1 y�
const double ALENNUS_KERROIN[] = { 1.0, 0.9, 0.8 }; // 0%, 10%, ja 20% alennus

//tallentaa yhden varauksen tiedot
struct varaus {
    int varausnumero;
    string nimi;
    int huoneenNumero;
    string huonetyyppi; //1hh tai 2hh
    int y�t;
    double hinta; //hinta alennuksen j�lkeen
};

//funktio satunnaisen numeron generointiin
int generoiVapaaNumero(vector<int>& vapaatNumerot, int alku, int loppu) { //satunnainen luku v�lilt� alku-loppu, joka ei ole jo k�yt�ss�
    int numero;
    bool onVapaa = false;

    //toistetaan kunnes vapaa nro on l�ytynyt
    while (!onVapaa) {
        numero = alku + rand() % (loppu - alku + 1); //voidaan sijoittaa annettu v�li, jolta numero arvotaan, alku-loppu
        onVapaa = true; //oletus: nro on vapaa

        //tarkistus
        for (int val : vapaatNumerot) {
            if (val == numero) { //nro l�ytyy listasta
                onVapaa = false; //nro ei ple vapaa
                break; //poistutaan loopista
            }
        }
    }
    //l�s�t��n nro k�ytettyjen listaan (vektori)
    vapaatNumerot.push_back(numero);
    return numero;
}

//funktio hinnan laskemiseen alennuksella
double laskeHinta(int perushinta, int y�t) {
    int alennus = rand() % 3; //arpoo alennusprosentin 0%, 10%, tai 20%
    double hintaYhteens� = perushinta * y�t * ALENNUS_KERROIN[alennus];
    cout << "Alennus: " << (1 - ALENNUS_KERROIN[alennus]) * 100 << "%" << endl; //kerrotaan k�ytt�j�lle paljonko alea
    return hintaYhteens�;
}

//funktio huoneen varaamiseen
void varaaHuone(vector<varaus>& varaukset, vector<bool>& huoneet, vector<int>& varausnumerot, string huonetyyppi, int y�t) { //listat kaikista tarvittavista tiedoista
    int perushinta;
    if (huonetyyppi == "1hh") {
        perushinta = YHDEN_HENGEN_HUONEEN_HINTA;
    }
    else {
        perushinta = KAHDEN_HENGEN_HUONEEN_HINTA;
    }
    //etsit��n vapaa huone oikeasta kategoriasta
    int alku, loppu;
    if (huonetyyppi == "1hh") { //1hh eli ensimm�inen puolikas huoneista
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
            huoneet[i] = true; //merkit��n huone varatuksi
            break; //lopettaa etsinn�n
        }
    }

    //jos ei l�ydy vapaata huonetta
    if (vapaaHuone == -1) {
        cout << "Valitettavasti kaikki " << huonetyyppi << " huoneet on varattu." << endl;
        return;
    }

    //ker�t��n varaajan tiedot
    string nimi;
    cout << "Anna varaajan nimi: ";
    cin.ignore(); //tyhjent�� sy�tepuskurin getlinea varten
    getline(cin, nimi); //getline mahdollistaa v�lily�nnin, etunimi sukunimi

    //lasketaan hinta yhteens� ja tehd��n varaus
    int varausnumero = generoiVapaaNumero(varausnumerot, 10000, 99999); //arvotaan varausnumero k�ytt�m�ll� generoiVapaaNumero-aliohjelmaa
    double hintaYhteens� = laskeHinta(perushinta, y�t); //laskee hinnan
    varaukset.push_back({ varausnumero, nimi, vapaaHuone, huonetyyppi, y�t, hintaYhteens� }); //tallentaa tiedot varauslistaan

    //tulostetaan varauksen tiedot n�kyville
    cout << "Huone " << vapaaHuone << " varattu onnistuneesti!" << endl;
    cout << "Varausnumero: " << varausnumero << endl << "Hinta on yhteens�: " << fixed << setprecision(2) << hintaYhteens� << " euroa" << endl; //hinta ilmoitetaan aina 2 desimaalin tarkkuudella
}

//funktio varausten hakuvalikkoon
void haeVaraus(const vector<varaus>& varaukset) {
    cout << "Hae varauksia:" << endl << "1. Varausnumerolla" << endl << "2. Varaajan nimell�" << endl << "Valinta: ";
    int valinta;
    cin >> valinta; //valitsee haluaako etsi� varauksia numerolla vai nimell�
    cout << endl;

    if (valinta == 1) { //haku varausnumerolla
        int numero;
        cout << "Anna varausnumero: ";
        cin >> numero;
        for (const auto& varaus : varaukset) { //k�y l�pi varaukset
            if (varaus.varausnumero == numero) { //tarkistaa vastaavuuden
                cout << "Varaaja: " << varaus.nimi << ", Huonetyyppi: " << varaus.huonetyyppi
                    << ", Huonenumero: " << varaus.huoneenNumero << ", Y�t: " << varaus.y�t << ", Hinta: " << varaus.hinta << " euroa" << endl; //tulostaa varauksen tiedot n�yt�lle
                return; //jos varaus l�ytyy
            }
        }
        cout << "Varausta ei l�ytynyt." << endl;
    }
    else if (valinta == 2) { //haku varaajan nimen perusteella
        string nimi;
        cout << "Anna varaajan nimi: ";
        cin.ignore();
        getline(cin, nimi);
        for (const auto& varaus : varaukset) { //k�y l�pi varaukset
            if (varaus.nimi == nimi) { //tarkistaa vastaavuuden
                cout << "Varausnumero: " << varaus.varausnumero << ", Huonetyyppi: " << varaus.huonetyyppi
                    << ", Huonenumero: " << varaus.huoneenNumero << ", Y�t: " << varaus.y�t << ", Hinta: " << varaus.hinta << " euroa" << endl; //tulostaa varauksen tiedot n�yt�lle
                return; //jos varaus l�ytyy
            }
        }
        cout << "Varausta ei l�ytynyt." << endl;
    }
    else if (cin.fail()) { //k�ytt�j� valitsi jotain muuta kuin 1 tai 2
        cin.clear(); //nollaa virhetilan
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //tyhjent�� virheelliset tiedot puskurista
        cout << "Virheellinen valinta." << endl;
    }
}

//p��ohjelma alkaa
int main() {
    setlocale(LC_ALL, "fi_FI"); //��kk�set k�ytt��n

    srand(time(0)); //alustaa satunnaislukugeneraattorin

    //arvotaan huoneiden m��r� 
    int huoneidenMaara = (rand() % (300 - 40 + 1) + 40) / 2 * 2; //huoneita 40�300kpl, parillinen
    vector<bool> huoneet(huoneidenMaara, false); //kaikki huoneet alussa vapaita
    vector<varaus> varaukset; //lista varauksista
    vector<int> varausnumerot; //lista varausnumeroista

    cout << "Tervetuloa hotellihuoneen varauspalveluun!" << endl;
    cout << "Hotellissamme on " << huoneidenMaara / 2 << " yhden hengen huonetta ja "
        << huoneidenMaara / 2 << " kahden hengen huonetta." << endl; //tulostaa n�yt�lle huoneiden m��r�n arvonnan tuloksen

    //ensimm�inen valikko
    while (true) {
        cout << "" << endl << "Valikko:" << endl << "1. Varaa huone" << endl << "2. Hae varausta" << endl << "3. Lopeta" << endl << "Valinta: ";
        int valinta;
        cin >> valinta; //k�ytt�j� valitsee joko 1, 2 tai 3 riippuen siit� mit� haluaa tehd� ohjelmalla
        cout << endl;

        if (valinta == 1) { //varataan huone kysym�ll� tarvittavat tiedot
            cout << "Valitse huonetyyppi (1hh tai 2hh): ";
            string huonetyyppi;
            cin >> huonetyyppi;
            cout << "Anna �iden m��r�: ";
            int y�t;
            cin >> y�t;
            if (huonetyyppi != "1hh" && huonetyyppi != "2hh") { //sy�tteentarkistus
                cout << "Virheellinen huonetyyppi." << endl;
                continue; //palaa valikkoon
            }
            varaaHuone(varaukset, huoneet, varausnumerot, huonetyyppi, y�t); //kutsutaan varausfunktio
        }
        else if (valinta == 2) { // haetaan olemassa olevaa varausta
            haeVaraus(varaukset); //aliohjelma
        }
        else if (valinta == 3) { //lopetetaan ohjelma
            cout << "Ohjelma lopetettu." << endl;
            break;
        }
        else if (cin.fail()) { //k�ytt�j� valitsi jotain muuta kuin 1 tai 2 tai 3
            cin.clear(); //nollaa virhetilan
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //tyhjent�� virheelliset tiedot puskurista
            cout << "Virheellinen valinta." << endl;
        }
    }

    return 0; //ohjelma p��ttyy
}
