#include "database.h"
#include <limits>
#include <iostream>
using namespace std;

// Function to establish database connection
Connection* connectToDatabase(Environment* env) {
    Connection* conn = nullptr;
    try {
        cout << "Connecting to database..." << endl;
        conn = env->createConnection("system", "AdityaSK", "LAPTOP-DDAOM75B:1521/xepdb1");
        cout << "Database connection successful!" << endl;
    } catch (SQLException& ex) {
        cerr << "Database Connection Error: " << ex.getMessage() << endl;
        throw; // Re-throw the exception to be caught in main()
    }
    return conn;
}

// Helper function to clear input buffer
void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Helper function to get string input safely
string getStringInput(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

// Helper function to get double input safely
double getDoubleInput(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.good()) {
            clearInputBuffer();
            return value;
        } else {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            clearInputBuffer();
        }
    }
}