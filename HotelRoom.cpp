#include "HotelRoom.h"
#include <sstream>

// Допоміжна функція: глибока копія списку бронювань
static periodsList copyList(const periodsList& src) {
    periodsList r = {nullptr, src.count};
    if (src.count > 0) {
        r.list = new period[src.count];
        for (int i = 0; i < src.count; i++) r.list[i] = src.list[i];
    }
    return r;
}

// -------------------------Constructors----------------------

HotelRoom::HotelRoom(): roomAmount (0), area (0), number (0), occupancy (0), booking {nullptr, 0}, price (0) {}

HotelRoom::HotelRoom(int _roomAmount, int _area, int _number, int _occupancy,
                     periodsList _booking, int _price)
    : roomAmount(_roomAmount), area(_area), number(_number),
      occupancy(_occupancy), booking(copyList(_booking)), price(_price) {}

HotelRoom::HotelRoom(const HotelRoom& o)
    : roomAmount(o.roomAmount), area(o.area), number(o.number),
      occupancy(o.occupancy), booking(copyList(o.booking)), price(o.price) {}

HotelRoom::~HotelRoom() { delete[] booking.list; }

// --------------------------Get methods----------------------
int HotelRoom::get_roomAmount() const
{
    return roomAmount;
}

int HotelRoom::get_area() const
{
    return area;
}

int HotelRoom::get_number() const
{
    return number;
}

int HotelRoom::get_occupancy() const
{
    return occupancy;
}

periodsList HotelRoom::get_booking() const
{
    return booking;
}

int HotelRoom::get_price() const
{
    return price;
}

//--------------------------Set methods----------------------
void HotelRoom::set_roomAmount(int _roomAmount)
{
    roomAmount = _roomAmount;
}

void HotelRoom::set_area(int _area)
{
    area = _area;
}

void HotelRoom::set_number(int _number)
{
    number = _number;
}

void HotelRoom::set_occupancy(int _occupancy)
{
    occupancy = _occupancy;
}

void HotelRoom::set_booking(periodsList _booking)
{
    delete[] booking.list;
    booking = copyList(_booking);
}

void HotelRoom::set_price(int _price)
{
    price = _price;
}


//---------------------easy class methods-------------------------

void HotelRoom::print()
{
    cout << "Hotel room:" << endl;
    cout << "Number of rooms: " << roomAmount << endl;
    cout << "Area: " << area << endl;
    cout << "Room number: " << number << endl;
    cout << "Occupancy: " << occupancy << endl;
    if(booking.count>0){
        cout << "Booked dates:" << endl;
        for (int i = 0; i < booking.count; i++){
            cout << i+1<< ". ";
            ::print(booking.list[i]);
            cout << endl;
        }
    }
    cout << "Price: " << price << "\n\n";
}

void HotelRoom::input()
{
    cout << "Enter number of rooms: ";
    cin >> roomAmount;
    cout << "Enter area: ";
    cin >> area;
    cout << "Enter room number: ";
    cin >> number;
    cout << "Enter occupancy: ";
    cin >> occupancy;
    cout << "Enter booking dates number: ";
    cin >> booking.count;
    delete[] booking.list;
    booking.list = new period[booking.count];
    for (int i = 0; i < booking.count; i++){
        cout << "Enter booking number " << i + 1 << ": ";
        booking.list[i]=inputPeriod();
    }
    cout << "Enter room price: ";
    cin >> price;
}

void HotelRoom::toFlow(ostream &toF)
{
    toF << roomAmount << endl;
    toF << area << endl;
    toF << number << endl;
    toF << occupancy << endl;
    toF << booking.count << endl;
    if(booking.count>0){
        for (int i = 0; i < booking.count; i++){
            toFile(toF, booking.list[i]);
            toF << endl;
        }
    }
    toF << price << "\n\n";
}

void HotelRoom::fromFlow(istream &fromF)
{
    fromF >> roomAmount ;
    fromF >> area ;
    fromF >> number ;
    fromF >> occupancy ;
    fromF >> booking.count;
    delete[] booking.list;
    booking.list = new period[booking.count];
    if(booking.count>0){
        for (int i = 0; i < booking.count; i++){
            fromFile(fromF, booking.list[i]);
        }
    }
    fromF >> price ;
}



// ----------------------hard class methods-------------------------
bool HotelRoom::available(period x)
{
    for (int i = 0; i < booking.count; i++){
        if(periodOverlap(x,booking.list[i])){
            return false;
        }
    }
    return true;
}

int HotelRoom::periodPrice(period x)
{
    int total = price * daysAmount(x);
    if (daysAmount(x) >= 10) total = total * 9 / 10; // знижка 10%
    return total;
}


// -----------------------Not a class methods------------------------
void print(date _date)
{
    cout << _date.day << "." << _date.month << "." << _date.year;
}

void print(period _period)
{
    print(_period.start);
    cout << "-";
    print(_period.finish);
}

date inputDate()
{
    date _date;
    cin >> _date.day >> _date.month >> _date.year;
    return _date;
}

period inputPeriod()
{
    period _period;
    cout << "Enter start date: ";
    _period.start = inputDate();
    cout << "Enter finish date: ";
    _period.finish = inputDate();
    return _period;
}

