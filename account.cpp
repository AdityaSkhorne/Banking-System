#include "account.h"

// Function to add a new account
void addAccount(Connection* conn) {
    cout << "\n--- Add New Account ---" << endl;
    string accNum = getStringInput("Enter Account Number: ");
    string branchName = getStringInput("Enter Branch Name: ");
    double initialBalance = getDoubleInput("Enter Initial Balance: ");
    string accType;
    while (true) {
        cout << "Enter Account Type (Savings/Checking): ";
        getline(cin, accType);
        if (accType == "Savings" || accType == "Checking") {
            break;
        } else {
            cout << "Invalid type. Please enter 'Savings' or 'Checking'." << endl;
        }
    }

    Statement* stmtBase = nullptr;
    Statement* stmtSpecific = nullptr;

    try {
        // 1. Insert into base Account table
        stmtBase = conn->createStatement(
            "INSERT INTO Account (account_number, balance, account_branch_name, account_type) "
            "VALUES (:1, :2, :3, :4)"
        );
        stmtBase->setString(1, accNum);
        stmtBase->setDouble(2, initialBalance);
        stmtBase->setString(3, branchName);
        stmtBase->setString(4, accType);

        int rowsBase = stmtBase->executeUpdate();

        if (rowsBase <= 0) {
            throw runtime_error("Failed to insert into base Account table.");
        }


        // 2. Insert into specific account type table
        if (accType == "Savings") {
            double interestRate = getDoubleInput("Enter Interest Rate (e.g., 1.5): ");
            stmtSpecific = conn->createStatement(
                "INSERT INTO Savings_Account (account_number, interest_rate) VALUES (:1, :2)"
            );
            stmtSpecific->setString(1, accNum);
            stmtSpecific->setDouble(2, interestRate);
        } else { // Checking
            double overdraftLimit = getDoubleInput("Enter Overdraft Limit: ");
            stmtSpecific = conn->createStatement(
                "INSERT INTO Checking_Account (account_number, overdraft_limit) VALUES (:1, :2)"
            );
            stmtSpecific->setString(1, accNum);
            stmtSpecific->setDouble(2, overdraftLimit);
        }

        int rowsSpecific = stmtSpecific->executeUpdate();

        if (rowsSpecific <= 0) {
            throw runtime_error("Failed to insert into specific account table.");
        }

        // 3. If all successful, commit
        conn->commit();
        cout << accType << " Account '" << accNum << "' created successfully." << endl;

    } catch (SQLException& ex) {
        cerr << "Database Error adding account: " << ex.getMessage() << endl;
        if (stmtBase) {
            try{
               conn->terminateStatement(stmtBase);
             } catch(SQLException &e){
                cerr << "Error terminating statement:" << e.getMessage() << endl;
             }
           }
        if (stmtSpecific) {
            try{
              conn->terminateStatement(stmtSpecific);
            } catch(SQLException &e){
               cerr << "Error terminating statement:" << e.getMessage() << endl;
            }
        }
        conn->rollback();
        throw;
    } catch (const runtime_error& err) {
        cerr << "Error adding account: " << err.what() << endl;
         if (stmtBase)  {
            try{
               conn->terminateStatement(stmtBase);
             } catch(SQLException &e){
                cerr << "Error terminating statement:" << e.getMessage() << endl;
             }
           }
        if (stmtSpecific) {
            try{
              conn->terminateStatement(stmtSpecific);
            } catch(SQLException &e){
               cerr << "Error terminating statement:" << e.getMessage() << endl;
            }
        }
        conn->rollback();
        throw;
    }
}

// Function to deposit money into an account
void depositMoney(Connection* conn) {
    cout << "\n--- Deposit Money ---" << endl;
    string accNum = getStringInput("Enter Account Number: ");
    double amount = getDoubleInput("Enter Amount to Deposit: ");

    Statement* stmt = nullptr;
    try {
        // 1. Update account balance
        stmt = conn->createStatement(
            "UPDATE Account SET balance = balance + :1 WHERE account_number = :2"
        );
        stmt->setDouble(1, amount);
        stmt->setString(2, accNum);

        int rowsAffected = stmt->executeUpdate();

        if (rowsAffected > 0) {
            conn->commit();
            cout << "Successfully deposited " << amount << " into account " << accNum << endl;
        } else {
            cout << "Deposit failed. Account not found or invalid amount." << endl;
            conn->rollback();
        }
        conn->terminateStatement(stmt);
    } catch (SQLException& ex) {
        cerr << "Error depositing money: " << ex.getMessage() << endl;
        if (stmt)  {
            try{
               conn->terminateStatement(stmt);
             } catch(SQLException &e){
                cerr << "Error terminating statement:" << e.getMessage() << endl;
             }
           }
        conn->rollback();
        throw;
    }
}

