#include "HotelRoom.h"
#include "StandartRoom.h"
#include "Suite.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

const int ROOMS_COUNT = 6; // початкова кількість номерів у файлі

void showMainMenu();
int chooseRoomIndex(int size);
void roomSubmenu(HotelRoom &r);
int readInt();
void saveRooms(HotelRoom *rooms, int size, const char *filename);
HotelRoom *addRoom(HotelRoom *rooms, int &size);
void demo(ostream &out, HotelRoom *rooms, int n);
HotelRoom *filterByPrice(const HotelRoom *arr, int n, int minPrice, int maxPrice, int &outCount);
void findForPersons(HotelRoom *rooms, int n, int persons, period p);
void polymorphismDemo();

int main() {
    // ---------- 1. Demonstrate constructors ----------
    cout << "===== Demonstration of constructors =====\n\n";

    HotelRoom room1; // default constructor
    cout << "Room 1 (default constructor):\n";
    room1.print();

    period p1;
    p1.start = {1, 6, 2026};
    p1.finish = {10, 6, 2026};
    periodsList bookingList;
    bookingList.count = 1;
    bookingList.list = new period[1];
    bookingList.list[0] = p1;

    HotelRoom room2(2, 60, 100, 3, bookingList, 500); // parametrized constructor
    cout << "Room 2 (parametrized constructor):\n";
    room2.print();

    HotelRoom room3(room2); // copy constructor
    cout << "Room 3 (copy of room 2):\n";
    room3.print();

    delete[] bookingList.list; // конструктор зробив глибоку копію, тож наш список звільняємо самі
    bookingList.list = nullptr;

    // ---------- 1.5. Inheritance and polymorphism ----------
    polymorphismDemo();

    // ---------- 2. Load dynamic array of rooms from file ----------
    cout << "===== Loading " << ROOMS_COUNT << " rooms from file =====\n\n";

    int roomsCount = ROOMS_COUNT;
    HotelRoom *rooms = new HotelRoom[roomsCount]; // динамічний масив

    ifstream fin("HaliciaHotel.txt");
    if (!fin.is_open()) {
        cout << "Could not open HaliciaHotel.txt\n";
    } else {
        for (int i = 0; i < roomsCount; i++) {
            fin >> rooms[i]; // перевантажений operator>> (викликає віртуальний fromFlow)
        }
        fin.close();
        cout << "Loaded " << roomsCount << " rooms.\n\n";

        // ---------- Демонстрація операторів ----------
        // demo змінює масив (сортує, збільшує ціни), тому кожному виклику даємо свіжу копію
        HotelRoom *tmp = new HotelRoom[roomsCount];
        for (int i = 0; i < roomsCount; i++) tmp[i] = rooms[i]; // operator=
        demo(cout, tmp, roomsCount);
        delete[] tmp;

        tmp = new HotelRoom[roomsCount];
        for (int i = 0; i < roomsCount; i++) tmp[i] = rooms[i];
        ofstream res("Result.txt");
        demo(res, tmp, roomsCount);
        res.close();
        delete[] tmp;

        // ---------- Фільтр за діапазоном ----------
        int cnt;
        HotelRoom *filtered = filterByPrice(rooms, roomsCount, 1000, 2500, cnt);
        cout << "\nRooms with price in [1000; 2500]: " << cnt << "\n";
        ofstream res2("Result2.txt");
        for (int i = 0; i < cnt; i++) {
            cout << filtered[i] << "\n";
            res2 << filtered[i] << "\n";
        }
        res2.close();
        delete[] filtered;
    }

    // ---------- 3. Menu ----------
    int choice = -1;
    do {
        showMainMenu();
        choice = readInt();

        switch (choice) {
            case 1: { // show all rooms
                for (int i = 0; i < roomsCount; i++) {
                    cout << "--- Room " << i + 1 << " ---\n";
                    rooms[i].print();
                }
                break;
            }
            case 2: { // work with a specific room
                int idx = chooseRoomIndex(roomsCount);
                if (idx >= 0) {
                    roomSubmenu(rooms[idx]);
                }
                break;
            }
            case 3: { // enter new data for a room
                int idx = chooseRoomIndex(roomsCount);
                if (idx >= 0) {
                    rooms[idx].input();
                    cout << "Room updated.\n";
                }
                break;
            }
            case 4: { // save all rooms to file
                saveRooms(rooms, roomsCount, "HaliciaHotel_out.txt");
                break;
            }
            case 5: { // find free rooms for period
                cout << "Enter period to search:\n";
                period p = inputPeriod();
                bool found = false;
                for (int i = 0; i < roomsCount; i++) {
                    if (rooms[i].available(p)) {
                        cout << "Room " << i + 1 << " is free. Price for period: "
                             << rooms[i].periodPrice(p) << "\n";
                        found = true;
                    }
                }
                if (!found) {
                    cout << "No free rooms for this period.\n";
                }
                break;
            }
            case 6: { // add a new room (array grows)
                rooms = addRoom(rooms, roomsCount);
                cout << "Room added. Total rooms: " << roomsCount << "\n";
                break;
            }
            case 7: { // find rooms for N persons in a period
                cout << "Enter number of persons: ";
                int persons = readInt();
                cout << "Enter period:\n";
                period p = inputPeriod();
                findForPersons(rooms, roomsCount, persons, p);
                break;
            }
            case 0:
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid choice, try again.\n";
        }

    } while (choice != 0);

    delete[] rooms; // звільняємо динамічний масив
    rooms = nullptr;

    return 0;
}

