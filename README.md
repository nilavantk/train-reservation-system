# 🚆 Train Reservation System in C

### 📘 Project Overview
The **Train Reservation System** is a console-based C program designed to simplify train ticket booking and management. It replaces the traditional manual reservation process with an automated, transparent, and efficient digital system. The project focuses on **seat allocation**, **waiting list management**, and **shortest route assistance** between stations.

---

### 🎯 Features
- **Seat Booking:** Allows passengers to reserve seats dynamically until capacity is full.  
- **Waiting List Management:** Automatically queues passengers when seats are unavailable.  
- **Cancellation & Reallocation:** Reassigns cancelled seats to waiting passengers.  
- **Passenger Data Handling:** Stores details like ID, name, source, and destination.  
- **Shortest Route Suggestion:** Finds the minimal travel route between two stations.  
- **User-Friendly Menu:** Interactive console interface for smooth navigation.

---

### 🧠 Concepts Used
- Structures (`struct`)
- Arrays and Queues
- String handling (`<string.h>`)
- File handling (for saving passenger data)
- Functions and modular programming
- Graph traversal algorithms (for route finding)

---

### ⚙️ Compilation & Execution
#### Using GCC or Dev-C++:
```bash
gcc train_reservation.c -o train_reservation
./train_reservation
