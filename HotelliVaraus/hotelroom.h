#pragma once
#include <vector>
class HotelRoom 
{
public:
    double price;
    int size; //Henkil�iden m��r� (1h tai 2h)
    int roomNumber;
    bool available;
    std::string booker;
    int bookingNumber;

    std::vector<HotelRoom> generateRooms();
    void bookRoom(std::vector<HotelRoom>& rooms);
    void findRoom();

};