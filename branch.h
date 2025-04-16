#ifndef BANK_BRANCH_H
#define BANK_BRANCH_H

#include <iostream>
#include <string>
#include <occi.h>
#include "database.h"

using namespace oracle::occi;
using namespace std;

// Function to add a new branch
void addBranch(Connection* conn);

// Function to view branch details
void viewBranchDetails(Connection* conn);

#endif // BANK_BRANCH_H