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
			hotelRoom.booker = "Muu Varaaja";
		}
		hotelRoom.nightAmount = 0;
		hotelRoom.bookingNumber = 0;
		rooms.emplace_back(hotelRoom);
	}
	writeRooms(rooms);
	return rooms;
}

// Huoneiden varaus
void bookRoom(vector <HotelRoom>& rooms)
{
	HotelRoom selectedRoom;
	random_device rd;
	default_random_engine generator(rd());

	int input;
	char input2;

	cout << "Haluatko varata huoneen yhdelle (1) vai kahdelle (2) henkil�lle?";
	cin >> input;
	// Jos vastaus ei ollut 1 tai 2, kysyt��n uudelleen
	while (cin.fail() || input < 1 || input > 2)
	{
		cout << "Sy�tteesi oli virheellinen. Haluatko varata huoneen yhdelle (1) vai kahdelle (2) henkil�lle?";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> input;
	}
	cout << "Haluatko valita huoneen numeron? (K/E):";
	cin >> input2; 
	while (input2 != 'K' && input2 != 'k' && input2 != 'E' && input2 != 'e')
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Sy�tteesi oli virheellinen.  Haluatko valita huoneen numeron? (K/E):";
		cin >> input2;
	}

	// K�ytt�j�n huoneen numeron valinta
	if (input2 == 'K' || input2 == 'k')
	{
		cout << "Saatavilla olevat huoneet: \n";
		vector<HotelRoom> availableRooms;
		for (HotelRoom& room : rooms)
		{
			if (room.available && input == 1 && room.size == 1)
			{
				cout << room.roomNumber << '\n';
				availableRooms.emplace_back(room);
			}
			else if (room.available && input == 2 && room.size == 2)
			{
				cout << room.roomNumber << '\n';
				availableRooms.emplace_back(room);
			}
		}

		cout << "Valitse saatavilla oleva huone: ";
		cin >> input;

		// T�m�n voisi varmasti tehd� paremmin mutta en ehdi
		bool roomFound = false;
		while (!roomFound)
		{
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
			for (HotelRoom room : availableRooms)
			{
				if (room.roomNumber == input)
				{
					roomFound = true;
				}
			}
			if (!roomFound)
			{
				cout << "Valitsemasi huone ei ole saatavilla. Valitse saatavilla oleva huone: ";
				cin >> input;
			}
		}

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

		uniform_int_distribution<int> distribution(1, availableRooms.size());
		selectedRoom = availableRooms[distribution(generator)];
	}

	// Raja �iden m��r�lle, koska ei ole haluttavaa ett� joku varaisi huoneen miljoonaksi y�ksi
	int nightAmount;
	cout << "Kuinka moneksi y�ksi haluat varata huoneen? (max. 30pv): ";
	cin >> nightAmount;
	while(nightAmount >30 || nightAmount <1 || cin.fail())
	{ 
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Anna �iden m��r� v�lilt� 1-30: ";
		cin >> nightAmount;
	}
	
	string inputName;
	cout << "Anna nimesi: \n";
	cin.ignore();
	getline(cin, inputName);

	HotelRoom& SRoom = rooms[selectedRoom.roomNumber - 1];

	// Satunnainen alennus
	uniform_int_distribution<int> distribution(1, 3);
	int discount = distribution(generator);
	int discountMultiplier = (discount == 1?0:discount==2?10:20);
	SRoom.price = nightAmount * SRoom.price * (100 - discountMultiplier) / 100;
	cout << "Huoneen hinta " << nightAmount << " y�ksi on " << SRoom.price << " euroa\n";
	cout << "Haluatko varmasti varata huoneen " << SRoom.roomNumber << " " << nightAmount << " y�ksi? (K/E)\n";
	
	string selectionInput;
	cin >> selectionInput;
	if (selectionInput != "K" && selectionInput != "k")
	{
		return;
	}

	distribution = uniform_int_distribution<int>(10000, 99999);
	SRoom.bookingNumber = distribution(generator);
	SRoom.available = false;
	SRoom.booker = inputName;
	SRoom.nightAmount = nightAmount;

	cout << "Varasit huoneen " << SRoom.roomNumber << ".\n";
	cout << "Varausnumerosi on " << SRoom.bookingNumber << ".\n";
	writeRooms(rooms);
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

	while (input.empty())
	{
		cout << "Et antanut varausnumeroa tai varaajan nime�. Sy�t� varausnumero tai varaajan nimi: ";
		cin.ignore();
		getline(cin, input);
	}
	// Jos sy�tteess� on numero, on sy�tetty varausnumero.
	// Haetaan siis huone annetulla varausnumerolla.
	if (any_of(input.begin(), input.end(), isdigit) && input!= "0")
	{
		int inputBookingNumber = stoi(input);
		for (HotelRoom room : rooms)
		{
			if (room.bookingNumber == inputBookingNumber)
			{
				foundRooms.emplace_back(room);
				cout << "Olet varannut huoneen " << room.roomNumber << " " << room.nightAmount << " y�ksi.\n";
			}
		}
	}
	// Jos sy�tteess� ei ollut numeroita, on sy�tetty varaajan nimi.
	// Haetaan siis huoneet varaajan nimell�.
	else
	{
		for (HotelRoom room : rooms)
		{
			if (room.booker == input)
			{
				foundRooms.emplace_back(room);
				cout << "Olet varannut huoneen " << room.roomNumber << " " << room.nightAmount << " y�ksi.\n";
			}
		}
	}

	if (foundRooms.empty())
	{
		cout << "Varattuja huoneita ei l�ytynyt. Varmista, ett� kirjoitit varausnumerosi tai nimesi oikein.\n";
	}
	writeRooms(rooms);
	cout << "Paina mit� tahansa n�pp�int� jatkaaksesi.";
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
		{"bookingNumber", room.bookingNumber},
		{"nightAmount", room.nightAmount } };

}
void from_json(const json& j, HotelRoom& room)
{
	j.at("price").get_to(room.price);
	j.at("size").get_to(room.size);
	j.at("roomNumber").get_to(room.roomNumber);
	j.at("available").get_to(room.available);
	j.at("booker").get_to(room.booker);
	j.at("bookingNumber").get_to(room.bookingNumber);
	j.at("nightAmount").get_to(room.nightAmount);
}


