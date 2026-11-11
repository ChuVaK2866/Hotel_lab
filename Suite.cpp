#include "Suite.h"


// --------------Constructors-------------------------
Suite::Suite() : HotelRoom(), amenities(nullptr),amenitiesCount(0), bathroomCount(0){}
Suite::Suite(int _roomAmount, int _area, int _number, int _occupancy,
             periodsList _booking, int _price, const string* _amenities, int _amenitiesCount, int _bathroomCount) : HotelRoom(_roomAmount, _area, _number, _occupancy, _booking, _price), amenities(nullptr), amenitiesCount(_amenitiesCount), bathroomCount(_bathroomCount) { if(amenitiesCount>0){
                amenities = new string[amenitiesCount];
        for(int i=0; i<amenitiesCount; ++i){
            amenities[i]=_amenities[i];
        }
    }
}
Suite::Suite(const Suite& _copy):HotelRoom(_copy),amenities(nullptr),amenitiesCount(_copy.amenitiesCount), bathroomCount(_copy.bathroomCount){
    if(amenitiesCount>0){
        amenities = new string[amenitiesCount];
        for(int i=0; i<amenitiesCount; ++i){
            amenities[i]=_copy.amenities[i];
        }
    }
}

Suite::~Suite() { delete[] amenities; }

Suite& Suite::operator=(const Suite& o) {
    if (this != &o) {
        HotelRoom::operator=(o);         // присвоєння бази
        delete[] amenities;
        amenitiesCount = o.amenitiesCount;
        amenities = nullptr;
        bathroomCount = o.bathroomCount;
        if (amenitiesCount > 0) {
            amenities = new string[amenitiesCount];
            for (int i = 0; i < amenitiesCount; i++)
                amenities[i] = o.amenities[i];
        }
    }
    return *this;
}

//-------------Other methods--------------
int Suite::periodPrice(period x){
    int total = 2 * HotelRoom::get_price() * daysAmount(x);
    if(bathroomCount>1){
        total += total * 2 / 10;
    }
    total+=50*amenitiesCount*daysAmount(x);
    if (daysAmount(x) >= 10) total = total * 9 / 10; // знижка 10%
    return total;
}


void Suite::addAmenity(string amenity) {
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
bool Suite::meetsStandard() const
{
    const int REQ = 4;
    const string required[REQ] = {"tv", "safe", "fridge", "hairdryer"};

    if (get_area() < 30 || bathroomCount < 1) return false;

    for (int r = 0; r < REQ; r++) {
        bool found = false;
        for (int i = 0; i < amenitiesCount; i++) {
            string a = amenities[i];
            lowerString(a);                 // регістр не важливий
            if (a == required[r]) { found = true; break; }
        }
        if (!found) return false;
    }
    return true;
}
void Suite::print() {
    HotelRoom::print();
    cout << "Bathrooms: " << bathroomCount << "\nAmenities: ";
    for (int i = 0; i < amenitiesCount; i++)
        cout << amenities[i] << (i + 1 < amenitiesCount ? ", " : "");
    cout << "\n\n";
}

void Suite::input() {
    HotelRoom::input();
    cout << "Enter bathrooms count: ";
    cin >> bathroomCount;
    cout << "Enter amenities count: ";
    cin >> amenitiesCount;
    delete[] amenities;
    amenities = amenitiesCount > 0 ? new string[amenitiesCount] : nullptr;
    for (int i = 0; i < amenitiesCount; i++) {
        cout << "Enter amenity " << i + 1 << ": ";
        cin >> ws;
        getline(cin, amenities[i]);
    }
}

void Suite::toFlow(ostream &toF) {
    HotelRoom::toFlow(toF);
    toF << bathroomCount << endl << amenitiesCount << endl;
    for (int i = 0; i < amenitiesCount; i++) toF << amenities[i] << endl;
    toF << endl;
}

void Suite::fromFlow(istream &fromF) {
    HotelRoom::fromFlow(fromF);
    fromF >> bathroomCount >> amenitiesCount;
    delete[] amenities;
    amenities = amenitiesCount > 0 ? new string[amenitiesCount] : nullptr;
    for (int i = 0; i < amenitiesCount; i++) {
        fromF >> ws;
        getline(fromF, amenities[i]);
    }
}