// ---------------------------------------------------------------------
// Завдання 3 (частина 2): похідні класи StandartRoom та Suite, поліморфізм
// ---------------------------------------------------------------------
void polymorphismDemo() {
    cout << "\n===== Demonstration of inheritance and polymorphism =====\n\n";

    period p = {{1, 7, 2026}, {12, 7, 2026}}; // 11 днів -> спрацьовує знижка 10%
    periodsList none = {nullptr, 0};

    string amStd[]   = {"tv", "hairdryer"};
    string amSuite[] = {"tv", "safe", "fridge"}; // для стандарту люкса ще бракує hairdryer

    // Масив вказівників на БАЗОВИЙ клас: базовий об'єкт + два похідні
    HotelRoom *arr[3];
    arr[0] = new HotelRoom(1, 20, 201, 2, none, 1000);
    arr[1] = new StandartRoom(2, 30, 202, 2, none, 1000, amStd, 2);
    arr[2] = new Suite(3, 60, 203, 4, none, 1000, amSuite, 3, 2);

    // --- Віртуальні print() і periodPrice(): ОДИН І ТОЙ САМИЙ виклик через
    //     вказівник на базовий клас, але різні результати для різних класів ---
    cout << "--- Virtual print() and periodPrice() via base pointers ---\n";
    for (int i = 0; i < 3; i++) {
        cout << "arr[" << i << "]:\n";
        arr[i]->print();                                   // виконується print() реального класу
        cout << "periodPrice(1.7.2026-12.7.2026) = "
             << arr[i]->periodPrice(p) << "\n\n";           // виконується periodPrice() реального класу
    }

    // --- Явний виклик базової версії для похідного об'єкта: видно різницю ---
    cout << "--- Base version vs overridden version for the same Suite object ---\n";
    cout << "arr[2]->HotelRoom::periodPrice(p) (base): " << arr[2]->HotelRoom::periodPrice(p) << "\n";
    cout << "arr[2]->periodPrice(p) (Suite):          " << arr[2]->periodPrice(p) << "\n\n";

    // --- Методи, які є лише в похідних класах ---
    StandartRoom *st = static_cast<StandartRoom *>(arr[1]);
    cout << "--- StandartRoom::addAmenity (повторні зручності не додаються) ---\n";
    st->addAmenity("safe");
    st->addAmenity("safe"); // дубль - ігнорується
    st->addAmenity("TV");   // інший регістр - вважається іншим рядком
    arr[1]->print();

    Suite *su = static_cast<Suite *>(arr[2]);
    cout << "--- Suite::meetsStandard ---\n";
    cout << "Before adding hairdryer: " << (su->meetsStandard() ? "meets standard" : "does NOT meet standard") << "\n";
    su->addAmenity("Hairdryer"); // регістр не важливий при перевірці
    cout << "After adding hairdryer:  " << (su->meetsStandard() ? "meets standard" : "does NOT meet standard") << "\n\n";

    // --- operator= для похідних класів (глибоке копіювання) ---
    cout << "--- operator= for derived classes ---\n";
    Suite suCopy;
    suCopy = *su;
    suCopy.addAmenity("minibar"); // змінюємо копію - оригінал не повинен змінитись
    cout << "Copy (with extra amenity):\n";
    suCopy.print();
    cout << "Original (unchanged):\n";
    su->print();

    StandartRoom stCopy;
    stCopy = *st;
    cout << "StandartRoom copy:\n";
    stCopy.print();

    // --- Віртуальне введення/виведення через потоки (read/write) ---
    // toFlow - віртуальний запис, fromFlow - віртуальне читання.
    cout << "--- Virtual toFlow / fromFlow via base pointer ---\n";
    stringstream ss;
    arr[2]->toFlow(ss); // записує і базові, і похідні поля Suite
    cout << "Serialized Suite:\n" << ss.str();

    HotelRoom *loaded = new Suite(); // читаємо в новий Suite через вказівник на базовий клас
    loaded->fromFlow(ss);
    cout << "Loaded from stream:\n";
    loaded->print();
    delete loaded;

    // Читання з заздалегідь підготовленого тексту (формат: базові поля, потім ванні та зручності)
    istringstream in("2\n40\n301\n3\n0\n1500\n2\n3\ntv\nsafe\nbar\n");
    arr[2]->fromFlow(in);
    cout << "Suite after fromFlow from text:\n";
    arr[2]->print();

    // --- Звільнення пам'яті через вказівники на базовий клас ---
    // Працює коректно завдяки віртуальному деструктору HotelRoom.
    for (int i = 0; i < 3; i++) delete arr[i];

    cout << "===== End of inheritance demonstration =====\n\n";
}

