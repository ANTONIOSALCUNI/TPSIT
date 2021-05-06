 /**
  * @author   Antonio Salcuni
  * @file     Ricerca.cc main.cpp .h ecc ecc ecc
  * @version  1.0
  * @date     12/04/2021
*/




#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

/**
* @brief Rubrica limitata a 200 contatti
*/
#define MAX_CONTATTI 200


/**
* @brief la costante MYDB definisce il file che contiene i contatti
*/
const string MYDB= "Rubrica.dat";
const string MYDB_BCK= "Rubrica_copia.dat";

/**
* @brief la variabile globale N_MIN contiene l'ultimo contatto inserito nel file
*/
int N_MIN=0;

/**
* @brief la variabile globale N_MAX contiene l'ultimo contatto presente nella struct
*/
int N_MAX=0;


/**
* @brief contiene i dati del contatto: telefono, nome e cognome
*/
struct Rubrica
{
    /**
     * @brief identificativo del contatto gestito automaticamente con un contatore incrementale
    */
    int codiceContatto;

    int n_telefono;
    string nome;
    string cognome;

 } persona,contatto[MAX_CONTATTI];

fstream F_rubrica,F_rubrica_copia;

/**
* @fn LEGGI_RUBRICA()
* @fn SCRIVI_RUBRICA()
* @fn VISUALIZZA_RUBRICA()
* @fn CANCELLA_CONTATTO()
* @fn MODIFICA_CONTATTO()
* @fn RICERCA_CONTATTO()
* @fn n_TelefonoPresente(int n_telefono)
* @param alla funzione n_TelefonoPresente viene passato il numero di telefono per la ricerca del contatto
*/

void LEGGI_RUBRICA();
void SCRIVI_RUBRICA();
void INSERISCI_CONTATTO();
void CANCELLA_CONTATTO();
void MODIFICA_CONTATTO();
void RICERCA_CONTATTO();
void VISUALIZZA_RUBRICA();
bool n_TelefonoPresente(int n_telefono);

void LEGGI_RUBRICA()
{
    /**
     * @brief legge la rubrica dal file
    */

    F_rubrica.open(MYDB.c_str(), ios::in | ios::binary);

    if (F_rubrica.good())
    {
       while(F_rubrica.read((char*) &contatto[N_MAX],sizeof(contatto[N_MAX])))
       {
          N_MAX++;
       }
    }
    F_rubrica.close();

    N_MIN=N_MAX;
}

void SCRIVI_RUBRICA()
{
    F_rubrica.open(MYDB.c_str(),ios::app | ios::binary);

    if(!F_rubrica.good())
       cout << "Errore apertura file!" <<endl;
    else
    {
        for(int i=N_MIN; i<N_MAX; i++)
            F_rubrica.write((char*) & contatto[i],sizeof(contatto[i]));
    }

    F_rubrica.close();

    N_MIN=N_MAX;
}


void INSERISCI_CONTATTO()
{
    char Scelta;

    do
    {
       cout << endl;
       cout << "CONTATTO N." << N_MAX+1 << " : " << endl;
       persona.codiceContatto=N_MAX+1;

       cout << "Inserisci il numero di telefono: " ;
       cin >> persona.n_telefono;

       fflush(stdin);
       cout << "Inserisci il nome: " ;
       getline(cin,persona.nome);

       fflush(stdin);
       cout << "Inserisci il cognome: " ;
       getline(cin,persona.cognome);

       cout << endl;

       if(N_MAX<MAX_CONTATTI)
       {
         cout << "Vuoi inserire un altro contatto? (S/N) " ;
         cin >> Scelta;
       }

       contatto[N_MAX]=persona;

       N_MAX++;

    }
    while(Scelta=='S' || Scelta=='s');

    SCRIVI_RUBRICA();

}

void CANCELLA_CONTATTO()
{
    int n_telefono;
    bool cancellato=false;

    F_rubrica_copia.open(MYDB_BCK.c_str(), ios::out | ios::binary);
    F_rubrica.open(MYDB.c_str(), ios::in | ios::binary);

    if(!F_rubrica | !F_rubrica_copia)
        cout<<"\n\nERRORE NELL'APERTURA DEL FILE\n\n";
    else
    {
        cout<<"\n\nINSERISCI IL NUMERO DI TELEFONO DA CANCELLARE: ";
        cin>>n_telefono;

        while(F_rubrica.read((char*) &persona, sizeof(persona)))
        {
            if(persona.n_telefono!=n_telefono)
                F_rubrica_copia.write((char*) &persona, sizeof(persona));
            else
                cancellato=true;
        }

        F_rubrica_copia.close();
        F_rubrica.close();
        remove(MYDB_BCK.c_str());
    }

    if(cancellato)
    {
        F_rubrica_copia.open(MYDB_BCK.c_str(), ios::in | ios::binary);
        F_rubrica.open(MYDB.c_str(), ios::out | ios::binary);

        if(!F_rubrica | !F_rubrica_copia)
            cout<<"\n\nERRORE NELL'APERTURA DEL FILE\n\n";
        else
        {
            while(F_rubrica_copia.read((char*) &persona, sizeof(persona)))
            {
                F_rubrica.write((char*) &persona, sizeof(persona));
            }

            F_rubrica_copia.close();
            F_rubrica.close();

            remove(MYDB_BCK.c_str());
            cout<<"\n\nCANCELLAZIONE AVVENUTA!\n";
        }
    }
    else
    {
        cout<<"\n\nNUMERO DI TELEFONO NON TROVATO!\n";
    }
}

