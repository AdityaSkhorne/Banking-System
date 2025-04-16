#ifndef BANK_EMPLOYEE_H
#define BANK_EMPLOYEE_H

#include <iostream>
#include <string>
#include <occi.h>
#include "database.h"

using namespace oracle::occi;
using namespace std;

// Function to add a new employee
void addEmployee(Connection* conn);

// Function to view employee details
void viewEmployeeDetails(Connection* conn);

#endif // BANK_EMPLOYEE_H