void showMainMenu() {
    cout << "\n===== Halicia Hotel Rooms - Main Menu =====\n";
    cout << "1. Show all rooms\n";
    cout << "2. Work with a specific room (test its methods)\n";
    cout << "3. Enter new data for a room (input)\n";
    cout << "4. Save all rooms to file\n";
    cout << "5. Find free rooms for period\n";
    cout << "6. Add new room\n";
    cout << "7. Find rooms for N persons in a period\n";
    cout << "0. Exit\n";
    cout << "Choice: ";
}

// Безпечне читання int: при помилці вводу повертає -1
int readInt() {
    int value;
    if (!(cin >> value)) {
        cin.clear();
        cin.ignore(10000, '\n');
        return -1;
    }
    return value;
}

int chooseRoomIndex(int size) {
    cout << "Choose a room (1-" << size << "): ";
    int idx = readInt();
    if (idx < 1 || idx > size) {
        cout << "Invalid room number.\n";
        return -1;
    }
    return idx - 1;
}

void saveRooms(HotelRoom *rooms, int size, const char *filename) {
    ofstream fout(filename);
    if (!fout.is_open()) {
        cout << "Could not open output file.\n";
        return;
    }
    for (int i = 0; i < size; i++) {
        rooms[i].toFlow(fout);
    }
    fout.close();
    cout << "Saved to " << filename << "\n";
}

// Створює більший масив, копіює старі номери, додає новий, старий масив видаляє
HotelRoom *addRoom(HotelRoom *rooms, int &size) {
    HotelRoom *newRooms = new HotelRoom[size + 1];
    for (int i = 0; i < size; i++) {
        newRooms[i] = rooms[i]; // operator= робить глибоку копію
    }
    newRooms[size].input();
    delete[] rooms;
    size++;
    return newRooms;
}

