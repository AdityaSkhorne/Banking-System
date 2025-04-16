#include <iostream>
#include <string>
#include <occi.h>
#include "database.h"
#include "customer.h"
#include "account.h"
#include "loan.h"
#include "branch.h"
#include "employee.h"
#include <limits>

using namespace oracle::occi;
using namespace std;

int main() {
    Environment* env = nullptr;
    Connection* conn = nullptr;

    try {
        // 1. Create OCCI environment
        env = Environment::createEnvironment(Environment::DEFAULT);
        cout << "OCCI Environment created." << endl;

        // 2. Establish connection
        conn = connectToDatabase(env);

        // --- Main Application Menu ---
        int choice = 0;
        do {
            cout << "\n--- Banking Application Menu ---" << endl;
            cout << "1. Add New Customer" << endl;
            cout << "2. View Customer Details" << endl;
            cout << "3. Add New Account" << endl;
            cout << "4. Deposit Money" << endl;
            cout << "5. Withdraw Money" << endl;
            cout << "6. Manage Account Holders (Depositors)" << endl;
            cout << "7. Apply for Loan" << endl;
            cout << "8. Make Loan Payment" << endl;
            cout << "9. Add New Branch" << endl;
            cout << "10. View Branch Details" << endl;
            cout << "11. Add New Employee" << endl;
            cout << "12. View Employee Details" << endl;
            cout << "0. Exit" << endl;
            cout << "-------------------------------" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            if (cin.fail()) {
                cout << "Invalid input. Please enter a number." << endl;
                cin.clear();
                clearInputBuffer();
                choice = -1;
                continue;
            }
            clearInputBuffer();

            try { //added try block
                switch (choice) {
                    case 1:
                        addCustomer(conn);
                        break;
                    case 2:
                        viewCustomerDetails(conn);
                        break;
                    case 3:
                        addAccount(conn);
                        break;
                    case 4:
                        depositMoney(conn);
                        break;
                    case 5:
                        withdrawMoney(conn);
                        break;
                    case 6:
                        manageDepositors(conn);
                        break;
                    case 7:
                        applyForLoan(conn);
                        break;
                    case 8:
                        makeLoanPayment(conn);
                        break;
                    case 9:
                        addBranch(conn);
                        break;
                    case 10:
                        viewBranchDetails(conn);
                        break;
                    case 11:
                        addEmployee(conn);
                        break;
                    case 12:
                        viewEmployeeDetails(conn);
                        break;
                    case 0:
                        cout << "Exiting application." << endl;
                        break;
                    default:
                        cout << "Invalid choice. Please try again." << endl;
                }
            } catch (SQLException &ex) {
                // Handle SQLException here.  The individual functions also 
                ow,
                // so this catch block will handle any database errors from them.
                cerr << "Database Error: " << ex.getMessage() << " (Error Code: " << ex.getErrorCode() << ")" << endl;
                conn->rollback();
            }

        } while (choice != 0);

        // 3. Terminate connection
        env->terminateConnection(conn);
        cout << "Database connection terminated." << endl;

        // 4. Terminate environment
        Environment::terminateEnvironment(env);
        cout << "OCCI Environment terminated." << endl;

    } catch (SQLException &ex) {
        cerr << "Database Error: " << ex.getMessage() << " (Error Code: " << ex.getErrorCode() << ")" << endl;
        // Handle initial connection errors
        if (conn) {
            try {
                env->terminateConnection(conn);
            } catch (SQLException &cleanupEx) {
                cerr << "Error during connection cleanup: " << cleanupEx.getMessage() << endl;
            }
        }
        if (env) {
            try {
                Environment::terminateEnvironment(env);
            } catch (SQLException &cleanupEx) {
                cerr << "Error during environment cleanup: " << cleanupEx.getMessage() << endl;
            }
        }
        return 1;
    } catch (exception &ex) {
        cerr << "Standard Error: " << ex.what() << endl;
        return 1;
    }

    return 0;
}