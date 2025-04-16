#include "customer.h"

// Function to add a new customer
void addCustomer(Connection* conn) {
    cout << "\n--- Add New Customer ---" << endl;
    string custId = getStringInput("Enter Customer ID: ");
    string custName = getStringInput("Enter Customer Name: ");
    string custStreet = getStringInput("Enter Street Address: ");
    string custCity = getStringInput("Enter City: ");

    Statement* stmt = nullptr;
    try {
        stmt = conn->createStatement(
            "INSERT INTO Customer (customer_id, customer_name, customer_street, customer_city) "
            "VALUES (:1, :2, :3, :4)"
        );
        stmt->setString(1, custId);
        stmt->setString(2, custName);
        stmt->setString(3, custStreet);
        stmt->setString(4, custCity);

        int rowsAffected = stmt->executeUpdate();

        if (rowsAffected > 0) {
            conn->commit();
            cout << "Customer '" << custName << "' added successfully." << endl;
        } else {
            cout << "Failed to add customer." << endl;
            conn->rollback();
        }
        conn->terminateStatement(stmt);
    } catch (SQLException& ex) {
        cerr << "Error adding customer: " << ex.getMessage() << endl;
        if (stmt) conn->terminateStatement(stmt);
        conn->rollback();
        throw; // Re-throw
    }
}

// Function to view customer details
void viewCustomerDetails(Connection* conn) {
    cout << "\n--- View Customer Details ---" << endl;
    string custId = getStringInput("Enter Customer ID: ");

    Statement* stmtCust = nullptr;
    Statement* stmtAcc = nullptr;
    ResultSet* rsCust = nullptr;
    ResultSet* rsAcc = nullptr;

    try {
        // 1. Get Customer Info
        stmtCust = conn->createStatement(
            "SELECT customer_name, customer_street, customer_city, tested_by_employee_id "
            "FROM Customer WHERE customer_id = :1"
        );
        stmtCust->setString(1, custId);
        rsCust = stmtCust->executeQuery();

        if (!rsCust->next()) {
            cout << "Customer ID '" << custId << "' not found." << endl;
        } else {
            cout << "\nCustomer Information:" << endl;
            cout << "  ID:          " << custId << endl;
            cout << "  Name:        " << rsCust->getString(1) << endl;
            cout << "  Street:      " << rsCust->getString(2) << endl;
            cout << "  City:        " << rsCust->getString(3) << endl;
            string testedBy = rsCust->isNull(4) ? "N/A" : rsCust->getString(4);
            cout << "  Tested By Emp:" << testedBy << endl;

            // 2. Get Associated Accounts
            stmtAcc = conn->createStatement(
                "SELECT a.account_number, a.balance, a.account_type, a.account_branch_name, d.access_date "
                "FROM Account a JOIN Depositor d ON a.account_number = d.account_number "
                "WHERE d.customer_id = :1 ORDER BY a.account_number"
            );
            stmtAcc->setString(1, custId);
            rsAcc = stmtAcc->executeQuery();

            cout << "\nAssociated Accounts:" << endl;
            bool hasAccounts = false;
            while (rsAcc->next()) {
                hasAccounts = true;
                cout << "  --------------------" << endl;
                cout << "  Account No:   " << rsAcc->getString(1) << endl;
                cout << "  Balance:      " << rsAcc->getDouble(2) << endl;
                cout << "  Type:         " << rsAcc->getString(3) << endl;
                cout << "  Branch:       " << rsAcc->getString(4) << endl;
                cout << "  Access Date:  " << rsAcc->getDate(5).toText("YYYY-MM-DD") << endl;
            }
            if (!hasAccounts) {
                cout << "  No accounts found for this customer." << endl;
            }
            cout << "  --------------------" << endl;

            // Add sections here to query and display Loans (via Borrower table)
            // cout << "\nAssociated Loans:" << endl; ...
        }

        // Clean up customer resources
        stmtCust->closeResultSet(rsCust);
        conn->terminateStatement(stmtCust);

    } catch (SQLException& ex) {
        cerr << "Error viewing customer details: " << ex.getMessage() << endl;
        if (rsAcc) {
            try {
                stmtAcc->closeResultSet(rsAcc);
            } catch (SQLException& e) {
                cerr << "Error closing result set: " << e.getMessage() << endl;
            }
        }
        if (stmtAcc) {
            try {
                conn->terminateStatement(stmtAcc);
            } catch (SQLException& e) {
                cerr << "Error terminating statement: " << e.getMessage() << endl;
            }
        }
        if (rsCust) {
            try {
                stmtCust->closeResultSet(rsCust);
            } catch (SQLException& e) {
                cerr << "Error closing result set: " << e.getMessage() << endl;
            }
        }
        if (stmtCust) {
           try{
               conn->terminateStatement(stmtCust);
           } catch(SQLException &e){
                cerr << "Error terminating statement: " << e.getMessage() << endl;
           }
        }
        throw; //Re-throw
    }
}