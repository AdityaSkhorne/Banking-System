#include "branch.h"
#include <iomanip>

// Function to add a new branch
void addBranch(Connection* conn) {
    cout << "\n--- Add New Branch ---" << endl;
    string branchName = getStringInput("Enter Branch Name: ");
    string branchCity = getStringInput("Enter Branch City: ");
    double assets = getDoubleInput("Enter Branch Assets: ");

    Statement* stmt = nullptr;
    try {
        stmt = conn->createStatement(
            "INSERT INTO Branch (branch_name, branch_city, assets) "
            "VALUES (:1, :2, :3)"
        );
        stmt->setString(1, branchName);
        stmt->setString(2, branchCity);
        stmt->setDouble(3, assets);

        int rowsAffected = stmt->executeUpdate();

        if (rowsAffected > 0) {
            conn->commit();
            cout << "Branch '" << branchName << "' added successfully." << endl;
        } else {
            cout << "Failed to add branch." << endl;
            conn->rollback();
        }
        conn->terminateStatement(stmt);
    } catch (SQLException& ex) {
        cerr << "Error adding branch: " << ex.getMessage() << endl;
        if (stmt) conn->terminateStatement(stmt);
        conn->rollback();
        throw;
    }
}

// Function to view branch details
void viewBranchDetails(Connection* conn) {
    cout << "\n--- View Branch Details ---" << endl;
    string branchName = getStringInput("Enter Branch Name: ");

    Statement* stmt = nullptr;
    ResultSet* rs = nullptr;
    try {
        stmt = conn->createStatement(
            "SELECT branch_name, branch_city, assets FROM Branch WHERE branch_name = :1"
        );
        stmt->setString(1, branchName);
        rs = stmt->executeQuery();

        if (rs->next()) {
            cout << "\nBranch Information:" << endl;
            cout << "  Name:   " << rs->getString(1) << endl;
            cout << "  City:   " << rs->getString(2) << endl;
            cout << "  Assets: " << fixed << setprecision(2) << rs->getDouble(3) << endl; // Format currency
        } else {
            cout << "Branch '" << branchName << "' not found." << endl;
        }
        stmt->closeResultSet(rs);
        conn->terminateStatement(stmt);
    } catch (SQLException& ex) {
        cerr << "Error viewing branch details: " << ex.getMessage() << endl;
        if (rs) {
            try{
               stmt->closeResultSet(rs);
             }catch(SQLException &e){
                cerr<< "Error closing result set:" << e.getMessage() << endl;
             }
           }
        if (stmt) conn->terminateStatement(stmt);
        throw;
    }
}