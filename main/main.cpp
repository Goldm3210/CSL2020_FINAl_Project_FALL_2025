#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <ctime>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <time.h>
using namespace std;

// Doctor database
struct Doctor {
    int id;
    string name;
    string specialty;
    int experience; // years
    bool available;
    
    Doctor(int _id, string _name, string _spec, int _exp) 
        : id(_id), name(_name), specialty(_spec), experience(_exp), available(true) {}
};

// Patient node (for linked list)
struct Patient {
    int id;
    string name;
    int age;
    string condition;
    int emergencyLevel; // 1=Critical, 2=Urgent, 3=Non-Urgent
    string room;
    string procedure;
    int timeRequired; // minutes
    string doctorName;
    string admissionTime;
    Patient* next;

    Patient(int _id, string _name, int _age, string _cond, int _level, 
            string _room, string _proc, int _time, string _doc, string _adm)
        : id(_id), name(_name), age(_age), condition(_cond), emergencyLevel(_level),
          room(_room), procedure(_proc), timeRequired(_time), doctorName(_doc), 
          admissionTime(_adm), next(nullptr) {}
};

// For priority queue (min-heap by emergency level, then admission time)
struct PriorityQueueNode {
    int id;
    int emergencyLevel;
    time_t admissionTime;
    
    PriorityQueueNode(int _id, int _level, time_t _time) 
        : id(_id), emergencyLevel(_level), admissionTime(_time) {}
    
    // Min-heap: lower emergency level (1=Critical) has higher priority
    bool operator>(const PriorityQueueNode& other) const {
        if (emergencyLevel != other.emergencyLevel) {
            return emergencyLevel > other.emergencyLevel;
        }
        return admissionTime > other.admissionTime;
    }
};

// For room allocation heap
struct RoomNode {
    string name;
    int priority; // 1=ICU, 2=Ward, 3=OPD
    bool occupied;
    
    RoomNode(string n, int p) : name(n), priority(p), occupied(false) {}
    
    bool operator>(const RoomNode& other) const {
        return priority > other.priority;
    }
};

class Hospital {
private:
    Patient* head;  // Linked list for sequential access
    
    // HASHING: O(1) patient lookup by ID
    unordered_map<int, Patient*> patientMap;
    
    // PRIORITY QUEUE: O(log n) emergency-based processing
    priority_queue<PriorityQueueNode, vector<PriorityQueueNode>, greater<PriorityQueueNode>> emergencyQueue;
    
    // MIN-HEAP for room allocation (prioritize ICU > Ward > OPD)
    priority_queue<RoomNode, vector<RoomNode>, greater<RoomNode>> roomHeap;
    
    int nextId;
    vector<Doctor> doctors;

    void initDoctors() {
        doctors = {
            Doctor(101, "Dr. Smith", "Cardiology", 15),
            Doctor(102, "Dr. Johnson", "Neurosurgery", 12),
            Doctor(103, "Dr. Williams", "Orthopedics", 8),
            Doctor(104, "Dr. Brown", "General Surgery", 10),
            Doctor(105, "Dr. Davis", "Internal Medicine", 7),
            Doctor(106, "Dr. Miller", "Pediatrics", 9)
        };
    }

    // Initialize room heap with correct counts: 3 ICU, 5 Ward, 10 OPD
    void initRooms() {
        // ICU rooms (priority 1) - 3 rooms
        roomHeap.push(RoomNode("ICU-01", 1));
        roomHeap.push(RoomNode("ICU-02", 1));
        roomHeap.push(RoomNode("ICU-03", 1));
        
        // Ward rooms (priority 2) - 5 rooms
        roomHeap.push(RoomNode("WARD-A1", 2));
        roomHeap.push(RoomNode("WARD-A2", 2));
        roomHeap.push(RoomNode("WARD-B1", 2));
        roomHeap.push(RoomNode("WARD-B2", 2));
        roomHeap.push(RoomNode("WARD-C1", 2));
        
        // OPD rooms (priority 3) - 10 rooms
        roomHeap.push(RoomNode("OPD-1", 3));
        roomHeap.push(RoomNode("OPD-2", 3));
        roomHeap.push(RoomNode("OPD-3", 3));
        roomHeap.push(RoomNode("OPD-4", 3));
        roomHeap.push(RoomNode("OPD-5", 3));
        roomHeap.push(RoomNode("OPD-6", 3));
        roomHeap.push(RoomNode("OPD-7", 3));
        roomHeap.push(RoomNode("OPD-8", 3));
        roomHeap.push(RoomNode("OPD-9", 3));
        roomHeap.push(RoomNode("OPD-10", 3));
    }

