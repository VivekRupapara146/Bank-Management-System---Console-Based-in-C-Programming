#include "banking.h"
#include "customer.h"

void customerMenu(struct Session *session)
{
    int choice;

    do
    {
        printf("\n======== CUSTOMER MENU ========");
        printf("\n1. View Account Details");
        printf("\n2. Deposit Money");
        printf("\n3. Withdraw Money");
        printf("\n4. Transfer Money");
        printf("\n5. View Transaction History");
        printf("\n6. Logout");
        printf("\nEnter choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            viewAccountDetails(session);
            break;
        case 2:
            depositMoney(session);
            break;
        case 3:
            withdrawMoney(session);
            break;
        case 4:
            transferMoney(session);
            break;
        case 5:
            viewMyTransactions(session);
            break;
        case 6:
            printf("\nCustomer Logged Out.\n");
            session->isLoggedIn = 0;
            session->role = 0;
            session->activeAccount = 0;
            printf("\nYou have been safely logged out.\n");
            break;

        default:
            break;
        }
    } while (choice != 6);
}
void viewAccountDetails(struct Session *session)
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

    while (fread(&acc, sizeof(struct BankAccount), 1, fp))
    {
        if (acc.accNum == session->activeAccount && acc.active == 1)
        {
            printf("\n========= ACCOUNT DETAILS =========");
            printf("\nAccount Number : %ld", acc.accNum);
            printf("\nName           : %s", acc.name);
            printf("\nAccount Type   : %s", acc.accType);
            printf("\nBalance        : %.2lf", acc.bal);
            printf("\nStatus         : Active");
            printf("\n=================================\n");

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
void depositMoney(struct Session *session)
{
    FILE *fp, *tempFp, *trFp;
    struct BankAccount acc;
    struct Transaction tr;
    double amt;
    int found = 0;

    printf("\nEnter amount to deposit: ");
    scanf("%lf", &amt);

    if (amt <= 0)
    {
        printf("\nInvalid amount. Deposit must be positive.\n");
        return;
    }

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
        if (acc.accNum == session->activeAccount && acc.active == 1)
        {
            acc.bal += amt;
            found = 1;

            /* -------- Transaction Log -------- */
            trFp = fopen(TrancFileName, "ab");
            if (trFp != NULL)
            {
                tr.trancId = (long)time(NULL); // simple unique id
                tr.accNum = acc.accNum;
                tr.relAcc = 0;
                strcpy(tr.type, "DEPOSIT");
                tr.Amt = amt;

                strcpy(tr.date, __DATE__);
                strcpy(tr.time, __TIME__);

                fwrite(&tr, sizeof(struct Transaction), 1, trFp);
                fclose(trFp);
            }

            printf("\nDeposit successful.");
            printf("\nUpdated Balance: %.2lf\n", acc.bal);
        }

        fwrite(&acc, sizeof(struct BankAccount), 1, tempFp);
    }

    fclose(fp);
    fclose(tempFp);

    if (!found)
    {
        printf("\nAccount not found or inactive.\n");
        remove("temp.dat");
        return;
    }

    remove(AccFileName);
    rename("temp.dat", AccFileName);
}
void withdrawMoney(struct Session *session)
{
    FILE *fp, *tempFp, *trFp;
    struct BankAccount acc;
    struct Transaction tr;
    double amt;
    int found = 0;

    printf("\nEnter amount to withdraw: ");
    scanf("%lf", &amt);

    if (amt <= 0)
    {
        printf("\nInvalid amount. Withdrawal must be positive.\n");
        return;
    }

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
        if (acc.accNum == session->activeAccount && acc.active == 1)
        {
            found = 1;

            if (acc.bal < amt)
            {
                printf("\nInsufficient balance.\n");
                fclose(fp);
                fclose(tempFp);
                remove("temp.dat");
                return;
            }

            acc.bal -= amt;

            /* -------- Transaction Log -------- */
            trFp = fopen(TrancFileName, "ab");
            if (trFp != NULL)
            {
                tr.trancId = (long)time(NULL); // simple unique id
                tr.accNum = acc.accNum;
                tr.relAcc = 0;
                strcpy(tr.type, "WITHDRAW");
                tr.Amt = amt;

                strcpy(tr.date, __DATE__);
                strcpy(tr.time, __TIME__);

                fwrite(&tr, sizeof(struct Transaction), 1, trFp);
                fclose(trFp);
            }

            printf("\nWithdrawal successful.");
            printf("\nRemaining Balance: %.2lf\n", acc.bal);
        }

        fwrite(&acc, sizeof(struct BankAccount), 1, tempFp);
    }

    fclose(fp);
    fclose(tempFp);

    if (!found)
    {
        printf("\nAccount not found or inactive.\n");
        remove("temp.dat");
        return;
    }

    remove(AccFileName);
    rename("temp.dat", AccFileName);
}
void viewMyTransactions(struct Session *session)
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
    printf("\n%-8s %-10s %-10s %-12s %-10s %-12s %-10s",
           "TrID", "AccNo", "RelAcc", "Type", "Amount", "Date", "Time");
    printf("\n---------------------------------------------------------------------------------------------");

    while (fread(&tr, sizeof(struct Transaction), 1, fp))
    {
        if (tr.accNum == session->activeAccount ||
            tr.relAcc == session->activeAccount)
        {
            printf("\n%-8ld %-10ld %-10ld %-12s %-10.2lf %-12s %-10s",
                   tr.trancId,
                   tr.accNum,
                   tr.relAcc,
                   tr.type,
                   tr.Amt,
                   tr.date,
                   tr.time);

            found = 1;
        }
    }

    printf("\n---------------------------------------------------------------------------------------------");

    fclose(fp);

    if (!found)
    {
        printf("\nNo transactions found for your account.\n");
    }
}
void transferMoney(struct Session *session)
{
    FILE *fp, *tempFp, *trFp;
    struct BankAccount acc;
    struct Transaction tr;

    long senderAcc = session->activeAccount;
    long receiverAcc;
    double amt;

    int senderFound = 0;
    int receiverFound = 0;

    printf("\nEnter Receiver Account Number: ");
    scanf("%ld", &receiverAcc);

    if (receiverAcc == senderAcc)
    {
        printf("\nCannot transfer to the same account.\n");
        return;
    }

    printf("Enter Transfer Amount: ");
    scanf("%lf", &amt);

    if (amt <= 0)
    {
        printf("\nInvalid amount.\n");
        return;
    }

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

    /* ---------- Process Accounts ---------- */
    while (fread(&acc, sizeof(struct BankAccount), 1, fp))
    {
        /* Sender */
        if (acc.accNum == senderAcc && acc.active == 1)
        {
            senderFound = 1;

            if (acc.bal < amt)
            {
                printf("\nInsufficient balance.\n");
                fclose(fp);
                fclose(tempFp);
                remove("temp.dat");
                return;
            }

            acc.bal -= amt;
        }
        /* Receiver */
        else if (acc.accNum == receiverAcc && acc.active == 1)
        {
            receiverFound = 1;
            acc.bal += amt;
        }

        fwrite(&acc, sizeof(struct BankAccount), 1, tempFp);
    }

    fclose(fp);
    fclose(tempFp);

    if (!senderFound || !receiverFound)
    {
        printf("\nSender or receiver account not found / inactive.\n");
        remove("temp.dat");
        return;
    }

    /* ---------- Commit Changes ---------- */
    remove(AccFileName);
    rename("temp.dat", AccFileName);

    /* ---------- Log Transaction ---------- */
    trFp = fopen(TrancFileName, "ab");
    if (trFp != NULL)
    {
        tr.trancId = (long)time(NULL);
        tr.accNum = senderAcc;
        tr.relAcc = receiverAcc;
        strcpy(tr.type, "TRANSFER");
        tr.Amt = amt;
        strcpy(tr.date, __DATE__);
        strcpy(tr.time, __TIME__);

        fwrite(&tr, sizeof(struct Transaction), 1, trFp);
        fclose(trFp);
    }

    printf("\nTransfer successful.");
    printf("\nAmount %.2lf transferred to account %ld\n", amt, receiverAcc);
}