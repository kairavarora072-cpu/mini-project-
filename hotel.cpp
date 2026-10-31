#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
using namespace std;

// ---------- Abstract base class ----------
class Room {
protected:
    int roomNo;
    bool booked;
public:
    explicit Room(int n) : roomNo(n), booked(false) {}
    virtual ~Room() {}
    virtual string type() const = 0;
    virtual double ratePerNight() const = 0;
    int getRoomNo() const { return roomNo; }
    bool isBooked() const { return booked; }
    void setBooked(bool b) { booked = b; }
    void display() const {
        cout << left << setw(8) << roomNo << setw(12) << type()
             << setw(12) << static_cast<int>(ratePerNight())
             << (booked ? "Booked" : "Available") << "\n";
    }
};

class StandardRoom : public Room {
public:
    using Room::Room;
    string type() const override { return "Standard"; }
    double ratePerNight() const override { return 2000.0; }
};

class DeluxeRoom : public Room {
public:
    using Room::Room;
    string type() const override { return "Deluxe"; }
    double ratePerNight() const override { return 3500.0; }
};

class SuiteRoom : public Room {
public:
    using Room::Room;
    string type() const override { return "Suite"; }
    double ratePerNight() const override { return 6000.0; }
};

// ---------- Guest ----------
class Guest {
    int id;
    string name;
    string phone;
public:
    Guest(int i, string n, string p) : id(i), name(move(n)), phone(move(p)) {}
    int getId() const { return id; }
    string getName() const { return name; }
    string getPhone() const { return phone; }
};

// ---------- Booking ----------
class Booking {
    int id, guestId, roomNo, nights;
    double rate;
    bool active;
public:
    Booking(int i, int g, int r, int n, double rt)
        : id(i), guestId(g), roomNo(r), nights(n), rate(rt), active(true) {}
    int getId() const { return id; }
    int getGuestId() const { return guestId; }
    int getRoomNo() const { return roomNo; }
    int getNights() const { return nights; }
    double getRate() const { return rate; }
    bool isActive() const { return active; }
    double subtotal() const { return rate * nights; }
    void close() { active = false; }
};

// ---------- Hotel ----------
class Hotel {
    vector<unique_ptr<Room>> rooms;
    vector<Guest> guests;
    vector<Booking> bookings;
    int nextBookingId = 1;

    Room* findRoom(int no) {
        for (auto& r : rooms) if (r->getRoomNo() == no) return r.get();
        throw runtime_error("Room " + to_string(no) + " not found.");
    }
    Guest* findGuest(int id) {
        for (auto& g : guests) if (g.getId() == id) return &g;
        throw runtime_error("Guest " + to_string(id) + " not found.");
    }
    Booking* findBooking(int id) {
        for (auto& b : bookings) if (b.getId() == id) return &b;
        throw runtime_error("Booking " + to_string(id) + " not found.");
    }
public:
    void addRoom(unique_ptr<Room> r) {
        for (auto& x : rooms)
            if (x->getRoomNo() == r->getRoomNo())
                throw runtime_error("Room already exists.");
        rooms.push_back(move(r));
    }

    void addGuest(const Guest& g) {
        for (auto& x : guests)
            if (x.getId() == g.getId())
                throw runtime_error("Guest id already exists.");
        guests.push_back(g);
    }

    void showRooms() const {
        cout << left << setw(8) << "Room" << setw(12) << "Type"
             << setw(12) << "Rate (Rs.)" << "Status\n";
        for (const auto& r : rooms) r->display();
    }

    void showBookings() const {
        if (bookings.empty()) { cout << "No bookings yet.\n"; return; }
        for (const auto& b : bookings)
            cout << "Booking " << b.getId() << " | Guest " << b.getGuestId()
                 << " | Room " << b.getRoomNo() << " | " << b.getNights()
                 << " night(s) | " << (b.isActive() ? "Active" : "Closed") << "\n";
    }

