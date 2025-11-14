# CSL2020_FINAl_Project_FALL_2025
Hospital Management System (DSA Project)

This project is a simplified Hospital Management System built to demonstrate core data structures in a realistic scenario. It supports patient admission, triage, room allocation, doctor assignment, discharge, and instant lookup.

Key Features

Admit new patients with automatic room, procedure, and doctor assignment

Priority-based ordering of patients (Critical → Urgent → Non-Urgent)

Search patients by ID in constant time

Proper room allocation (ICU, Ward, OPD)

Discharge management with cleanup

Data Structures Used

Linked List: Stores patients in triage order

Hash Map (unordered_map): O(1) patient lookup

Priority Queue: Emergency-level queue

Min-Heap: Room allocation system

Vector: Doctor database

How to Compile
g++ DSA_Project.cpp -o DSA_Project
./DSA_Project

Purpose

To demonstrate how multiple data structures can work together to model a real system involving priority, lookup efficiency, and dynamic resource allocation.
