-- Drop existing tables (optional, useful for rerunning the script)
-- Use with caution, as this deletes data!
BEGIN
   EXECUTE IMMEDIATE 'DROP TABLE Loan_Payment';
EXCEPTION WHEN OTHERS THEN IF SQLCODE != -942 THEN RAISE; END IF;
END;
/
BEGIN
   EXECUTE IMMEDIATE 'DROP TABLE Borrower';
EXCEPTION WHEN OTHERS THEN IF SQLCODE != -942 THEN RAISE; END IF;
END;
/
BEGIN
   EXECUTE IMMEDIATE 'DROP TABLE Depositor';
EXCEPTION WHEN OTHERS THEN IF SQLCODE != -942 THEN RAISE; END IF;
END;
/
BEGIN
   EXECUTE IMMEDIATE 'DROP TABLE Payment';
EXCEPTION WHEN OTHERS THEN IF SQLCODE != -942 THEN RAISE; END IF;
END;
/
BEGIN
   EXECUTE IMMEDIATE 'DROP TABLE Loan';
EXCEPTION WHEN OTHERS THEN IF SQLCODE != -942 THEN RAISE; END IF;
END;
/
BEGIN
   EXECUTE IMMEDIATE 'DROP TABLE Savings_Account';
EXCEPTION WHEN OTHERS THEN IF SQLCODE != -942 THEN RAISE; END IF;
END;
/
BEGIN
   EXECUTE IMMEDIATE 'DROP TABLE Checking_Account';
EXCEPTION WHEN OTHERS THEN IF SQLCODE != -942 THEN RAISE; END IF;
END;
/
BEGIN
   EXECUTE IMMEDIATE 'DROP TABLE Account';
EXCEPTION WHEN OTHERS THEN IF SQLCODE != -942 THEN RAISE; END IF;
END;
/
BEGIN
   EXECUTE IMMEDIATE 'DROP TABLE Customer';
EXCEPTION WHEN OTHERS THEN IF SQLCODE != -942 THEN RAISE; END IF;
END;
/
BEGIN
   EXECUTE IMMEDIATE 'DROP TABLE Employee';
EXCEPTION WHEN OTHERS THEN IF SQLCODE != -942 THEN RAISE; END IF;
END;
/
BEGIN
   EXECUTE IMMEDIATE 'DROP TABLE Branch';
EXCEPTION WHEN OTHERS THEN IF SQLCODE != -942 THEN RAISE; END IF;
END;
/

-- Create Tables

CREATE TABLE Branch (
    branch_name VARCHAR2(100) PRIMARY KEY,
    branch_city VARCHAR2(100),
    assets NUMBER(15, 2) -- Assuming assets are monetary
);

CREATE TABLE Employee (
    employee_id VARCHAR2(20) PRIMARY KEY,
    employee_name VARCHAR2(100) NOT NULL,
    telephone_no VARCHAR2(20),
    start_date DATE,
    works_for_branch_name VARCHAR2(100),
    manager_id VARCHAR2(20), -- Self-referencing FK for manages relationship
    CONSTRAINT fk_emp_branch FOREIGN KEY (works_for_branch_name) REFERENCES Branch(branch_name),
    CONSTRAINT fk_emp_manager FOREIGN KEY (manager_id) REFERENCES Employee(employee_id)
);

CREATE TABLE Customer (
    customer_id VARCHAR2(20) PRIMARY KEY,
    customer_name VARCHAR2(100) NOT NULL,
    customer_street VARCHAR2(150),
    customer_city VARCHAR2(100),
    tested_by_employee_id VARCHAR2(20), -- From ambiguous 'Test' relationship
    CONSTRAINT fk_cust_employee FOREIGN KEY (tested_by_employee_id) REFERENCES Employee(employee_id)
);

CREATE TABLE Account (
    account_number VARCHAR2(30) PRIMARY KEY,
    balance NUMBER(15, 2) DEFAULT 0.00 NOT NULL,
    account_branch_name VARCHAR2(100), -- Account-Branch relationship
    date_opened DATE DEFAULT SYSDATE,
    account_type VARCHAR2(10) NOT NULL CHECK (account_type IN ('Savings', 'Checking')), -- To distinguish subtypes
    CONSTRAINT fk_acc_branch FOREIGN KEY (account_branch_name) REFERENCES Branch(branch_name)
);

CREATE TABLE Savings_Account (
    account_number VARCHAR2(30) PRIMARY KEY,
    interest_rate NUMBER(5, 2), -- e.g., 3.25%
    CONSTRAINT fk_savacc_acc FOREIGN KEY (account_number) REFERENCES Account(account_number) ON DELETE CASCADE
);

CREATE TABLE Checking_Account (
    account_number VARCHAR2(30) PRIMARY KEY,
    overdraft_limit NUMBER(10, 2),
    CONSTRAINT fk_chkacc_acc FOREIGN KEY (account_number) REFERENCES Account(account_number) ON DELETE CASCADE
);

-- Updated Loan table
CREATE TABLE Loan (
    loan_number VARCHAR2(30) PRIMARY KEY,
    total_amount NUMBER(15, 2) NOT NULL,
    remaining_amount NUMBER(15, 2) NOT NULL,
    loan_type VARCHAR2(20),
    date_issued DATE DEFAULT SYSDATE,
    loan_branch_name VARCHAR2(100),
    CONSTRAINT fk_loan_branch FOREIGN KEY (loan_branch_name) REFERENCES Branch(branch_name)
);

