#include "loan.h"
#include <iomanip>

// Function to apply for a loan
void applyForLoan(Connection* conn) {
    cout << "\n--- Apply for Loan ---" << endl;
    string custId = getStringInput("Enter Customer ID: ");
    string branchName = getStringInput("Enter Branch Name: ");
    double loanAmount = getDoubleInput("Enter Loan Amount: ");
    string loanNumber = getStringInput("Enter Loan Number: "); // Get Loan Number

    Statement* stmtLoan = nullptr;
    Statement* stmtBorrower = nullptr;
    try {
        // 1. Insert into Loan table
        stmtLoan = conn->createStatement(
            "INSERT INTO Loan (loan_number, amount, loan_branch_name, date_issued) "
            "VALUES (:1, :2, :3, SYSDATE)"
        );
        stmtLoan->setString(1, loanNumber); // Use input loan number
        stmtLoan->setDouble(2, loanAmount);
        stmtLoan->setString(3, branchName);

        int rowsLoan = stmtLoan->executeUpdate();

        if (rowsLoan <= 0) {
            throw runtime_error("Failed to insert into Loan table.");
        }
        // 2. Insert into Borrower table (junction table)
        stmtBorrower = conn->createStatement(
            "INSERT INTO Borrower (customer_id, loan_number) VALUES (:1, :2)"
        );
        stmtBorrower->setString(1, custId);
        stmtBorrower->setString(2, loanNumber); // Use input loan number

        int rowsBorrower = stmtBorrower->executeUpdate();

        if (rowsBorrower <= 0) {
            throw runtime_error("Failed to insert into Borrower table.");
        }

        conn->commit();
        cout << "Loan application for " << loanAmount << " successful. Loan Number: " << loanNumber << endl;

    } catch (SQLException& ex) {
        cerr << "Error applying for loan: " << ex.getMessage() << endl;
        if (stmtLoan) {
            try{
              conn->terminateStatement(stmtLoan);
            }catch(SQLException &e){
               cerr << "Error terminating statement:" << e.getMessage() << endl;
            }
           }
        if (stmtBorrower) {
            try{
              conn->terminateStatement(stmtBorrower);
            }catch(SQLException &e){
               cerr << "Error terminating statement:" << e.getMessage() << endl;
            }
           }
        conn->rollback();
        throw;
    } catch (const runtime_error& err) {
        cerr << "Error applying for loan: " << err.what() << endl;
        if (stmtLoan) {
            try{
              conn->terminateStatement(stmtLoan);
            }catch(SQLException &e){
               cerr << "Error terminating statement:" << e.getMessage() << endl;
            }
           }
        if (stmtBorrower) {
            try{
              conn->terminateStatement(stmtBorrower);
            }catch(SQLException &e){
               cerr << "Error terminating statement:" << e.getMessage() << endl;
            }
           }
        conn->rollback();
        throw;
    }
}

// Function to make a loan payment
void makeLoanPayment(Connection* conn) {
    cout << "\n--- Make Loan Payment ---" << endl;
    string loanNumber = getStringInput("Enter Loan Number: ");
    double paymentAmount = getDoubleInput("Enter Payment Amount: ");
    string paymentNumber = getStringInput("Enter Payment Number: "); // Get Payment Number

    Statement* stmtPayment = nullptr;
    Statement* stmtLoanPayment = nullptr;
    try {
        // 1. Insert into Payment table
        stmtPayment = conn->createStatement(
            "INSERT INTO Payment (payment_number, payment_date, payment_amount) "
            "VALUES (:1, SYSDATE, :2)"
        );
        stmtPayment->setString(1, paymentNumber); // Use input payment number
        stmtPayment->setDouble(2, paymentAmount);

        int rowsPayment = stmtPayment->executeUpdate();

        if (rowsPayment <= 0) {
            throw runtime_error("Failed to insert into Payment table.");
        }

        // 2. Insert into Loan_Payment table (junction table)
        stmtLoanPayment = conn->createStatement(
            "INSERT INTO Loan_Payment (loan_number, payment_number) VALUES (:1, :2)"
        );
        stmtLoanPayment->setString(1, loanNumber);
        stmtLoanPayment->setString(2, paymentNumber); // Use input payment number

        int rowsLoanPayment = stmtLoanPayment->executeUpdate();

        if (rowsLoanPayment <= 0) {
            throw runtime_error("Failed to insert into Loan_Payment table.");
        }

        // Optionally, update Loan balance (not required by ERD, but often done)
        // (Simplified:  Does not handle interest, etc.)
        Statement* stmtUpdateLoan = conn->createStatement(
            "UPDATE Loan SET amount = amount - :1 WHERE loan_number = :2"
        );
        stmtUpdateLoan->setDouble(1, paymentAmount);
        stmtUpdateLoan->setString(2, loanNumber);
        stmtUpdateLoan->executeUpdate();
        conn->terminateStatement(stmtUpdateLoan);


        conn->commit();
        cout << "Payment of " << paymentAmount << " made for Loan Number: " << loanNumber
            << ". Payment Number: " << paymentNumber << endl;

    } catch (SQLException& ex) {
        cerr << "Error making loan payment: " << ex.getMessage() << endl;
         if (stmtPayment) {
            try{
              conn->terminateStatement(stmtPayment);
            }catch(SQLException &e){
               cerr << "Error terminating statement:" << e.getMessage() << endl;
            }
           }
        if (stmtLoanPayment) {
            try{
              conn->terminateStatement(stmtLoanPayment);
            }catch(SQLException &e){
               cerr << "Error terminating statement:" << e.getMessage() << endl;
            }
           }
        conn->rollback();
        throw;
    } catch (const runtime_error& err) {
        cerr << "Error making loan payment: " << err.what() << endl;
         if (stmtPayment) {
            try{
              conn->terminateStatement(stmtPayment);
            }catch(SQLException &e){
               cerr << "Error terminating statement:" << e.getMessage() << endl;
            }
           }
        if (stmtLoanPayment) {
            try{
              conn->terminateStatement(stmtLoanPayment);
            }catch(SQLException &e){
               cerr << "Error terminating statement:" << e.getMessage() << endl;
            }
           }
        conn->rollback();
        throw;
    }
}