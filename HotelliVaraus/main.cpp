
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

    setlocale(LC_ALL, "fi-FI");
    int input;
    vector<HotelRoom>rooms;
    
    while (true)
    {
        // P‰‰valikko
        // K‰ytt‰j‰lt‰ kysyt‰‰n mit‰ h‰n haluaa tehd‰.
        system("cls"); // Ilmeisesti system():n k‰yttˆ‰ pit‰isi v‰ltt‰‰
        cout << "Tervetuloa! Mit‰ haluaisit tehd‰?\n";
        cout << "1: Varaa huone" << '\n' << "2: Hae varauksia\n" << "3: Generoi uudet huoneet\n" << "0: Sulje ohjelma";
        cout << "Huoneiden lkm: " << rooms.size() << '\n';
        cin >> input; 
        while (cin.fail() || input >3 || input < 0)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Syˆtteesi oli virheellinen. Syˆt‰ valintasi uudelleen: ";
            cin >> input;
        }
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

    


