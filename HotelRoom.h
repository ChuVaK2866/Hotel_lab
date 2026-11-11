#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// --------------------Structures----------------------
struct date{
    int day;
    int month;
    int year;
};

struct period{
    date start;
    date finish;
};

struct periodsList
{
    period* list;
    int count;
};

//--------------------Structure functions---------------------
void print(date _date);
void print(period _period);
date inputDate();
period inputPeriod();
void toFile(ostream& toF, date _date);
void toFile(ostream &toF, period _period);
void fromFile(istream &fromF, date &_date);
void fromFile(istream &fromF, period &_period);
int fullDate(date _date);
bool periodOverlap(period first, period second);
int daysAmount(period x);

// ----------------------Class-------------------------------
class HotelRoom{
private:
    int roomAmount;
    int area;
    int number;
    int occupancy;
    periodsList booking;
    int price;

public:
    HotelRoom();
    HotelRoom(
        int _roomAmount,
        int _area,
        int _number,
        int _occupancy,
        periodsList _booking,
        int _price);
    HotelRoom(const HotelRoom& _HotelRoom);
    virtual ~HotelRoom();

    int get_roomAmount()const;
    int get_area() const;
    int get_number() const;
    int get_occupancy() const;
    periodsList get_booking() const;
    int get_price() const;
    void set_roomAmount(int _roomAmount);
    void set_area(int _area);
    void set_number(int _number);
    void set_occupancy(int _occupancy);
    void set_booking(periodsList _booking);
    void set_price(int _price);
    virtual void print();
    virtual void input();
    virtual void toFlow(ostream &toF);
    virtual void fromFlow(istream &fromF);

    bool available(period x);
    virtual int periodPrice(period x);

    // ------Перевантаження операторів-------
    HotelRoom& operator=(const HotelRoom& other);
    bool operator>(const HotelRoom& o) const;
    bool operator<(const HotelRoom& o) const;
    bool operator==(const HotelRoom& o) const;
    bool operator!=(const HotelRoom& o) const;
    operator string() const;
    HotelRoom operator+(const HotelRoom& o) const;
    HotelRoom& operator++();     // префіксний
    HotelRoom operator++(int);   // постфіксний
};

ostream& operator<<(ostream& os, const HotelRoom& r);
istream& operator>>(istream& is, HotelRoom& r);

//----------Other functions-------------

void mergeString(string *arr, int left, int mid, int right);
void mergeStringSort(string *arr, int left, int right);
void lowerString(string& a);
void lowerStringArray(string* a, int n);