    string getCurrentTime() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        ostringstream oss;
        oss << 1900 + ltm->tm_year << "-"
            << setw(2) << setfill('0') << 1 + ltm->tm_mon << "-"
            << setw(2) << setfill('0') << ltm->tm_mday << " "
            << setw(2) << setfill('0') << ltm->tm_hour << ":"
            << setw(2) << setfill('0') << ltm->tm_min;
        return oss.str();
    }

    // CORRECTED: Strict room assignment rules
string assignRoom(int level) {
        // Use MIN-HEAP for optimal room allocation
        vector<RoomNode> tempRooms;
        string assignedRoom = "GEN-101";
        
        while (!roomHeap.empty()) {
            RoomNode room = roomHeap.top();
            roomHeap.pop();
            
            // For Critical patients (level 1): ONLY ICU rooms (priority 1)
            if (level == 1 && room.priority == 1 && !room.occupied) {
                room.occupied = true;
                assignedRoom = room.name + " (Intensive Care)";
                roomHeap.push(room);
                break;
            }
            // For Urgent patients (level 2): Ward rooms (priority 2) first, then ICU if needed
            else if (level == 2 && !room.occupied && 
                    (room.priority == 2 || (room.priority == 1 && noAvailableWardRooms()))) {
                room.occupied = true;
                assignedRoom = room.name + (room.priority == 1 ? " (Intensive Care)" : " (High Dependency)");
                roomHeap.push(room);
                break;
            }
            // For Non-Urgent patients (level 3): ONLY OPD rooms (priority 3)
            else if (level == 3 && room.priority == 3 && !room.occupied) {
                room.occupied = true;
                assignedRoom = room.name + " (Outpatient)";
                roomHeap.push(room);
                break;
            }
            
            // Save unassigned rooms to restore later
            tempRooms.push_back(room);
        }
        
        // Push back all unassigned rooms
        for (auto& r : tempRooms) {
            roomHeap.push(r);
        }
        
        return assignedRoom;
    }

    // Helper function to check if any Ward rooms are available
    bool noAvailableWardRooms() {
        // Create a temporary copy of the heap
        auto tempHeap = roomHeap;
        while (!tempHeap.empty()) {
            RoomNode room = tempHeap.top();
            tempHeap.pop();
            if (room.priority == 2 && !room.occupied) {
                return false; // Found available Ward room
            }
        }
        return true; // No available Ward rooms
    }

    pair<string, int> assignProcedure(const string& condition) {
        string lowerCond = condition;
        transform(lowerCond.begin(), lowerCond.end(), lowerCond.begin(), ::tolower);
        
        if (lowerCond.find("heart") != string::npos || 
            lowerCond.find("chest") != string::npos) {
            return {"Emergency Surgery", 360}; // 6 hours
        }
        if (lowerCond.find("fracture") != string::npos || 
            lowerCond.find("bone") != string::npos) {
            return {"Orthopedic Operation", 180}; // 3 hours
        }
        if (lowerCond.find("fever") != string::npos || 
            lowerCond.find("cold") != string::npos) {
            return {"Routine Checkup", 30};
        }
        if (lowerCond.find("head") != string::npos || 
            lowerCond.find("brain") != string::npos) {
            return {"Neurosurgery", 480}; // 8 hours
        }
        return {"General Checkup", 45};
    }

    string assignDoctor(const string& procedure) {
        string specialty = "General Medicine";
        
        if (procedure.find("Surgery") != string::npos) specialty = "General Surgery";
        if (procedure.find("Neurosurgery") != string::npos) specialty = "Neurosurgery";
        if (procedure.find("Orthopedic") != string::npos) specialty = "Orthopedics";
        if (procedure.find("Cardio") != string::npos || procedure.find("heart") != string::npos) 
            specialty = "Cardiology";

        Doctor* bestDoc = nullptr;
        for (auto& doc : doctors) {
            if (doc.specialty == specialty && doc.available) {
                if (!bestDoc || doc.experience > bestDoc->experience) {
                    bestDoc = &doc;
                }
            }
        }

        if (!bestDoc) {
            for (int exp = 15; exp >= 5; exp--) {
                for (auto& doc : doctors) {
                    if (doc.experience >= exp && doc.available) {
                        bestDoc = &doc;
                        break;
                    }
                }
                if (bestDoc) break;
            }
        }

        if (bestDoc) {
            bestDoc->available = false;
            return bestDoc->name + " (Exp: " + to_string(bestDoc->experience) + " yrs)";
        }
        return "Dr. On-Call (General)";
    }

