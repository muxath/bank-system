#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100

struct Account {
    char name[100];
    int accountNumber;
    char password[20];
    float balance;
};

struct Account users[MAX_USERS];
int userCount = 0;

void saveToFile() {
    FILE *file = fopen("accounts.txt", "w");
    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%s %d %s %.2f\n", users[i].name, users[i].accountNumber, users[i].password, users[i].balance);
    }
    fclose(file);
}

void loadFromFile() {
    FILE *file = fopen("accounts.txt", "r");
    if (!file) return;
    while (fscanf(file, "%s %d %s %f", users[userCount].name, &users[userCount].accountNumber, users[userCount].password, &users[userCount].balance) != EOF) {
        userCount++;
    }
    fclose(file);
}

int findAccount(int accNum, char password[]) {
    for (int i = 0; i < userCount; i++) {
        if (users[i].accountNumber == accNum && strcmp(users[i].password, password) == 0) {
            return i;
        }
    }
    return -1;
}

void createAccount() {
    printf("\n--- Create a New Account ---\n");
    if (userCount >= MAX_USERS) {
        printf("Sorry, cannot create more accounts.\n");
        return;
    }
    printf("Enter user name: ");
    scanf("%s", users[userCount].name);
    printf("Enter account number: ");
    scanf("%d", &users[userCount].accountNumber);
    printf("Enter password: ");
    scanf("%s", users[userCount].password);
    users[userCount].balance = 0;
    userCount++;
    saveToFile();
    printf("Account created successfully!\n");
}

void deposit(int index) {
    float amount;
    printf("Enter amount to deposit: ");
    scanf("%f", &amount);
    users[index].balance += amount;
    saveToFile();
    printf("%.2f deposited successfully!\n", amount);
}

void transfer(int index) {
    int targetAcc;
    float amount;
    printf("Enter recipient account number: ");
    scanf("%d", &targetAcc);
    printf("Enter amount to transfer: ");
    scanf("%f", &amount);

    int targetIndex = -1;
    for (int i = 0; i < userCount; i++) {
        if (users[i].accountNumber == targetAcc) {
            targetIndex = i;
            break;
        }
    }

    if (targetIndex == -1) {
        printf("Account not found.\n");
    } else if (users[index].balance < amount) {
        printf("Insufficient balance.\n");
    } else {
        users[index].balance -= amount;
        users[targetIndex].balance += amount;
        saveToFile();
        printf("Transfer successful.\n");
    }
}

void checkBalance(int index) {
    printf("Current balance: %.2f\n", users[index].balance);
}

void deleteAccount() {
    int accNum;
    char password[20];
    printf("Enter account number: ");
    scanf("%d", &accNum);
    printf("Enter password: ");
    scanf("%s", password);

    int index = findAccount(accNum, password);
    if (index == -1) {
        printf("Account not found or incorrect password.\n");
        return;
    }

    if (users[index].balance > 0) {
        printf("Account must have zero balance to be deleted.\n");
        return;
    }

    for (int i = index; i < userCount - 1; i++) {
        users[i] = users[i + 1];
    }
    userCount--;
    saveToFile();
    printf("Account deleted successfully.\n");
}

void login() {
    int accNum;
    char password[20];
    printf("\n--- Login Page ---\n");
    printf("Enter account number: ");
    scanf("%d", &accNum);
    printf("Enter password: ");
    scanf("%s", password);

    int index = findAccount(accNum, password);
    if (index == -1) {
        printf("Incorrect login details.\n");
        return;
    }

    printf("\nLogin successful!\n");
    int choice;
    do {
        printf("\nWhat would you like to do?\n1. Deposit\n2. Transfer\n3. Check balance\n4. Logout\nChoose: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: deposit(index); break;
            case 2: transfer(index); break;
            case 3: checkBalance(index); break;
        }
    } while (choice != 4);
}

int main() {
    loadFromFile();
    int choice;
    do {
        printf("\n----------- Welcome to Our Bank -----------\n");
        printf("1. Create Account\n2. Login\n3. Delete Account\n4. Exit\nChoose: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: createAccount(); break;
            case 2: login(); break;
            case 3: deleteAccount(); break;
        }
    } while (choice != 4);

    printf("\nThank you for using our banking system!\n");
    return 0;
}