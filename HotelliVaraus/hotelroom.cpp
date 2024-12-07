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

// Tiedoston k�sittelyyn
void writeRooms(std::vector<HotelRoom>& rooms);
void from_json(const json& j, HotelRoom& room);
void to_json(json& j, const HotelRoom& room);
vector<HotelRoom> readRooms();

// Huoneiden luonti
vector<HotelRoom> generateRooms()
{
	vector<HotelRoom> rooms;
	random_device rd;
	default_random_engine generator(rd());
	// Huoneiden m��r� v�lilt� 40-300. (41 muuttuu numeroksi 40)
	uniform_int_distribution<int> distribution(41, 300);

	int numOfRooms = distribution(generator);
	// Tarkistetaan, ett� huoneiden m��r� on parillinen, jos ei ole parillinen poistetaan yksi huone.
	numOfRooms = (numOfRooms % 2 == 0) ? numOfRooms : numOfRooms - 1;

	//Huoneiden tietojen t�ytt�
	for (int i = 0; i < numOfRooms; i++)
	{	
		HotelRoom hotelRoom;
		// Ensimm�isen puolikkaan huoneet 1 henkil�lle, toisen puolen 2 henkil�lle 
		hotelRoom.size = (i < numOfRooms / 2) ? 1 : 2;
		hotelRoom.price = (hotelRoom.size == 1) ? 100 : 150;
		hotelRoom.roomNumber = i + 1;
		// 40% mahdollisuus ett� huone on jo varattu
		distribution = uniform_int_distribution<int>(1, 10);
		int isAvailableRNG = distribution(generator);
		hotelRoom.available = (isAvailableRNG <= 4) ? false : true; 
		if (!hotelRoom.available)
		{
			hotelRoom.booker = "Muu Varaaja";
		}
		hotelRoom.nightAmount = 0;
		hotelRoom.bookingNumber = 0;
		rooms.emplace_back(hotelRoom);
	}
	// Huoneiden tietojen t�yt�n j�lkeen laitetaan ne json- muodossa tiedostoon
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

		// Jos kaikki huoneet on jo varattu palataan takaisin p��valikkoon
		if (availableRooms.empty())
		{
			cout << "Valitettavasti kaikki huoneemme ovat jo varattuja.";
			cout << "Paina mit� tahansa n�pp�int� jatkaaksesi. ";
			_getch(); //_getch() odottaa ett� k�ytt�j� antaa yhden merkin, toimii k�tev�sti t�ss�
			return;
		}

		cout << "Valitse saatavilla oleva huone: ";
		cin >> input;

		// T�m�n voisi tehd� paremmin mutta en ehdi
		// Varmistetaan, ett� k�ytt�j�n antama huonenumero on saatavilla
		// Eli jos se l�ytyy saatavilla olevista huoneista sen voi varata, muuten pyydet��n uusi numero
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
		// Jos k�ytt�j�n antama huone on saatavilla
		selectedRoom = rooms[input - 1];
	}
	// Jos k�ytt�j� vastasi ett� ei halua itse valita huoneen numeroa, valitaan satunnainen huone
	else
	{
		// Katsotaan mitk� huoneet ovat saatavilla, ja sitten valitaan niist� yksi satunnainen
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
	// Maksimim��r�n voi nostaa jos haluaa ett� voi varata pidemm�ksi aikaa
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
	
	HotelRoom& room = rooms[selectedRoom.roomNumber - 1];

	// Satunnainen alennus, 0 10 tai 20 prosenttia
	uniform_int_distribution<int> distribution(1, 3);
	int discount = distribution(generator);
	int discountMultiplier = (discount == 1?0:discount==2?10:20);
	// Hinta on �iden m��r� kerrottuna hinnalla alennusten j�lkeen
	room.price = nightAmount * room.price * (100 - discountMultiplier) / 100;
	cout << "Huoneen hinta " << nightAmount << " y�ksi on " << room.price << " euroa\n";
	cout << "Haluatko varmasti varata huoneen " << room.roomNumber << " " << nightAmount << " y�ksi? (K/E)\n";
	
	// Varauksen varmistus
	string selectionInput;
	cin >> selectionInput;
	if (selectionInput != "K" && selectionInput != "k")
	{
		return;
	}

	// Varatun huoneen tietojen muutto
	distribution = uniform_int_distribution<int>(10000, 99999);
	room.bookingNumber = distribution(generator);
	room.available = false;
	room.booker = inputName;
	room.nightAmount = nightAmount;

	cout << "Varasit huoneen " << room.roomNumber << ".\n";
	cout << "Varausnumerosi on " << room.bookingNumber << ".\n";
	// P�ivitet��n huonetiedot tiedostoon
	writeRooms(rooms);
	cout << "Paina mit� tahansa n�pp�int� jatkaaksesi. ";
	_getch();
}

// Varattujen huoneiden katsominen
// Huoneita voi hakea joko varausnumerolla tai varaajan nimell�
// Jos k�ytt�j�n sy�tteess� on numero, oletetaan ett� kyseess� on varausnumero, muuten oletetaan ett� on varaajan nimi
void findRoom(std::vector<HotelRoom>& rooms)
{
	string input;
	cout << "Sy�t� varausnumero tai varaajan nimi: ";
	cin.ignore();
	getline(cin, input);

	// T�m� vektori on olemassa jotta voimme kertoa k�ytt�j�lle jos mit��n huonetta ei l�ytynyt (jos t�m� on tyhj�)
	vector<HotelRoom> foundRooms;

	while (input.empty())
	{
		cout << "Et antanut varausnumeroa tai varaajan nime�. Sy�t� varausnumero tai varaajan nimi: ";
		cin.ignore();
		getline(cin, input);
	}

	// Varausnumero
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

// json-huonetiedostosta luku
vector<HotelRoom> readRooms()
{
	// Muunnetaan luettu json hotellihuonevektoriksi
	ifstream inputFile("rooms.json");
	json roomJson = json::parse(inputFile);
	auto rooms = roomJson.template get<vector<HotelRoom>>();
	return rooms;
}
// json-huonetiedostoon kirjoittaminen
void writeRooms(vector<HotelRoom>& rooms)
{
	json roomJson = rooms;
	ofstream outputFile;
	outputFile.open("rooms.json");
	outputFile << roomJson.dump(2);
	outputFile.close();
}
void to_json(json& j, const HotelRoom& room)
{
	// Hotellihuoneen muunto jsoniksi
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
	// jsonista muunto hotellihuoneeksi
	j.at("price").get_to(room.price);
	j.at("size").get_to(room.size);
	j.at("roomNumber").get_to(room.roomNumber);
	j.at("available").get_to(room.available);
	j.at("booker").get_to(room.booker);
	j.at("bookingNumber").get_to(room.bookingNumber);
	j.at("nightAmount").get_to(room.nightAmount);
}


