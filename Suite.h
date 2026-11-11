#pragma once
#include "HotelRoom.h"

class Suite : public HotelRoom {
        string* amenities;
        int amenitiesCount;
        int bathroomCount;
    public:
        Suite();
        Suite(int _roomAmount, int _area, int _number, int _occupancy,
             periodsList _booking, int _price, const string* _amenities, int _amenitiesCount,int _bathroomCount);
        Suite(const Suite& _copy);
        ~Suite();
        Suite& operator=(const Suite& _copy);
        
                int periodPrice(period x) override;
        void addAmenity(string amenity);
        bool meetsStandard() const;
void print() override;
void input() override;
void toFlow(ostream &toF) override;
void fromFlow(istream &fromF) override;
};