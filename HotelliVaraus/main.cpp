
#include <iostream>
#include <random>
#include <vector>
#include "hotelroom.h"
using namespace std;

int main()
{
    std::vector<HotelRoom> generateRooms();
    void bookRoom(std::vector<HotelRoom>&rooms);

    setlocale(LC_ALL, "fi-FI");
    int input;
    vector<HotelRoom>rooms;
    
    while (true)
    {
        system("cls"); // Ilmeisesti system():n k�ytt�� pit�isi v�ltt��
        cout << "Tervetuloa! Mit� haluaisit tehd�?\n";
        cout << "1: Varaa huone" << '\n' << "2: Hae varauksia\n" << "3: Generoi uudet huoneet";
        cout << "Huoneiden lkm: " << rooms.size() << '\n';
        cin >> input; //Lis�� sy�tteen tarkistus
        
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
            
            break;
        case 3:
            rooms = generateRooms();
            break;
        }
    }
}

    