CREATE TABLE Payment (
    payment_number VARCHAR2(40) PRIMARY KEY, -- Might need a sequence or better generation strategy
    payment_date DATE NOT NULL,
    payment_amount NUMBER(15, 2) NOT NULL
);

-- Junction Tables for Many-to-Many Relationships

CREATE TABLE Borrower ( -- Customer <-> Loan
    customer_id VARCHAR2(20),
    loan_number VARCHAR2(30),
    CONSTRAINT pk_borrower PRIMARY KEY (customer_id, loan_number),
    CONSTRAINT fk_borrower_cust FOREIGN KEY (customer_id) REFERENCES Customer(customer_id) ON DELETE CASCADE,
    CONSTRAINT fk_borrower_loan FOREIGN KEY (loan_number) REFERENCES Loan(loan_number) ON DELETE CASCADE
);

CREATE TABLE Depositor ( -- Customer <-> Account
    customer_id VARCHAR2(20),
    account_number VARCHAR2(30),
    access_date DATE, -- Attribute on the relationship
    CONSTRAINT pk_depositor PRIMARY KEY (customer_id, account_number),
    CONSTRAINT fk_depositor_cust FOREIGN KEY (customer_id) REFERENCES Customer(customer_id) ON DELETE CASCADE,
    CONSTRAINT fk_depositor_acc FOREIGN KEY (account_number) REFERENCES Account(account_number) ON DELETE CASCADE
);

CREATE TABLE Loan_Payment ( -- Loan <-> Payment
    loan_number VARCHAR2(30),
    payment_number VARCHAR2(40),
    CONSTRAINT pk_loan_payment PRIMARY KEY (loan_number, payment_number),
    CONSTRAINT fk_loanpay_loan FOREIGN KEY (loan_number) REFERENCES Loan(loan_number) ON DELETE CASCADE,
    CONSTRAINT fk_loanpay_pay FOREIGN KEY (payment_number) REFERENCES Payment(payment_number) ON DELETE CASCADE
);

-- Add sample data (optional, for testing)
INSERT INTO Branch (branch_name, branch_city, assets) VALUES ('Downtown', 'Metropolis', 50000000.00);
INSERT INTO Branch (branch_name, branch_city, assets) VALUES ('Uptown', 'Metropolis', 35000000.00);

INSERT INTO Employee (employee_id, employee_name, telephone_no, start_date, works_for_branch_name) VALUES ('E001', 'Alice Manager', '555-1111', DATE '2018-05-15', 'Downtown');
INSERT INTO Employee (employee_id, employee_name, telephone_no, start_date, works_for_branch_name, manager_id) VALUES ('E002', 'Bob Teller', '555-1112', DATE '2020-01-20', 'Downtown', 'E001');
INSERT INTO Employee (employee_id, employee_name, telephone_no, start_date, works_for_branch_name) VALUES ('E003', 'Charlie LoanOff', '555-2222', DATE '2019-03-10', 'Uptown');

INSERT INTO Customer (customer_id, customer_name, customer_street, customer_city, tested_by_employee_id) VALUES ('C001', 'John Doe', '123 Main St', 'Metropolis', 'E002');
INSERT INTO Customer (customer_id, customer_name, customer_street, customer_city) VALUES ('C002', 'Jane Smith', '456 Oak Ave', 'Metropolis');

-- Note: For accounts, first insert into Account, then into Savings/Checking, then into Depositor
INSERT INTO Account (account_number, balance, account_branch_name, account_type) VALUES ('A-DT-001', 5000.00, 'Downtown', 'Savings');
INSERT INTO Savings_Account (account_number, interest_rate) VALUES ('A-DT-001', 1.5);
INSERT INTO Depositor (customer_id, account_number, access_date) VALUES ('C001', 'A-DT-001', SYSDATE);

INSERT INTO Account (account_number, balance, account_branch_name, account_type) VALUES ('A-UP-001', 1200.50, 'Uptown', 'Checking');
INSERT INTO Checking_Account (account_number, overdraft_limit) VALUES ('A-UP-001', 500.00);
INSERT INTO Depositor (customer_id, account_number, access_date) VALUES ('C001', 'A-UP-001', SYSDATE);
INSERT INTO Depositor (customer_id, account_number, access_date) VALUES ('C002', 'A-UP-001', SYSDATE); -- Shared account

INSERT INTO Loan (loan_number, amount, loan_branch_name) VALUES ('L-DT-001', 10000.00, 'Downtown');
INSERT INTO Borrower (customer_id, loan_number) VALUES ('C002', 'L-DT-001');

COMMIT;

SELECT table_name FROM user_tables; -- Verify tables are created

DESC Branch;
DESC Employee;
DESC Customer;
DESC Account;
DESC Savings_Account;
DESC Checking_Account;
DESC Loan;
DESC Payment;
DESC Borrower;
DESC Depositor;
DESC Loan_Payment;

-- Check Sample Data
SELECT * FROM Branch;
SELECT * FROM Employee;
SELECT * FROM Customer;
SELECT a.*, sa.interest_rate, ca.overdraft_limit FROM Account a LEFT JOIN Savings_Account sa ON a.account_number = sa.account_number LEFT JOIN Checking_Account ca ON a.account_number = ca.account_number;
SELECT * FROM Loan;
SELECT * FROM Borrower;
SELECT * FROM Depositor;