void roomSubmenu(HotelRoom &r) {
    int subChoice = -1;
    do {
        cout << "\n--- Room menu ---\n";
        cout << "1. Print room\n";
        cout << "2. Check availability for a period\n";
        cout << "3. Calculate price for a period\n";
        cout << "4. Show fields (get methods)\n";
        cout << "5. Change price (set method)\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        subChoice = readInt();

        switch (subChoice) {
            case 1:
                r.print();
                break;
            case 2: {
                cout << "Enter period to check:\n";
                period p = inputPeriod();
                if (r.available(p)) {
                    cout << "Room is available for this period.\n";
                } else {
                    cout << "Room is NOT available for this period.\n";
                }
                break;
            }
            case 3: {
                cout << "Enter period to calculate price:\n";
                period p = inputPeriod();
                cout << "Price: " << r.periodPrice(p) << "\n";
                break;
            }
            case 4:
                cout << "roomAmount: " << r.get_roomAmount() << "\n";
                cout << "area: " << r.get_area() << "\n";
                cout << "number: " << r.get_number() << "\n";
                cout << "occupancy: " << r.get_occupancy() << "\n";
                cout << "price: " << r.get_price() << "\n";
                break;
            case 5: {
                cout << "Enter new price: ";
                int newPrice = readInt();
                r.set_price(newPrice);
                cout << "Price updated.\n";
                break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (subChoice != 0);
}

// Демонстрація всіх перевантажених операторів. Пише в будь-який потік (консоль або файл).
void demo(ostream &out, HotelRoom *rooms, int n) {
    out << "=== operator<< (вивід усіх номерів) ===\n";
    for (int i = 0; i < n; i++) out << rooms[i] << "\n";

    out << "=== operator= (копіювання rooms[0]) ===\n";
    HotelRoom copyRoom;
    copyRoom = rooms[0];
    out << copyRoom << "\n";

    out << "=== operator> < == != (порівняння за вартістю) ===\n";
    out << "r0 > r1:  " << (rooms[0] > rooms[1]) << "\n";
    out << "r0 < r1:  " << (rooms[0] < rooms[1]) << "\n";
    out << "r0 == r1: " << (rooms[0] == rooms[1]) << "\n";
    out << "r0 != r1: " << (rooms[0] != rooms[1]) << "\n\n";

    out << "=== operator string (приведення до рядка) ===\n";
    string s = rooms[0];
    out << s << "\n";

    out << "=== operator++ (ціна +1000) ===\n";
    HotelRoom t = rooms[0];
    out << "Початкова ціна: " << t.get_price() << "\n";
    out << "Префіксний ++t:  " << (++t).get_price() << " (повернуто вже змінений)\n";
    out << "Постфіксний t++: " << (t++).get_price() << " (повернуто старе), тепер " << t.get_price() << "\n\n";

    out << "=== Сортування за зростанням ціни (через operator>) ===\n";
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (rooms[j] > rooms[j + 1]) {
                HotelRoom tmp = rooms[j];
                rooms[j] = rooms[j + 1];
                rooms[j + 1] = tmp;
            }
    for (int i = 0; i < n; i++) out << rooms[i].get_price() << " ";
    out << "\n\n";

    out << "=== operator+ (сума всіх номерів: кімнати, площа, місткість, ціна, бронювання) ===\n";
    HotelRoom sum;
    for (int i = 0; i < n; i++) sum = sum + rooms[i];
    out << sum << "\n";

    out << "=== operator++ для всіх елементів масиву ===\n";
    for (int i = 0; i < n; i++) ++rooms[i];
    for (int i = 0; i < n; i++) out << rooms[i].get_price() << " ";
    out << "\n\n";
}

// Повертає новий масив номерів, у яких price у діапазоні [minPrice, maxPrice]
HotelRoom *filterByPrice(const HotelRoom *arr, int n, int minPrice, int maxPrice, int &outCount) {
    HotelRoom lo, hi;
    lo.set_price(minPrice);
    hi.set_price(maxPrice);

    outCount = 0;
    for (int i = 0; i < n; i++)
        if (!(arr[i] < lo) && !(arr[i] > hi)) outCount++;

    HotelRoom *res = new HotelRoom[outCount];
    int k = 0;
    for (int i = 0; i < n; i++)
        if (!(arr[i] < lo) && !(arr[i] > hi)) res[k++] = arr[i];
    return res;
}

// Пошук номерів для заданої кількості осіб, вільних у заданий період
void findForPersons(HotelRoom *rooms, int n, int persons, period p) {
    bool found = false;
    for (int i = 0; i < n; i++) {
        if (rooms[i].get_occupancy() >= persons && rooms[i].available(p)) {
            cout << "Room " << i + 1 << " (number " << rooms[i].get_number()
                 << ", occupancy " << rooms[i].get_occupancy()
                 << ") - price for period: " << rooms[i].periodPrice(p) << "\n";
            found = true;
        }
    }
    if (!found) cout << "No suitable rooms found.\n";
}