public:
    Hospital() : head(nullptr), nextId(1001) {
        initDoctors();
        initRooms();
        cout << "WELCOME TO DYNAMIC HOSPITAL MANAGEMENT SYSTEM" << endl;
        cout << "Using Priority-based Linked List (Emergency Triage)" << endl << endl;
    }

    ~Hospital() {
        while (head) {
            Patient* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void admitPatient() {
        cout << "\n[Admit New Patient]\n";
        
        string name, condition;
        int age, level;
        
        cout << "Name: ";
        cin.ignore();
        getline(cin, name);
        
        cout << "Age: ";
        cin >> age;
        
        cout << "Condition (e.g., Chest Pain, Fracture): ";
        cin.ignore();
        getline(cin, condition);
        
        cout << "Enter emergency level (1-Critical, 2-Urgent, 3-Non-Urgent): ";
        cin >> level;
        
        if (level < 1 || level > 3) {
            cout << "Invalid emergency level! Using Non-Urgent (3)." << endl;
            level = 3;
        }

        string room = assignRoom(level);
        auto procTime = assignProcedure(condition);
        string procedure = procTime.first;
        int timeRequired = procTime.second;
        string doctor = assignDoctor(procedure);
        string admissionTime = getCurrentTime();

        Patient* newPatient = new Patient(
            nextId, name, age, condition, level, 
            room, procedure, timeRequired, doctor, admissionTime
        );

        // LINKED LIST insertion (priority-based)
        if (!head || newPatient->emergencyLevel < head->emergencyLevel) {
            newPatient->next = head;
            head = newPatient;
        } else {
            Patient* curr = head;
            while (curr->next && curr->next->emergencyLevel <= newPatient->emergencyLevel) {
                curr = curr->next;
            }
            newPatient->next = curr->next;
            curr->next = newPatient;
        }

        // HASHING: Store in O(1) lookup map
        patientMap[nextId] = newPatient;
        
        // PRIORITY QUEUE: Add for emergency processing
        time_t admitTime = time(0);
        emergencyQueue.push(PriorityQueueNode(nextId, level, admitTime));

        cout << "Patient admitted with ID: " << newPatient->id << endl;
        cout << "   Priority: " << getPriorityLabel(newPatient->emergencyLevel) << endl;
        cout << "   Room: " << newPatient->room << endl;
        cout << "   Procedure: " << newPatient->procedure << " (" << newPatient->timeRequired << " mins)" << endl;
        cout << "   Doctor: " << newPatient->doctorName << endl;
        cout << "   Admission Time: " << newPatient->admissionTime << endl;
        
        nextId++;
    }

    string getPriorityLabel(int level) {
        switch (level) {
            case 1: return "CRITICAL (Immediate care)";
            case 2: return "URGENT (Within 1 hour)";
            case 3: return "NON-URGENT (Routine)";
            default: return "Unknown";
        }
    }

    void displayPatients() {
        if (!head) {
            cout << "\nNo patients admitted." << endl;
            return;
        }
        
        cout << "\n=== CURRENT PATIENTS (by priority) ===\n";
        cout << left << setw(6) << "ID"
             << setw(12) << "Name"
             << setw(4) << "Age"
             << setw(15) << "Room"
             << setw(20) << "Procedure"
             << setw(12) << "Time(min)"
             << setw(25) << "Doctor"
             << setw(12) << "Priority" << endl;
        cout << string(105, '-') << endl;

        Patient* curr = head;
        while (curr) {
            cout << left 
                 << setw(6) << curr->id
                 << setw(12) << curr->name.substr(0, 11)
                 << setw(4) << curr->age
                 << setw(15) << (curr->room.length() > 14 ? curr->room.substr(0, 14) : curr->room)
                 << setw(20) << (curr->procedure.length() > 19 ? curr->procedure.substr(0, 19) : curr->procedure)
                 << setw(12) << curr->timeRequired
                 << setw(25) << (curr->doctorName.length() > 24 ? curr->doctorName.substr(0, 24) : curr->doctorName)
                 << setw(12) << (curr->emergencyLevel == 1 ? "CRITICAL" : 
                                curr->emergencyLevel == 2 ? "URGENT" : "NON-URGENT") << endl;
            curr = curr->next;
        }
        cout << "========================================" << endl;
    }

    void dischargePatient() {
        if (!head) {
            cout << "\nNo patients to discharge." << endl;
            return;
        }

        int id;
        cout << "\nEnter Patient ID to discharge: ";
        cin >> id;

        // HASHING: O(1) lookup
        if (patientMap.find(id) == patientMap.end()) {
            cout << "Patient with ID " << id << " not found." << endl;
            return;
        }

        Patient* patient = patientMap[id];
        
        // Free doctor
        string doctorName = patient->doctorName;
        size_t expPos = doctorName.find("(Exp:");
        if (expPos != string::npos) {
            string docName = doctorName.substr(0, expPos-1);
            for (auto& doc : doctors) {
                if (doc.name == docName) {
                    doc.available = true;
                }
            }
        }

        // Remove from linked list
        Patient* curr = head;
        Patient* prev = nullptr;
        while (curr && curr->id != id) {
            prev = curr;
            curr = curr->next;
        }
        if (curr) {
            if (prev == nullptr) {
                head = curr->next;
            } else {
                prev->next = curr->next;
            }
        }

        // Remove from hash map
        patientMap.erase(id);
        
        // Note: Priority queue uses lazy deletion 

        cout << "Discharged patient: " << patient->name 
             << " (ID: " << patient->id << ")" << endl;
        cout << "   Room " << patient->room.substr(0, patient->room.find(" ")) 
             << " is now available" << endl;
        
        delete patient;
    }

    void findPatient() {
        if (!head) {
            cout << "\nNo patients admitted." << endl;
            return;
        }

        int id;
        cout << "\nEnter Patient ID to search: ";
        cin >> id;

        // HASHING: O(1) lookup
        auto it = patientMap.find(id);
        if (it != patientMap.end()) {
            Patient* p = it->second;
            
            cout << "\nPATIENT DETAILS:" << endl;
            cout << "ID: " << p->id << endl;
            cout << "Name: " << p->name << endl;
            cout << "Age: " << p->age << endl;
            cout << "Condition: " << p->condition << endl;
            cout << "Emergency Level: " << getPriorityLabel(p->emergencyLevel) << endl;
            cout << "Room: " << p->room << endl;
            cout << "Procedure: " << p->procedure << " (" << p->timeRequired << " minutes)" << endl;
            cout << "Assigned Doctor: " << p->doctorName << endl;
            cout << "Admission Time: " << p->admissionTime << endl;
        } else {
            cout << "Patient with ID " << id << " not found." << endl;
        }
    }
};

// Main menu
int main() {
    Hospital hospital;
    int choice;

    do {
        cout << "\n--- MENU ---\n";
        cout << "1. Admit Patient\n";
        cout << "2. Display All Patients\n";
        cout << "3. Discharge Patient (by ID)\n";
        cout << "4. Find Patient (by ID)\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (choice) {
            case 1: hospital.admitPatient(); break;
            case 2: hospital.displayPatients(); break;
            case 3: hospital.dischargePatient(); break;
            case 4: hospital.findPatient(); break;
            case 0: 
                cout << "\nThank you for using Hospital Management System!" << endl;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 0);

    return 0;
}
