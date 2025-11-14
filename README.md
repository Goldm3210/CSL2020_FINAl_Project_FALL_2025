# CSL2020_FINAl_Project_FALL_2025
Hospital Management System (DSA Project)

This project is a simplified Hospital Management System built to demonstrate core data structures in a realistic scenario. It supports patient admission, triage, room allocation, doctor assignment, discharge, and instant lookup.

**Key Features :**

1. Admit new patients with automatic room, procedure, and doctor assignment

2. Priority-based ordering of patients (Critical → Urgent → Non-Urgent)

3. Search patients by ID in constant time

4. Proper room allocation (ICU, Ward, OPD)

5. Discharge management with cleanup


**Data Structures Used :**

1. Linked List: Stores patients in triage order

2. Hash Map (unordered_map): O(1) patient lookup

3. Priority Queue: Emergency-level queue

4. Min-Heap: Room allocation system

5. Vector: Doctor database


**How to Compile :**

g++ DSA_Project.cpp -o DSA_Project

./DSA_Project


**Purpose :**

The goal of this project is to design a data-structure–driven system that models how a modern hospital should manage patients under varying levels of urgency. Instead of relying on static or manual processes, this system demonstrates how linked lists, heaps, priority queues, and hashing can work together to support real-time decision making. By automating triage, room selection, doctor assignment, and patient lookup, the project shows how computational structures can be used to reduce delays, improve resource utilization, and create a more responsive hospital workflow.
