#include "banking.h"
#include "auth.h"

int login(int expectedRole, struct Session *session)
{
    FILE *fp;
    struct User user;
    char uName[50], pass[30];
    int attempts = 3, found;

    while (attempts > 0)
    {
        found = 0;
        printf("\nUsername: ");
        scanf("%s", uName);
        printf("\nPassword: ");
        scanf("%s", pass);

        fp = fopen(UserFileName, "rb");
        if (fp == NULL)
        {
            printf("\nError opening file.");
        }

        while (fread(&user, sizeof(struct User), 1, fp))
        {
            if (strcmp(user.uName, uName) == 0 && strcmp(user.pass, pass) == 0 && user.role == expectedRole)
            {
                session->isLoggedIn = 1;
                session->role = user.role;
                session->activeAccount = user.linkedAcc;

                fclose(fp);
                return 1;
            }
        }
        fclose(fp);
        attempts--;
        printf("\nInvalid creds. Attempts left: %d", attempts);
    }
    printf("\nLogin Failed. Returning to main menu.");
    return 0;
}

void ensureDefaultAdmin()
{
    FILE *fp;
    struct User user;
    int adminFound = 0;

    fp = fopen(UserFileName, "rb");
    if (fp == NULL)
        return;

    while (fread(&user, sizeof(struct User), 1, fp))
    {
        if (user.role == ROLE_ADMIN)
        {
            adminFound = 1;
            break;
        }
    }
    fclose(fp);

    if (!adminFound)
    {
        fp = fopen(UserFileName, "ab");
        if (fp == NULL)
        {
            printf("Error creating default admin.\n");
            exit(1);
        }

        strcpy(user.uName, "admin");
        strcpy(user.pass, "admin123");
        user.role = ROLE_ADMIN;
        user.linkedAcc = 0;

        fwrite(&user, sizeof(struct User), 1, fp);
        fclose(fp);

        printf("\n[Default Admin Created]");
        printf("\nUsername: admin");
        printf("\nPassword: admin123\n");
    }
}