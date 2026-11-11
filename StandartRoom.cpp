#include "StandartRoom.h"


// --------------Constructors-------------------------
StandartRoom::StandartRoom() : HotelRoom(), amenities(nullptr),amenitiesCount(0){}
StandartRoom::StandartRoom(int _roomAmount, int _area, int _number, int _occupancy,
             periodsList _booking, int _price, const string* _amenities, int _amenitiesCount) : HotelRoom(_roomAmount, _area, _number, _occupancy, _booking, _price), amenities(nullptr), amenitiesCount(_amenitiesCount) { if(amenitiesCount>0){
                amenities = new string[amenitiesCount];
        for(int i=0; i<amenitiesCount; ++i){
            amenities[i]=_amenities[i];
        }
    }
}
StandartRoom::StandartRoom(const StandartRoom& _copy):HotelRoom(_copy),amenities(nullptr),amenitiesCount(_copy.amenitiesCount){
    if(amenitiesCount>0){
        amenities = new string[amenitiesCount];
        for(int i=0; i<amenitiesCount; ++i){
            amenities[i]=_copy.amenities[i];
        }
    }
}

StandartRoom::~StandartRoom() { delete[] amenities; }

StandartRoom& StandartRoom::operator=(const StandartRoom& o) {
    if (this != &o) {
        HotelRoom::operator=(o);         // присвоєння бази
        delete[] amenities;
        amenitiesCount = o.amenitiesCount;
        amenities = nullptr;
        if (amenitiesCount > 0) {
            amenities = new string[amenitiesCount];
            for (int i = 0; i < amenitiesCount; i++)
                amenities[i] = o.amenities[i];
        }
    }
    return *this;
}


//-------------Other methods--------------
int StandartRoom::periodPrice(period x){
    int total = HotelRoom::periodPrice(x);
    total+=50*amenitiesCount*daysAmount(x);
    return total;
}


void StandartRoom::addAmenity(string amenity) {
    for (int i=0; i<amenitiesCount; ++i){
        if(amenities[i]==amenity){
            return;
        }
    }
    string* temp= new string[amenitiesCount+1];
    for(int i=0; i<amenitiesCount;++i){
        temp[i]=amenities[i];
    }
    temp[amenitiesCount]=amenity;
    delete[] amenities;
    amenities=temp;
    amenitiesCount++;
}