bool n_TelefonoPresente(int n_telefono)
{
    for(int i=0;i<N_MAX;i++)
        if(contatto[i].n_telefono==n_telefono)
            return true;

    return false;
}
void MODIFICA_CONTATTO()
{
    int n_telefono, campo;
    bool modificato=false;

    F_rubrica_copia.open(MYDB_BCK.c_str(), ios::out | ios::binary);
    F_rubrica.open(MYDB.c_str(), ios::in | ios::binary);

    if(!F_rubrica | !F_rubrica)
        cout<<"\n\nERRORE NELL'APERTURA DEL FILE\n\n";
    else
    {
        cout<<"\n\nINDICA IL NUMERO DI TELEFONO DEL CONTATTO DA MODIFICARE: ";
        cin>>n_telefono;

        while(F_rubrica.read((char*) &persona, sizeof(persona)))
        {
            if(persona.n_telefono==n_telefono)
            {
                cout<<"\n\n1) NUMERO DI TELEFONO\n";
                cout<<"2) NOME\n";
                cout<<"3) COGNOME\n";
                 cout<<"\n\nINDICA IL CAMPO DA MODIFICARE: ";
                do
                {
                    cin>>campo;
                    if(campo<1 | campo>3)
                        cout<<"\n\nCAMPO NON VALIDO, RIPROVA: ";
                }while(campo<1 | campo>3);

                switch(campo)
                {
                    case 1: cout<<"\n\nINSERISCI IL NUOVO NUMERO DI TELEFONO: ";
                            do
                            {
                                cin>>persona.n_telefono;
                                if((persona.n_telefono<1) || (persona.n_telefono>9999999999))
                                    cout<<"\n\NUMERO DI TELEFONO NON VALIDA, RIPROVA: ";
                                if(n_TelefonoPresente(persona.n_telefono))
                                    cout<<"\n\nNUMERO DI TELEFONO UTILIZZATO, RIPROVA: ";
                            }while(persona.n_telefono<1 || persona.n_telefono>9999999999 || n_TelefonoPresente(persona.n_telefono));
                        break;

                    case 2: cout<<"\n\nINSERISCI IL NUOVO NOME: ";
                            cin.ignore();
                            getline(cin, persona.nome);
                        break;

                    case 3: cout<<"\n\nINSERISCI IL NUOVO COGNOME: ";
                            cin.ignore();
                            getline(cin, persona.cognome);
                        break;

                }

                modificato=true;
            }

            F_rubrica_copia.write((char*) &persona, sizeof(persona));
        }

        F_rubrica_copia.close();
        F_rubrica.close();
        remove(MYDB_BCK.c_str());
    }

    if(modificato)
    {
        F_rubrica_copia.open(MYDB_BCK.c_str(), ios::in | ios::binary);
        F_rubrica.open(MYDB.c_str(), ios::out | ios::binary);

        if(!F_rubrica | !F_rubrica_copia)
            cout<<"\n\nERRORE NELL'APERTURA DEL FILE\n\n";
        else
        {
            while(F_rubrica_copia.read((char*) &persona, sizeof(persona)))
            {
                F_rubrica.write((char*) &persona, sizeof(persona));
            }

            F_rubrica_copia.close();
            F_rubrica.close();
            remove(MYDB_BCK.c_str());
            cout<<"\n\nMODIFICA AVVENUTA!\n";
        }
    }
    else
    {
        cout<<"\n\nNUMERO DI TELEFONO NON TROVATO, NESSUNA MODIFICA EFFETTUATA\n";
    }
}


void RICERCA_CONTATTO()
{
    string nome;
    bool trovato=false;

    cout << "Inserisci il nome da ricercare" << endl;
    cin >> nome;

    F_rubrica.open(MYDB.c_str(), ios::in | ios::binary);

    if (F_rubrica.good())
    {
        while(F_rubrica.read((char*)&persona,sizeof(persona)))
        {
          if (nome==persona.nome)
          {
            cout << "Il contatto cercato e':" << endl;
            cout << persona.codiceContatto << endl;
            cout << persona.n_telefono << endl;
            cout << persona.nome << endl;
            cout << persona.cognome << endl;
            trovato=true;
          }
        }
    }

    F_rubrica.close();

    if (!trovato)
       cout << "Contatto non trovato" << endl;

}

void VISUALIZZA_RUBRICA()
 {
    for (int i=0;i<N_MAX;i++)
    {
        cout << contatto[i].codiceContatto << "\t" << contatto[i].n_telefono << "\t" << contatto[i].nome << "\t" << contatto[i].cognome << endl;
    }
}


int  main()
{
    LEGGI_RUBRICA();

    int M=0;

    do
    {
       do
       {
          cout << endl << endl;
          cout << "MENU'" << endl;
          cout << "1)INSERISCI CONTATTO" << endl;
          cout << "2)CANCELLA CONTATTO" << endl;
          cout << "3)MODIFICA CONTATTO" << endl;
          cout << "4)RICERCA CONTATTO" << endl;
          cout << "5)VISUALIZZA RUBRICA" << endl;
          cout << "6)EXIT" << endl;
          cin >> M;
       }
       while(M<1 || M>6);

       switch(M)
       {
          case 1:
                  INSERISCI_CONTATTO();
                  break;

          case 2:
                  CANCELLA_CONTATTO();
                  break;

          case 3:
                  MODIFICA_CONTATTO();
                  break;

          case 4:
                  RICERCA_CONTATTO();
                  break;

          case 5:
                  VISUALIZZA_RUBRICA();
                  break;

          case 6:
                  break;
       }
    }
    while(M<6);

    return 0;
}
