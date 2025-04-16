# Banking System

## Project Overview
This project implements a **Banking System** using **Oracle 18c Express Edition**. It is designed to manage core banking functionalities such as customer management, account management, loan management, employee management, and branch operations. The project includes SQL scripts to create and manage the required tables and relationships, and a C++ application using Oracle OCCI (Oracle C++ Call Interface) for database connectivity.

## What is This Project?
- **Purpose:**  
  The project is intended to simulate and manage basic banking operations such as handling customer accounts, tracking loans and payments, and managing branch and employee data.
  
- **Problem Statement:**  
  In modern banking, processing customer transactions, managing loans and payments, and maintaining up-to-date information across multiple branches is complex. The goal here is to create a robust and scalable database system that efficiently handles these core functions.

## How I Created This Project
- **Design and Development:**  
  I designed the database based on an Entity-Relationship (ER) diagram that covers key entities like Customer, Branch, Employee, Account, Loan, Payment, and specific account types (e.g., individual savings with overdraft limits).  
  The project was developed with Oracle 18c Express Edition, leveraging SQL scripts for table creation, establishing relationships, and inserting sample data. The application part is built in C++ using Oracle OCCI for database connections.

![366165671-5029a356-643a-4b8c-bfbd-ee9e3193f55b](https://github.com/user-attachments/assets/f9be2695-d231-4802-8b4d-828a395ce5ac)


- **Tools Used:**  
  - Oracle 18c Express Edition (Database)
  - SQL*Plus (for running SQL scripts)
  - Visual Studio 2022 (C++ development)
  - Oracle Instant Client SDK (for Oracle OCCI)

## Files Used in the Project
- **SQL Scripts:**
  - `tables.sql` – Creates the necessary tables for the banking system.
  - `relationships.sql` – Defines the relationships and constraints among the tables.
  - `data_insertion.sql` – Inserts sample data into the database.
  
- **Source Code Files:**
  - `main.cpp` – Contains the main application logic.
  - `database.cpp`, `database.h` – Handles the database connection and queries.
  - `customer.cpp`, `customer.h` – Manages customer-related operations.
  - `account.cpp`, `account.h` – Manages account-related operations.
  - `loan.cpp`, `loan.h` – Manages loan-related operations.
  - `branch.cpp`, `branch.h` – Handles branch operations.
  - `employee.cpp`, `employee.h` – Manages employee information.
  
- **Other Files:**
  - `ER_diagram.png` – ER diagram that visualizes the structure of the banking system.
  - `.gitignore` – Specifies intentionally untracked files to ignore.
  - `LICENSE` – MIT License for the project.

## System Requirements and Prerequisites
- **Database:**  
  Oracle 18c Express Edition is required to run the SQL scripts and serve as the backend for the application.

- **Development Environment:**
  - **Operating System:** Windows
  - **Compiler:** Visual Studio 2022 (x64)
  - **Oracle Files:** Oracle Instant Client SDK (includes include files and libraries such as `oraocci19.lib` and `oci.lib`)


## Setup and Running Instructions

### Preparing the Environment
1. **Install Oracle 18c Express Edition:**  
   Follow the Oracle 18c installation guide for your OS.
   
2. **Set Up SQL*Plus:**  
   Open SQL*Plus and connect to your database instance to run the provided SQL scripts:
   - Create tables: Run `tables.sql`.
   - Define relationships: Run `relationships.sql`.
   - Insert sample data: Run `data_insertion.sql`.

### Using the x64 Native Tools Command Prompt
Before compiling the project, open the appropriate Visual Studio command prompt:

```cmd
**********************************************************************
** Visual Studio 2022 Developer Command Prompt v17.14.0-pre.2.0
** Copyright (c) 2022 Microsoft Corporation
**********************************************************************
[vcvarsall.bat] Environment initialized for: 'x64'
Alternatively, you may use:

cmd
Visual Studio 2022 Developer Command Prompt v17.9.0-pre.3.0
           Copyright (c) 2022 Microsoft Corporation
To confirm that the environment is working, type:

cmd
cl
You should see an output like: 
Microsoft (R) C/C++ Optimizing Compiler Version 19.44.34918.1 for x64

...
Building and Running the Project

Change Directory:
cd /d "D:\my projects\banking\Banking-System"

List Files (Optional):
dir
This confirms all necessary project files are in the directory.


Compile the Project:
Use the following cl command to compile the project:

cmd
cl /EHsc main.cpp database.cpp customer.cpp account.cpp loan.cpp branch.cpp employee.cpp ^
/I "D:\Users\Aditya\Downloads\instantclient-sdk-windows.x64-19.26.0.0.0dbru\instantclient_19_26\sdk\include" ^
/link /LIBPATH:"D:\Users\Aditya\Downloads\instantclient-sdk-windows.x64-19.26.0.0.0dbru\instantclient_19_26\sdk\lib\msvc" oraocci19.lib oci.lib

Explanation:
/EHsc: Enables standard exception handling.
/I "path\to\includes": Specifies the include directory of the Oracle Instant Client SDK.
/link: Begins the linker options.
/LIBPATH:"path\to\lib": Specifies the library path for Oracle libraries.
oraocci19.lib oci.lib: Links against the Oracle OCCI and OCI libraries.

Run the Application:
After a successful build, an executable (e.g., main.exe) will be generated. Run it:
main.exe
(Output screenshots can be added here to show the runtime result.)

![Screenshot 2025-04-15 160844](https://github.com/user-attachments/assets/9dcbe43d-857d-4668-a0d4-253fee9159c2)

![Screenshot 2025-04-15 161037](https://github.com/user-attachments/assets/8f1ee7f3-f7a8-4526-afdd-ba82848faf79)

![Screenshot 2025-04-15 161146](https://github.com/user-attachments/assets/ab378fdd-87d1-424c-84bf-67de4ad868af)

![Screenshot 2025-04-15 161155](https://github.com/user-attachments/assets/7b2776d9-93b8-421a-b3f5-03f9f2ada3f1)

![Screenshot 2025-04-15 161249](https://github.com/user-attachments/assets/7da78427-cf4b-4dd6-a277-a534b83e11d8)




Known Limitations and Future Enhancements
Note:
Due to some challenges and limited experience with Oracle DB and its related files/libraries, this project is currently incomplete. Some features may not work as expected, and further enhancements are planned, including:
Security Features: Adding encryption for sensitive customer data.
Transaction Management: Supporting real-time transactions between accounts.
Reporting: Generating comprehensive monthly reports for customers on account balances and loan statuses.

Final Thoughts
This project is a work in progress intended to provide a foundation for a comprehensive banking system. Contributions, feedback, and suggestions are welcome!

Feel free to modify or expand this README as your project evolves. Happy coding!
You can adjust any sections and details as needed. This README provides an overview, instructions, and notes that cover the key points of your project.







