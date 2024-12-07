#pragma once
#include <vector>
#include "json.hpp"

    class HotelRoom
    {
    public:
        double price; 
        int size; //Henkilöiden määrä (1h tai 2h)
        int roomNumber;
        bool available;
        std::string booker; // Varaajan nimi
        int bookingNumber;
        int nightAmount;


        std::vector<HotelRoom> generateRooms();
        void bookRoom(std::vector<HotelRoom>& rooms);
        void findRoom(std::vector <HotelRoom>& rooms);
        void writeRooms(std::vector<HotelRoom>& rooms, HotelRoom sroom);
    };