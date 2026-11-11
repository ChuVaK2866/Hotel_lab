#include "Hotel.h"
#include <iostream>
using namespace std;

Hotel::Hotel() : rooms(nullptr), bookedNum(0), roomsNum(0) {}

Hotel::Hotel(HotelRoom *_rooms, int _bookedNum, int _roomsNum)
    : bookedNum(_bookedNum), roomsNum(_roomsNum) {
    if (bookedNum>roomsNum){
        roomsNum = bookedNum;
    }
    rooms = new HotelRoom[roomsNum];
    for (int i = 0; i < roomsNum; i++) {
        rooms[i] = _rooms[i];
    }
}

Hotel::Hotel(const Hotel& _other)
    : bookedNum(_other.bookedNum), roomsNum(_other.roomsNum) {
    rooms = new HotelRoom[roomsNum];
    for (int i = 0; i < roomsNum; i++) {
        rooms[i] = _other.rooms[i];
    }
}

Hotel::Hotel(istream &in) {
    in >> roomsNum >> bookedNum;
    if (bookedNum>roomsNum){
        roomsNum = bookedNum;
    }
    rooms = new HotelRoom[roomsNum];
    for (int i = 0; i < roomsNum; i++) {
        rooms[i].fromFlow(in);
    }
}

Hotel::~Hotel() {
    delete[] rooms;
}

// getters
HotelRoom* Hotel::getRooms() const { return rooms; }
int Hotel::getBookedNum() const { return bookedNum; }
int Hotel::getRoomsNum() const { return roomsNum; }

// setters
void Hotel::setRooms(HotelRoom *_rooms, int _roomsNum) {
    delete[] rooms;
    roomsNum = _roomsNum;
    rooms = new HotelRoom[roomsNum];
    for (int i = 0; i < roomsNum; i++) {
        rooms[i] = _rooms[i];
    }
}

void Hotel::setBookedNum(int _bookedNum) { bookedNum = _bookedNum; }
void Hotel::setRoomsNum(int _roomsNum) { roomsNum = _roomsNum; }