void toFile(ostream &toF, date _date)
{
    toF << _date.day << " " << _date.month << " " << _date.year;
}

void toFile(ostream &toF, period _period)
{
    toFile(toF, _period.start);
    toF << "   ";
    toFile(toF, _period.finish);
}

void fromFile(istream &fromF, date &_date)
{
    fromF >> _date.day >> _date.month >> _date.year;
}

void fromFile(istream &fromF, period &_period)
{
    fromFile(fromF, _period.start);
    fromFile(fromF, _period.finish);
}

int fullDate(date _date)
{
    int x;
    x = _date.day + _date.month * 100 + _date.year * 10000;
    return x;
}

bool periodOverlap(period first, period second)
{
    if(fullDate(first.start)<fullDate(second.start)){
        if(fullDate(first.finish)<fullDate(second.start)){
            return false;
        }else{
            return true;
        }
    }else if(fullDate(first.start)>fullDate(second.start)){
        if(fullDate(first.start)<=fullDate(second.finish)){
            return true;
        }else{
            return false;
        }
    }else{
        return true;
    }
}

int daysAmount(period x)
{
    int days;
    days = (x.finish.year - x.start.year) * 365 + (x.finish.month - x.start.month) * 30 + (x.finish.day - x.start.day);
    return days;
}





//---------Operators---------------

// Присвоєння: глибока копія, перевірка на самоприсвоєння (a = a)
HotelRoom& HotelRoom::operator=(const HotelRoom& o) {
    if (this != &o) {
        delete[] booking.list;
        roomAmount = o.roomAmount; area = o.area; number = o.number;
        occupancy = o.occupancy; price = o.price;
        booking = copyList(o.booking);
    }
    return *this;
}

// Порівняння виконується за вартістю (поле price) - згідно з варіантом 9
bool HotelRoom::operator>(const HotelRoom& o) const  { return price > o.price; }
bool HotelRoom::operator<(const HotelRoom& o) const  { return price < o.price; }
bool HotelRoom::operator==(const HotelRoom& o) const { return price == o.price; }
bool HotelRoom::operator!=(const HotelRoom& o) const { return !(*this == o); }

// Приведення до string: повертає рядкове представлення номера
HotelRoom::operator string() const {
    ostringstream oss;
    oss << *this;
    return oss.str();
}

// Оператор +: "об'єднання" двох номерів - сумуються кількість кімнат, площа,
// місткість і ціна, а списки бронювань зливаються в один. Номер береться від лівого операнда.
HotelRoom HotelRoom::operator+(const HotelRoom& o) const {
    HotelRoom res;
    res.roomAmount = roomAmount + o.roomAmount;
    res.area = area + o.area;
    res.number = number;
    res.occupancy = occupancy + o.occupancy;
    res.price = price + o.price;
    res.booking.count = booking.count + o.booking.count;
    res.booking.list = new period[res.booking.count];
    for (int i = 0; i < booking.count; i++) res.booking.list[i] = booking.list[i];
    for (int i = 0; i < o.booking.count; i++) res.booking.list[booking.count + i] = o.booking.list[i];
    return res;
}

// Префіксний ++: збільшує ціну на 1000 і повертає вже змінений об'єкт
HotelRoom& HotelRoom::operator++() {
    price += 1000;
    return *this;
}

// Постфіксний ++: збільшує ціну на 1000, але повертає копію ДО зміни
HotelRoom HotelRoom::operator++(int) {
    HotelRoom old(*this);
    ++(*this);
    return old;
}

// Оператор виводу: працює з будь-яким потоком (cout, ofstream, stringstream)
ostream& operator<<(ostream& os, const HotelRoom& r) {
    os << "Hotel room:\n"
       << "Number of rooms: " << r.get_roomAmount() << "\n"
       << "Area: " << r.get_area() << "\n"
       << "Room number: " << r.get_number() << "\n"
       << "Occupancy: " << r.get_occupancy() << "\n";
    periodsList b = r.get_booking();
    if (b.count > 0) {
        os << "Booked dates:\n";
        for (int i = 0; i < b.count; i++) {
            os << i + 1 << ". "
               << b.list[i].start.day << "." << b.list[i].start.month << "." << b.list[i].start.year << "-"
               << b.list[i].finish.day << "." << b.list[i].finish.month << "." << b.list[i].finish.year << "\n";
        }
    }
    os << "Price: " << r.get_price() << "\n";
    return os;
}

// Оператор вводу: читає у форматі файлу HaliciaHotel.txt (без запрошень)
istream& operator>>(istream& is, HotelRoom& r) {
    r.fromFlow(is);
    return is;
}

void mergeString(string* arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    string* L = new string[n1];
    string* R = new string[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else              arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeStringSort(string* arr, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    mergeStringSort(arr, left, mid);
    mergeStringSort(arr, mid + 1, right);
    mergeString(arr, left, mid, right);
}

void lowerString(string& a){
    for(char& c : a){
        if(c>='A' && c<='Z'){
            c += 32;
        }
    }
}

void lowerStringArray(string *a, int n){
    for (int i = 0; i < n; i++){
        lowerString(a[i]);
    }
}
