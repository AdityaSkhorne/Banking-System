#ifndef BANK_DATABASE_H
#define BANK_DATABASE_H

#include <iostream>
#include <string>
#include <occi.h>

using namespace oracle::occi;
using namespace std;

// --- Database Connection Details ---
// !!! REPLACE WITH YOUR ACTUAL CREDENTIALS !!!
const string DB_USER = "System";
const string DB_PASS = "XYZ";
const string DB_CONN_STR = "//localhost:1521/CDB$ROOT";

// Function to establish database connection
Connection* connectToDatabase(Environment* env);

// Function to clear input buffer
void clearInputBuffer();

// Function to get string input safely
string getStringInput(const string& prompt);

// Function to get double input safely
double getDoubleInput(const string& prompt);

// clearInputBuffer
void clearInputBuffer();


#endif // BANK_DATABASE_H