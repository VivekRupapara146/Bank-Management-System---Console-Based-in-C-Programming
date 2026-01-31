#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <string.h>

#define AccFileName "accounts.dat"
#define TrancFileName "transactions.dat"
#define UserFileName "users.dat"
#define ROLE_ADMIN 1
#define ROLE_CUSTOMER 2

struct BankAccount
{
    long accNum;
    char name[50];
    char accType[20]; // Savings  Current
    double bal;
    int active; // 0 - not active 1 - active
};

struct Transaction
{
    long trancId;
    long accNum;
    long relAcc;
    char type[20];
    double Amt;
    char date[15];
    char time[20];
};

struct User
{
    char uName[30];
    char pass[30];
    int role;       // 0 - Admin 1 - Customer
    long linkedAcc; // 0 for admin
};

struct Session
{
    int isLoggedIn;     // 1 = logged in, 0 = logged out
    int role;           // Admin or Customer
    long activeAccount; // Customer account number
};

// struct AccSearch
// {
//     int found; // 0 -not found, 1 -found
//     struct BankAccount acc;
// };