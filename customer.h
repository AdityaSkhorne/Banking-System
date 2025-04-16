#ifndef BANK_CUSTOMER_H
#define BANK_CUSTOMER_H

#include <iostream>
#include <string>
#include <occi.h>
#include "database.h"

using namespace oracle::occi;
using namespace std;

// Function to add a new customer
void addCustomer(Connection* conn);

// Function to view customer details
void viewCustomerDetails(Connection* conn);

#endif // BANK_CUSTOMER_H