// Function to withdraw money from an account
void withdrawMoney(Connection* conn) {
    cout << "\n--- Withdraw Money ---" << endl;
    string accNum = getStringInput("Enter Account Number: ");
    double amount = getDoubleInput("Enter Amount to Withdraw: ");

    Statement* stmt = nullptr;
    try {
        // Check for sufficient funds (including overdraft if applicable)
        string accType;
        double balance;

        // Get account type and current balance
        stmt = conn->createStatement(
            "SELECT balance, account_type FROM Account WHERE account_number = :1"
        );
        stmt->setString(1, accNum);
        ResultSet* rs = stmt->executeQuery();

        if (rs->next()) {
            balance = rs->getDouble(1);
            accType = rs->getString(2);
        } else {
            cout << "Account not found." << endl;
            conn->terminateStatement(stmt);
            return;
        }
        stmt->closeResultSet(rs);

        double availableFunds = balance;
        if (accType == "Checking") {
            //get overdraft limit
            Statement* stmtChk = conn->createStatement("SELECT overdraft_limit from Checking_Account where account_number = :1");
            stmtChk->setString(1,accNum);
            ResultSet *rsChk = stmtChk->executeQuery();
            if(rsChk->next()){
                availableFunds += rsChk->getDouble(1);
            }
            stmtChk->closeResultSet(rsChk);
            conn->terminateStatement(stmtChk);
        }
       conn->terminateStatement(stmt);
       stmt = nullptr;

        if (amount > availableFunds) {
            cout << "Insufficient funds." << endl;
            return;
        }

        // 2. Update account balance
        stmt = conn->createStatement(
            "UPDATE Account SET balance = balance - :1 WHERE account_number = :2"
        );
        stmt->setDouble(1, amount);
        stmt->setString(2, accNum);

        int rowsAffected = stmt->executeUpdate();

        if (rowsAffected > 0) {
            conn->commit();
            cout << "Successfully withdrew " << amount << " from account " << accNum << endl;
        } else {
            cout << "Withdrawal failed." << endl;
            conn->rollback();
        }
        if(stmt){
           conn->terminateStatement(stmt);
        }

    } catch (SQLException& ex) {
        cerr << "Error withdrawing money: " << ex.getMessage() << endl;
        if (stmt)  {
            try{
               conn->terminateStatement(stmt);
             } catch(SQLException &e){
                cerr << "Error terminating statement:" << e.getMessage() << endl;
             }
           }
        conn->rollback();
        throw;
    }
}

void manageDepositors(Connection* conn) {
    cout << "\n--- Manage Account Holders (Depositors) ---" << endl;
    string accNum = getStringInput("Enter Account Number to manage: ");
    string custId = getStringInput("Enter Customer ID to add/remove: ");
    string action;
    while (true) {
        cout << "Action (add/remove): ";
        getline(cin, action);
        if (action == "add" || action == "remove") break;
        cout << "Invalid action." << endl;
    }

    Statement* stmt = nullptr;
    try {
        if (action == "add") {
            cout << "Adding customer " << custId << " to account " << accNum << endl;
            stmt = conn->createStatement(
                "INSERT INTO Depositor (customer_id, account_number, access_date) VALUES (:1, :2, SYSDATE)"
            );
            stmt->setString(1, custId);
            stmt->setString(2, accNum);
            int rowsAffected = stmt->executeUpdate();
            if (rowsAffected > 0) {
                conn->commit();
                cout << "Customer added as depositor successfully." << endl;
            } else {
                cout << "Failed to add depositor (maybe already exists, or IDs invalid?)." << endl;
                conn->rollback();
            }

        } else { // remove
            cout << "Removing customer " << custId << " from account " << accNum << endl;
            stmt = conn->createStatement(
                "DELETE FROM Depositor WHERE customer_id = :1 AND account_number = :2"
            );
            stmt->setString(1, custId);
            stmt->setString(2, accNum);
            int rowsAffected = stmt->executeUpdate();
            if (rowsAffected > 0) {
                conn->commit();
                cout << "Customer removed as depositor successfully." << endl;
            } else {
                cout << "Failed to remove depositor (maybe relationship didn't exist?)." << endl;
                conn->rollback();
            }
        }
        conn->terminateStatement(stmt);
    } catch (SQLException& ex) {
        cerr << "Error managing depositor: " << ex.getMessage() << endl;
        if (stmt) conn->terminateStatement(stmt);
        conn->rollback();
        throw;
    }
}