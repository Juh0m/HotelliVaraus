
#include <iostream>
#include <random>
#include <vector>
#include "hotelroom.h"
using namespace std;
int main()
{
    std::vector<HotelRoom> generateRooms();
    void bookRoom(std::vector<HotelRoom>&rooms);
    void findRoom(std::vector<HotelRoom>&rooms);
    vector<HotelRoom> readRooms();
    vector<HotelRoom>rooms;
    setlocale(LC_ALL, "fi-FI");
    int input;
    // Huoneiden tietojen luku tiedostosta
    // Jos tiedosto on tyhj� tai virheellinen, varmistetaan ettei ohjelma kaadu
    try
    {
        rooms = readRooms();
    }
    catch(exception e)
    {
        
    }
    
    while (true)
    {
        // P��valikko
        // K�ytt�j�lt� kysyt��n mit� h�n haluaa tehd�.
        system("cls");
        cout << "Tervetuloa! Mit� haluaisit tehd�?\n";
        cout << "1: Varaa huone" << '\n' << "2: Hae varauksia\n" << "3: Generoi uudet huoneet\n" << "0: Sulje ohjelma";
        cout << "\nHuoneiden m��r�: " << rooms.size() << '\n';
        
        cin >> input; 
        while (cin.fail() || input >3 || input < 0)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Sy�tteesi oli virheellinen. Sy�t� valintasi uudelleen: ";
            cin >> input;
        }
        // Suljetaan ohjelma l�htem�ll� while- loopista.
        if (input == 0)
        {
            break;
        }
        switch (input)
        {
        case 1: 
            bookRoom(rooms);
            break;
        case 2:
            findRoom(rooms);
            break;
        case 3:
            rooms = generateRooms();
            break;
        }
    }
}

    


