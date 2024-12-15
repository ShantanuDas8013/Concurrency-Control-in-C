# Concurrency-Control-in-C
This project implements a basic Concurrency Control System using the C programming language. It simulates transaction management, resource allocation, and operations in a database-like environment. The goal is to demonstrate how multiple transactions can safely interact with shared resources while ensuring consistency, preventing conflicts, and adhering to isolation principles.

# Features:
Resource Initialization: Reads resources and metadata from a file (resource.txt) and initializes the system.
Transaction Management: Allows creating, managing, and committing transactions with unique IDs and timestamps.
Operations:
Read: Reads data from shared resources, with optional locking for transaction safety.
Arithmetic Operations: Performs addition or subtraction on transaction-held values.
Write: Updates shared resources after ensuring proper locking.
Commit: Safely writes resource states to the file, marking the transaction as complete.
Conflict Handling: Detects and resolves conflicts when multiple transactions attempt to access or modify the same resource.
Transaction Table: Logs transaction activities, including timestamps, operations performed, and resource values held.
Menu-Driven Interface: Interactive menu for creating transactions and performing various operations.
# Usage:
The program includes a user-friendly menu to:
Create new transactions.
Perform read, arithmetic, and write operations.
Commit changes to shared resources.
Display the transaction table and resource status.
# Learning Objectives:
Understand how transactions operate in a concurrent system.
Explore locking mechanisms to manage resource conflicts.
Gain insight into basic ACID properties (Atomicity, Consistency, Isolation, Durability) for database transactions.
Learn C programming techniques for data structure management and file handling.

# Summary
This project provides a foundational understanding of concurrency control, making it ideal for students and developers looking to grasp the basics of database systems or concurrent programming.
