#include "banking.h"
#include "admin.h"
#include "customer.h"
#include "auth.h"
#include "file_utils.h"
void showMenu();
int main()
{
    int choice;
    struct Session session = {0};

    initializeFile(AccFileName);
    initializeFile(TrancFileName);
    initializeFile(UserFileName);
    ensureDefaultAdmin();
    printf("\n\n===== BANK MANAGEMENT SYSTEM =====");
    while (1)
    {
        showMenu();
        printf("\nEnter the choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printf("\n[Admin Login Selected]");
            // login(ROLE_ADMIN, &session);

            if (login(ROLE_ADMIN, &session))
            {
                printf("\nAdmin login Successful.");
                adminMenu(&session);
            }
            else
            {
                printf("Admin login failed.\n");
            }
            break;

        case 2: /* Customer Login */
            printf("\n[Customer Login Selected]\n");

            if (login(ROLE_CUSTOMER, &session))
            {
                printf("Customer login successful.\n");
                customerMenu(&session); // will be added later */
            }
            else
            {
                printf("Customer login failed.\n");
            }
            break;

        case 3:
            printf("\nExiting Bank Management System. Thanks for Using our Applicataion.\n");
            return 0;

        default:
            printf("\nInvalid choice. Please try again.\n");
        }
    }

    return 0;
}
void showMenu()
{
    printf("\n----------------------------------");
    printf("\n1. Admin Login");
    printf("\n2. Customer Login");
    printf("\n3. Exit");
    printf("\n----------------------------------\n");
}