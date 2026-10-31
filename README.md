# Hotel Management System (Mini Project) - C++ OOP

**Name:** Kairava Rora
**UID:** 25BCD10071
**Section:** 25BCD-2-A
**Subject:** Object Oriented Programming (C++)
**Submitted to:** Jitendra Kumar, Assistant Professor

## About the project
A console-based Hotel Management System written in C++ using Object Oriented Programming.
It manages rooms (Standard, Deluxe, Suite), guests, bookings, check-out and a bill with 12% GST.

## Features
- Add rooms of different types
- Register guests
- Book a room for a number of nights (double booking is prevented)
- Check out and print a bill with GST
- Show all rooms and all bookings
- Error handling using exceptions

## OOP concepts used
- **Encapsulation:** private/protected data with public member functions
- **Abstraction:** abstract base class `Room` with pure virtual functions
- **Inheritance:** `StandardRoom`, `DeluxeRoom`, `SuiteRoom` derive from `Room`
- **Polymorphism:** `ratePerNight()` and `type()` are overridden (virtual functions)
- **Exception handling:** `runtime_error` and `invalid_argument`
- **STL and smart pointers:** `vector` and `unique_ptr`

## Room rates
| Room type | Rate per night |
|---|---|
| Standard | Rs. 2000 |
| Deluxe | Rs. 3500 |
| Suite | Rs. 6000 |

## How to compile and run
```
g++ -std=c++14 hotel.cpp -o hotel
./hotel
```
On Windows (MinGW): `g++ -std=c++14 hotel.cpp -o hotel.exe` then run `hotel.exe`

## Sample output
See `sample_output.txt`.

## Files
- `hotel.cpp` - complete source code
- `README.md` - this file
- `sample_output.txt` - sample run
- `Mini_Project_Report_25BCD10071.pdf` - project report (add your PDF here)
