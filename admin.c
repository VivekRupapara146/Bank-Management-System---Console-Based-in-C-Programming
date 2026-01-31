#include "banking.h"
#include "admin.h"

void adminMenu(struct Session *session)
{
    int choice;
    do
    {
        printf("\n========== ADMIN MENU ==========");
        printf("\n1. Create New Account.");
        printf("\n2. Display All Account.");
        printf("\n3. Search Account.");
        printf("\n4. Delete Account.");
        printf("\n5. View All Transactions.");
        printf("\n6. Logout.");
        printf("\nEnter the choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            createAccount();
            break;
        case 2:
            displayAllAccounts();
            break;
        case 3:
            searchAccount();
            break;
        case 4:
            deleteAccount();
            break;
        case 5:
            viewAllTransactions();
            break;
        case 6:
            printf("\nAdmin Logged Out.\n");
            session->isLoggedIn = 0;
            session->role = 0;
            session->activeAccount = 0;
            printf("\nYou have been safely logged out.\n");
            break;

        default:
            printf("\nInvalid choice. Please try again.\n");
            break;
        }

    } while (choice != 6);
}
void createAccount()
{
    FILE *accFp, *userFp;
    struct BankAccount newAcc;  // ✅ for new input
    struct BankAccount tempAcc; // ✅ for file traversal
    struct User user;
    long lastAccNo = 1000;
    int ch;

    /* Clear input buffer */
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;

    /* ----------- TAKE INPUT ----------- */
    printf("\nEnter Account Holder Name: ");
    fgets(newAcc.name, sizeof(newAcc.name), stdin);
    newAcc.name[strcspn(newAcc.name, "\n")] = 0;

    printf("Enter Account Type (Savings/Current): ");
    fgets(newAcc.accType, sizeof(newAcc.accType), stdin);
    newAcc.accType[strcspn(newAcc.accType, "\n")] = 0;

    printf("Enter Initial Deposit: ");
    scanf("%lf", &newAcc.bal);

    if (newAcc.bal < 0)
    {
        printf("Initial balance cannot be negative.\n");
        return;
    }

    /* ----------- GENERATE ACCOUNT NUMBER ----------- */
    accFp = fopen(AccFileName, "rb");
    if (accFp != NULL)
    {
        while (fread(&tempAcc, sizeof(tempAcc), 1, accFp))
        {
            lastAccNo = tempAcc.accNum;
        }
        fclose(accFp);
    }

    newAcc.accNum = lastAccNo + 1;
    newAcc.active = 1;

    /* ----------- CREATE USER LOGIN ----------- */
    printf("Create Username: ");
    scanf("%s", user.uName);

    printf("Create Password: ");
    scanf("%s", user.pass);

    user.role = ROLE_CUSTOMER;
    user.linkedAcc = newAcc.accNum;

    /* ----------- SAVE ACCOUNT ----------- */
    accFp = fopen(AccFileName, "ab");
    fwrite(&newAcc, sizeof(newAcc), 1, accFp);
    fclose(accFp);

    /* ----------- SAVE USER ----------- */
    userFp = fopen(UserFileName, "ab");
    fwrite(&user, sizeof(user), 1, userFp);
    fclose(userFp);

    printf("\nAccount Created Successfully!");
    printf("\nAccount Number: %ld\n", newAcc.accNum);
}
void displayAllAccounts()
{
    FILE *fp;
    struct BankAccount acc;
    int found = 0;

    fp = fopen(AccFileName, "rb");
    if (fp == NULL)
    {
        printf("\nError opening account file.\n");
        return;
    }

    printf("\n-----------------------------------------------------------");
    printf("\n%-10s %-20s %-12s %-10s",
           "Acc No", "Name", "Type", "Balance");
    printf("\n-----------------------------------------------------------");

    while (fread(&acc, sizeof(struct BankAccount), 1, fp))
    {
        if (acc.active)
        {
            printf("\n%-10ld %-20s %-12s %-10.2lf",
                   acc.accNum,
                   acc.name,
                   acc.accType,
                   acc.bal);
            found = 1;
        }
    }

    printf("\n-----------------------------------------------------------");

    if (!found)
    {
        printf("\nNo active accounts found.\n");
    }

    fclose(fp);
}
void searchAccount()
{
    FILE *fp;
    struct BankAccount acc;
    long accNo;
    int found = 0;

    printf("\nEnter Account Number to Search: ");
    scanf("%ld", &accNo);

    fp = fopen(AccFileName, "rb");
    if (fp == NULL)
    {
        printf("\nError opening account file.\n");
        return;
    }

    while (fread(&acc, sizeof(struct BankAccount), 1, fp))
    {
        if (acc.accNum == accNo && acc.active)
        {
            printf("\nAccount Found");
            printf("\n-------------------------");
            printf("\nAccount No : %ld", acc.accNum);
            printf("\nName       : %s", acc.name);
            printf("\nType       : %s", acc.accType);
            printf("\nBalance    : %.2lf", acc.bal);
            printf("\nStatus     : Active\n");
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found)
    {
        printf("\nAccount not found or inactive.\n");
    }
}
void deleteAccount()
{
    FILE *fp, *tempFp;
    struct BankAccount acc;
    long delAcc;
    int found = 0;

    printf("\nEnter Account Number to Delete: ");
    scanf("%ld", &delAcc);

    fp = fopen(AccFileName, "rb");
    if (fp == NULL)
    {
        printf("\nError opening account file.\n");
        return;
    }

    tempFp = fopen("temp.dat", "wb");
    if (tempFp == NULL)
    {
        fclose(fp);
        printf("\nError creating temp file.\n");
        return;
    }

    while (fread(&acc, sizeof(struct BankAccount), 1, fp))
    {
        if (acc.accNum == delAcc && acc.active == 1)
        {
            found = 1;

            if (acc.bal != 0)
            {
                printf("\nAccount cannot be deleted. Balance is not zero.\n");
                fclose(fp);
                fclose(tempFp);
                remove("temp.dat");
                return;
            }

            acc.active = 0; // logical delete
            printf("\nAccount deleted successfully.\n");
        }

        fwrite(&acc, sizeof(struct BankAccount), 1, tempFp);
    }

    fclose(fp);
    fclose(tempFp);

    if (!found)
    {
        printf("\nAccount not found or already inactive.\n");
        remove("temp.dat");
        return;
    }

    remove(AccFileName);
    rename("temp.dat", AccFileName);
}
void viewAllTransactions()
{
    FILE *fp;
    struct Transaction tr;
    int found = 0;

    fp = fopen(TrancFileName, "rb");
    if (fp == NULL)
    {
        printf("\nError opening transaction file.\n");
        return;
    }

    printf("\n---------------------------------------------------------------------------------------------");
    printf("\n%-8s %-10s %-10s %-12s %-10s %-12s %-8s",
           "TrID", "AccNo", "RelAcc", "Type", "Amount", "Date", "Time");
    printf("\n---------------------------------------------------------------------------------------------");

    while (fread(&tr, sizeof(struct Transaction), 1, fp))
    {
        printf("\n%-8ld %-10ld %-10ld %-12s %-10.2lf %-12s %-8s",
               tr.trancId,
               tr.accNum,
               tr.relAcc,
               tr.type,
               tr.Amt,
               tr.date,
               tr.time);

        found = 1;
    }

    printf("\n---------------------------------------------------------------------------------------------");

    fclose(fp);

    if (!found)
    {
        printf("\nNo transactions found.\n");
    }
}