    void bookRoom(int guestId, int roomNo, int nights) {
        if (nights <= 0)
            throw invalid_argument("Number of nights must be positive.");
        Guest* g = findGuest(guestId);
        Room* r = findRoom(roomNo);
        if (r->isBooked())
            throw runtime_error("Room " + to_string(roomNo) + " is already booked.");
        r->setBooked(true);
        int id = nextBookingId++;
        bookings.emplace_back(id, guestId, roomNo, nights, r->ratePerNight());
        cout << "Booking " << id << " confirmed: " << g->getName() << ", Room "
             << roomNo << " (" << r->type() << "), " << nights << " night(s).\n";
    }

    void checkout(int bookingId) {
        Booking* b = findBooking(bookingId);
        if (!b->isActive())
            throw runtime_error("Booking " + to_string(bookingId) + " is already closed.");
        Guest* g = findGuest(b->getGuestId());
        Room* r = findRoom(b->getRoomNo());
        const double GST = 0.12;
        double sub = b->subtotal();
        double tax = sub * GST;
        double total = sub + tax;

        cout << fixed << setprecision(2);
        cout << "\n------------ HOTEL BILL ------------\n";
        cout << "Booking id  : " << b->getId() << "\n";
        cout << "Guest       : " << g->getName() << "\n";
        cout << "Room        : " << r->getRoomNo() << " (" << r->type() << ")\n";
        cout << "Rate/night  : Rs. " << b->getRate() << "\n";
        cout << "Nights      : " << b->getNights() << "\n";
        cout << "Subtotal    : Rs. " << sub << "\n";
        cout << "GST (12%)   : Rs. " << tax << "\n";
        cout << "Total       : Rs. " << total << "\n";
        cout << "------------------------------------\n";
        cout.unsetf(ios::floatfield);

        r->setBooked(false);
        b->close();
    }
};

// ---------- Main ----------
int main() {
    Hotel hotel;
    hotel.addRoom(make_unique<StandardRoom>(101));
    hotel.addRoom(make_unique<StandardRoom>(102));
    hotel.addRoom(make_unique<DeluxeRoom>(201));
    hotel.addRoom(make_unique<SuiteRoom>(301));
    hotel.addGuest(Guest(1, "Kairava Rora", "9876543210"));

    int choice;
    while (true) {
        cout << "\n===== HOTEL MENU =====\n"
             << "1. Add Room\n2. Register Guest\n3. Show Rooms\n"
             << "4. Book Room\n5. Check-out and Bill\n6. Show Bookings\n0. Exit\n"
             << "Enter choice: ";
        if (!(cin >> choice) || choice == 0) break;
        try {
            if (choice == 1) {
                int no, t;
                cout << "Room no: "; cin >> no;
                cout << "Type (1-Standard 2-Deluxe 3-Suite): "; cin >> t;
                if (t == 1) hotel.addRoom(make_unique<StandardRoom>(no));
                else if (t == 2) hotel.addRoom(make_unique<DeluxeRoom>(no));
                else if (t == 3) hotel.addRoom(make_unique<SuiteRoom>(no));
                else throw invalid_argument("Invalid room type.");
            } else if (choice == 2) {
                int id; string n, p;
                cout << "Guest id: "; cin >> id; cin.ignore();
                cout << "Name: "; getline(cin, n);
                cout << "Phone: "; getline(cin, p);
                hotel.addGuest(Guest(id, n, p));
            } else if (choice == 3) {
                hotel.showRooms();
            } else if (choice == 4) {
                int g, r, n;
                cout << "Guest id: "; cin >> g;
                cout << "Room no: "; cin >> r;
                cout << "Nights: "; cin >> n;
                hotel.bookRoom(g, r, n);
            } else if (choice == 5) {
                int b;
                cout << "Booking id: "; cin >> b;
                hotel.checkout(b);
            } else if (choice == 6) {
                hotel.showBookings();
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
    cout << "Thank you for using the Hotel Management System.\n";
    return 0;
}
