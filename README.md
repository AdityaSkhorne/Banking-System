# Banking-System
SQL Banking System using Oracle 18c Express Edition.

This project implements a banking system using Oracle 18c Express Edition. It includes SQL queries to create and manage the necessary tables, relationships, and operations within a banking database. The system is designed based on the ER diagram shown below.

# Problem Statement:
In modern banking, managing customer accounts, loans, and payments across various branches is a complex task that involves multiple relationships and entities. The goal of this project is to develop a comprehensive database system that efficiently manages the core functionalities of a banking system, such as:

Customer Management: Storing and managing customer information and their relationships with bank accounts.
Account Management: Tracking different types of bank accounts, including savings and checking accounts, along with their balances.
Loan Management: Handling loans issued to customers, their payment schedules, and tracking each loan’s progress.
Employee Management: Keeping track of employees working in different branches, their roles, and their relationship with customers.
Branch Operations: Managing various branches, their assets, and the accounts and loans associated with them.
The system ensures that all transactions, deposits, withdrawals, and loan payments are accurately tracked and that each entity's relationship with others is properly maintained.

**ER Diagram**
The following ER diagram represents the structure of the banking system:


Key Entities and Attributes
Customer:
Customer_Id
Customer_Name
Customer_Street
Customer_City
Branch:
Branch_Name
Branch_City
Assets
Employee:
Employee_Id
Employee_Name
Telephone_No
Start_Date
Employment_Length
Dependent_Name
Account:
Account_Number
Balance
Loan:
Loan_Number
Amount
Payment:
Payment_No
Payment_Amount
Payment_Date
ISA (Individual Savings Account):
Interest_Rate (for Savings)
Overdraft_Amount (for Checking)
Relationships
Customer deposits into an Account and applies for a Loan.
Employee works at a Branch and manages customer accounts or loans.
Branch manages customer accounts and loans.
Loan has multiple Payments associated with it.
Project Contents
The repository contains the following files:

SQL Queries: The repository contains SQL scripts for creating tables and defining relationships.
tables.sql: Creates the tables for the banking system.
relationships.sql: Defines relationships and constraints.
data_insertion.sql: Inserts sample data into the database.
ER Diagram: The ER diagram (Blank board.png) is included in the repository to visualize the banking system.
Prerequisites
To run this project, you will need:

Oracle 18c Express Edition: The project uses Oracle SQL commands and runs on Oracle 18c Express Edition.
SQL Plus: Used to run SQL scripts and manage the database.
Setup Instructions
Install Oracle 18c Express Edition: Follow the Oracle 18c installation guide.
Run SQL Plus: Open SQL Plus and connect to your database instance.
Create Tables: Run the tables.sql script to create all required tables.
Define Relationships: Execute the relationships.sql script to define the relationships between entities.
Insert Sample Data: Use the data_insertion.sql script to populate the database with sample data.
Future Enhancements
Security Features: Implement encryption for sensitive customer data.
Transaction Management: Add support for real-time transactions between accounts.
Reporting Features: Generate monthly reports for customers detailing account balances and loan statuses.
