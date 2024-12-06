#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <conio.h>
#include <ctype.h>
#include <fstream>
#include "hotelroom.h"

using namespace std;
using json = nlohmann::json;
void writeRooms(std::vector<HotelRoom>& rooms);
void from_json(const json& j, HotelRoom& room);
void to_json(json& j, const HotelRoom& room);
vector<HotelRoom> readRooms();

vector<HotelRoom> generateRooms()
{
	random_device rd;
	default_random_engine generator(rd());
	// Huoneiden määrä väliltä 40-300. (41 muuttuu numeroksi 40)
	uniform_int_distribution<int> distribution(41, 300);

	int numOfRooms = distribution(generator);
	// Tarkistetaan, että huoneiden määrä on parillinen, jos ei ole parillinen poistetaan yksi huone.
	numOfRooms = (numOfRooms % 2 == 0) ? numOfRooms : numOfRooms - 1;

	distribution = uniform_int_distribution<int>(1, 10);
	vector<HotelRoom> rooms;
	for (int i = 0; i < numOfRooms; i++)
	{
		// Huoneiden tietojen täyttö

		HotelRoom hotelRoom;
		// Ensimmäisen puolikkaan huoneet 1 henkilölle, toisen puolen 2 henkilölle 
		hotelRoom.size = (i < numOfRooms / 2) ? 1 : 2;
		hotelRoom.price = (hotelRoom.size == 1) ? 100 : 150;
		hotelRoom.roomNumber = i + 1;
		int isAvailableRNG = distribution(generator);
		hotelRoom.available = (isAvailableRNG <= 4) ? false : true; //40% mahdollisuus että huone on jo varattu
		if (!hotelRoom.available)
		{
			string booker = "Matti Varaaja";
		}
		hotelRoom.bookingNumber = 0;
		rooms.emplace_back(hotelRoom);
	}
	writeRooms(rooms);
	return rooms;
}

void bookRoom(vector <HotelRoom>& rooms)
{
	int input;
	char input2;
	cout << "Haluatko varata huoneen yhdelle (1) vai kahdelle (2) henkilölle?";
	cin >> input; //Lisää syötteen tarkistus
	cout << "Haluatko valita huoneen numeron? (K/E)";
	cin >> input2; //Lisää syötteen tarkistus
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

	HotelRoom& SRoom = rooms[selectedRoom.roomNumber - 1];

	cout << "Huoneen hinta on " << SRoom.price << "\n";
	cout << "Haluatko varmasti varata huoneen " << SRoom.roomNumber << "? (K/E)\n";
	
	

	string selectionInput;
	cin >> selectionInput;
	if (selectionInput != "K" && selectionInput != "k")
	{
		return;
	}

	SRoom.available = false;
	SRoom.booker = inputName;

	random_device rd;
	default_random_engine generator(rd());
	uniform_int_distribution<int> distribution(10000, 99999);
	SRoom.bookingNumber = distribution(generator);

	cout << "Varasit huoneen " << SRoom.roomNumber << ".\n";
	cout << "Varausnumerosi on " << SRoom.bookingNumber << ".\n";
	writeRooms(rooms);
	cout << "Paina mitä tahansa näppäintä jatkaaksesi. ";
	_getch();
}
void findRoom(std::vector<HotelRoom>& rooms)
{
	string input;
	cout << "Syötä varausnumero tai varaajan nimi: ";
	cin.ignore();
	getline(cin, input);

	vector<HotelRoom> foundRooms;

	while (input.empty())
	{
		cout << "Et antanut varausnumeroa tai varaajan nimeä. Syötä varausnumero tai varaajan nimi: ";
		cin.ignore();
		getline(cin, input);
	}
	// Jos syötteessä on numero, on syötetty varausnumero.
	// Haetaan siis huone annetulla varausnumerolla.
	if (any_of(input.begin(), input.end(), isdigit))
	{
		int inputBookingNumber = stoi(input);
		for (HotelRoom room : rooms)
		{
			if (room.bookingNumber == inputBookingNumber)
			{
				foundRooms.emplace_back(room);
				cout << "Olet varannut huoneen " << room.roomNumber << "\n";
			}
		}
	}
	// Jos syötteessä ei ollut numeroita, on syötetty varaajan nimi.
	// Haetaan siis huoneet varaajan nimellä.
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
		cout << "Varattuja huoneita ei löytynyt. Varmista, että kirjoitit varausnumerosi tai nimesi oikein.\n";
	}
	writeRooms(rooms);
	cout << "Paina mitä tahansa näppäintä jatkaaksesi.";
	_getch();
}

vector<HotelRoom> readRooms()
{
	ifstream inputFile("rooms.json");
	json roomJson = json::parse(inputFile);
	auto rooms = roomJson.template get<vector<HotelRoom>>();
	return rooms;
}
void writeRooms(std::vector<HotelRoom>& rooms)
{
	json roomJson = rooms;
	ofstream outputFile;
	outputFile.open("rooms.json");
	outputFile << roomJson.dump(2);
	outputFile.close();
}
void to_json(json& j, const HotelRoom& room)
{
	j = json{
		{"price", room.price},
		{"size", room.size},
		{"roomNumber", room.roomNumber},
		{"available", room.available},
		{"booker", room.booker},
		{"bookingNumber", room.bookingNumber} };
}
void from_json(const json& j, HotelRoom& room)
{
	j.at("price").get_to(room.price);
	j.at("size").get_to(room.size);
	j.at("roomNumber").get_to(room.roomNumber);
	j.at("available").get_to(room.available);
	j.at("booker").get_to(room.booker);
	j.at("bookingNumber").get_to(room.bookingNumber);
}


