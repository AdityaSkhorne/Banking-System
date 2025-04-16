#ifndef BANK_LOAN_H
#define BANK_LOAN_H

#include <iostream>
#include <string>
#include <occi.h>
#include "database.h"

using namespace oracle::occi;
using namespace std;

// Function to apply for a loan
void applyForLoan(Connection* conn);

// Function to make a loan payment
void makeLoanPayment(Connection* conn);

#endif // BANK_LOAN_H