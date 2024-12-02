#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <conio.h>
#include <ctype.h>
#include "hotelroom.h"
using namespace std;

vector<HotelRoom> generateRooms()
{
    random_device rd;
    default_random_engine generator(rd());
    // Huoneiden m��r� v�lilt� 40-300. (41 muuttuu numeroksi 40)
    uniform_int_distribution<int> distribution(41, 300);

    int numOfRooms = distribution(generator);
    // Tarkistetaan, ett� huoneiden m��r� on parillinen, jos ei ole parillinen poistetaan yksi huone.
    numOfRooms = (numOfRooms % 2 == 0) ? numOfRooms : numOfRooms - 1;

    distribution = uniform_int_distribution<int>(1, 10);
    vector<HotelRoom> rooms;
    for (int i = 0; i < numOfRooms; i++)
    {
        // Huoneiden tietojen t�ytt�

        HotelRoom hotelRoom;
        // Ensimm�isen puolikkaan huoneet 1 henkil�lle, toisen puolen 2 henkil�lle 
        hotelRoom.size = (i < numOfRooms / 2) ? 1 : 2;
        hotelRoom.price = (hotelRoom.size == 1) ? 100 : 150;
        hotelRoom.roomNumber = i + 1;
        int isAvailableRNG = distribution(generator);
        hotelRoom.available = (isAvailableRNG <= 4) ? false : true; //40% mahdollisuus ett� huone on jo varattu
        if (!hotelRoom.available)
        {
            string booker = "Matti Varaaja";
        }
        hotelRoom.bookingNumber = 0;
        rooms.emplace_back(hotelRoom);
    }
    return rooms;
}

void bookRoom(vector <HotelRoom>& rooms)
{
    int input;
    char input2;
    cout << "Haluatko varata huoneen yhdelle (1) vai kahdelle (2) henkil�lle?";
    cin >> input; //Lis�� sy�tteen tarkistus
    cout << "Haluatko valita huoneen numeron? (K/E)";
    cin >> input2; //Lis�� sy�tteen tarkistus
    HotelRoom selectedRoom;

    if (input2 == 'K' || input2 == 'k')
    {
        cout << "Saatavilla olevat huoneet: \n";
        for (HotelRoom& room : rooms)
        {
            if (room.available && input == 1 && room.size == 1)
            {
                cout << room.roomNumber << '\n';
            }
            else if (room.available && input == 2 && room.size == 2)
            {
                cout << room.roomNumber << '\n';
            }
        }
        
        cout << "Valitse saatavilla oleva huone: ";
        cin >> input;
        selectedRoom = rooms[input - 1]; 
    }
    else
    {
        vector<HotelRoom> availableRooms;
        for (HotelRoom& room : rooms)
        {
            if (room.available && input == 1 && room.size == 1)
            {
                availableRooms.emplace_back(room);
            }
            else if (room.available && input == 2 && room.size == 2)
            {
                availableRooms.emplace_back(room);
            }
        }

        random_device rd;
        default_random_engine generator(rd());
        uniform_int_distribution<int> distribution(1, availableRooms.size());
        selectedRoom = availableRooms[distribution(generator)];   
    }

    string inputName;
    cout << "Anna nimesi: \n";
    cin.ignore();
    getline(cin, inputName);

    // Tilap�inen ratkaisu
    HotelRoom& SRoom = rooms[selectedRoom.roomNumber-1];

    SRoom.available = false;
    SRoom.booker = inputName;

    random_device rd;
    default_random_engine generator(rd());
    uniform_int_distribution<int> distribution(10000, 99999);
    SRoom.bookingNumber = distribution(generator);

    cout << "Varasit huoneen " << SRoom.roomNumber << ".\n";
    cout << "Varausnumerosi on " << SRoom.bookingNumber << ".\n";
    cout << "Paina mit� tahansa n�pp�int� jatkaaksesi. ";
    _getch();
}
void findRoom(std::vector<HotelRoom>& rooms)
{
    string input;
    cout << "Sy�t� varausnumero tai varaajan nimi: ";
    cin.ignore();
    getline(cin, input);
    vector<HotelRoom> foundRooms;
    if (any_of(input.begin(), input.end(), isdigit))
    {
        int inputBookingNumber = stoi(input);
        for (HotelRoom room : rooms)
        {
            if (room.bookingNumber == inputBookingNumber)
            {
                foundRooms.emplace_back(room);
                cout << "Olet varannut huoneen " << room.roomNumber <<"\n";
                cout << "Paina mit� tahansa n�pp�int� jatkaaksesi.";
                _getch();
            }
        }
    }
    else 
    {
        for (HotelRoom room : rooms)
        {
            if (room.booker == input)
            {
                foundRooms.emplace_back(room);
                cout << "Olet varannut huoneen " << room.roomNumber << "\n";
            }           
        }
    }

    if (foundRooms.empty())
    {
        cout << "Varattuja huoneita ei l�ytynyt. Varmista, ett� kirjoitit varausnumerosi tai nimesi oikein.\n�";
    }
    else
    {
        for (HotelRoom room : foundRooms)
        {
            cout << "Olet varannut huoneen " << room.roomNumber << "\n";
        }
    }
    cout << "Paina mit� tahansa n�pp�int� jatkaaksesi.";
    _getch();
}
