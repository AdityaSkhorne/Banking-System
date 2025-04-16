#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

#include <iostream>
#include <string>
#include <occi.h>
#include "database.h"

using namespace oracle::occi;
using namespace std;

// Function to add a new account
void addAccount(Connection* conn);

// Function to deposit money into an account
void depositMoney(Connection* conn);

// Function to withdraw money from an account
void withdrawMoney(Connection* conn);

// Function to manage account holders (depositors)
void manageDepositors(Connection* conn);

#endif // BANK_ACCOUNT_H