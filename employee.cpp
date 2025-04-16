#include "employee.h"
#include <iomanip>

// Function to add a new employee
void addEmployee(Connection* conn) {
    cout << "\n--- Add New Employee ---" << endl;
    string empId = getStringInput("Enter Employee ID: ");
    string empName = getStringInput("Enter Employee Name: ");
    string phone = getStringInput("Enter Phone Number: ");
    string branchName = getStringInput("Enter Branch Name: ");
    string managerId;
    cout << "Enter Manager ID (or leave blank if none): ";
    getline(cin, managerId);


    Statement* stmt = nullptr;
    try {
        if (managerId.empty()) {
            stmt = conn->createStatement(
                "INSERT INTO Employee (employee_id, employee_name, telephone_no, start_date, works_for_branch_name) "
                "VALUES (:1, :2, :3, SYSDATE, :4)"
            );
            stmt->setString(1, empId);
            stmt->setString(2, empName);
            stmt->setString(3, phone);
            stmt->setString(4, branchName);

        } else {
             stmt = conn->createStatement(
                "INSERT INTO Employee (employee_id, employee_name, telephone_no, start_date, works_for_branch_name, manager_id) "
                "VALUES (:1, :2, :3, SYSDATE, :4, :5)"
            );
            stmt->setString(1, empId);
            stmt->setString(2, empName);
            stmt->setString(3, phone);
            stmt->setString(4, branchName);
            stmt->setString(5, managerId);
        }


        int rowsAffected = stmt->executeUpdate();

        if (rowsAffected > 0) {
            conn->commit();
            cout << "Employee '" << empName << "' added successfully." << endl;
        } else {
            cout << "Failed to add employee." << endl;
            conn->rollback();
        }
        conn->terminateStatement(stmt);
    } catch (SQLException& ex) {
        cerr << "Error adding employee: " << ex.getMessage() << endl;
        if (stmt) conn->terminateStatement(stmt);
        conn->rollback();
        throw;
    }
}

// Function to view employee details
void viewEmployeeDetails(Connection* conn) {
    cout << "\n--- View Employee Details ---" << endl;
    string empId = getStringInput("Enter Employee ID: ");

    Statement* stmt = nullptr;
    ResultSet* rs = nullptr;
    try {
        stmt = conn->createStatement(
            "SELECT employee_name, telephone_no, start_date, works_for_branch_name, manager_id "
            "FROM Employee WHERE employee_id = :1"
        );
        stmt->setString(1, empId);
        rs = stmt->executeQuery();

        if (rs->next()) {
            cout << "\nEmployee Information:" << endl;
            cout << "  ID:          " << empId << endl;
            cout << "  Name:        " << rs->getString(1) << endl;
            cout << "  Phone:       " << rs->getString(2) << endl;
            cout << "  Start Date:  " << rs->getDate(3).toText("YYYY-MM-DD") << endl;
            cout << "  Branch:      " << rs->getString(4) << endl;
            string managerId = rs->isNull(5) ? "N/A" : rs->getString(5);
            cout << "  Manager ID:  " << managerId << endl;
        } else {
            cout << "Employee ID '" << empId << "' not found." << endl;
        }
        stmt->closeResultSet(rs);
        conn->terminateStatement(stmt);
    } catch (SQLException& ex) {
        cerr << "Error viewing employee details: " << ex.getMessage() << endl;
        if (rs) {
            try{
              stmt->closeResultSet(rs);
            }catch(SQLException &e){
               cerr << "Error closing result set:" << e.getMessage() << endl;
            }
           }
        if (stmt) conn->terminateStatement(stmt);
        throw;
    }
}