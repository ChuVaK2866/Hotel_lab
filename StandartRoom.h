#pragma once
#include "HotelRoom.h"

class StandartRoom : public HotelRoom{
        string* amenities;
        int amenitiesCount;
    public:
        StandartRoom();
        StandartRoom(int _roomAmount, int _area, int _number, int _occupancy,
             periodsList _booking, int _price, const string* _amenities, int _amenitiesCount);
        StandartRoom(const StandartRoom& _copy);
        ~StandartRoom();
        StandartRoom& operator=(const StandartRoom& _copy);
        
        int periodPrice(period x) override;
        void addAmenity(string amenity);
        

};