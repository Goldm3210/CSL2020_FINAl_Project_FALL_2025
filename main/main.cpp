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
