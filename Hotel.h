#include "HotelRoom.h"

class Hotel {
    private:
        HotelRoom *rooms;
        int bookedNum;
        int roomsNum;
    public:
        Hotel();
        Hotel(HotelRoom *_rooms, int _bookedNum, int _roomsNum);
        Hotel(const Hotel& _other);
        Hotel(istream &in);
        ~Hotel();

        // getters
        HotelRoom* getRooms() const;
        int getBookedNum() const;
        int getRoomsNum() const;

        // setters
        void setRooms(HotelRoom *_rooms, int _roomsNum);
        void setBookedNum(int _bookedNum);
        void setRoomsNum(int _roomsNum);

        //Other functions
        void addRoom(HotelRoom _room);
};