#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Account {
    int pin;
    float balance;
    float lastTransactions[5];
    int transactionCount;
};

// Global account data
struct Account user = {1234, 10000, {0}, 0};

int login();
void menu();
void checkBalance();
void withdrawCash();
void depositCash();
void changePIN();
void miniStatement();
void saveData();
void loadData();

int login() {
    int pin, attempts = 0;

    while (attempts < 3) {
        printf("\nEnter your PIN: ");
        scanf("%d", &pin);

        if (pin == user.pin) {
            return 1;
        } else {
            attempts++;
            printf("Incorrect PIN! Attempts left: %d\n", 3 - attempts);
        }
    }

    return 0;
}

void checkBalance() {
    printf("\n----------------------------------\n");
    printf("      YOUR CURRENT BALANCE\n");
    printf("----------------------------------\n");
    printf("Balance: ₹%.2f\n", user.balance);
}

void withdrawCash() {
    float amount;

    printf("\nEnter withdrawal amount: ₹");
    scanf("%f", &amount);

    if (amount > user.balance) {
        printf("Insufficient Balance!\n");
    } else if (amount <= 0) {
        printf("Invalid Amount!\n");
    } else {
        user.balance -= amount;

        // Save in mini statement
        user.lastTransactions[user.transactionCount % 5] = -amount;
        user.transactionCount++;

        printf("Withdrawal Successful!\n");
    }
}

void depositCash() {
    float amount;

    printf("\nEnter deposit amount: ₹");
    scanf("%f", &amount);

    if (amount <= 0) {
        printf("Invalid Amount!\n");
    } else {
        user.balance += amount;

        // Save in history
        user.lastTransactions[user.transactionCount % 5] = amount;
        user.transactionCount++;

        printf("Deposit Successful!\n");
    }
}

void changePIN() {
    int oldPIN, newPIN;

    printf("\nEnter current PIN: ");
    scanf("%d", &oldPIN);

    if (oldPIN != user.pin) {
        printf("Incorrect PIN!\n");
        return;
    }

    printf("Enter new PIN: ");
    scanf("%d", &newPIN);

    if (newPIN < 1000 || newPIN > 9999) {
        printf("PIN must be 4 digits!\n");
        return;
    }

    user.pin = newPIN;
    printf("PIN changed successfully!\n");
}

void miniStatement() {
    printf("\n----------------------------------\n");
    printf("         MINI STATEMENT\n");
    printf("----------------------------------\n");

    int start = user.transactionCount - 5;
    if (start < 0) start = 0;

    for (int i = start; i < user.transactionCount; i++) {
        int index = i % 5;
        float amt = user.lastTransactions[index];

        if (amt < 0)
            printf("Withdrawal: ₹%.2f\n", -amt);
        else
            printf("Deposit: ₹%.2f\n", amt);
    }
}

void saveData() {
    FILE *file = fopen("atmdata.txt", "w");
    if (file == NULL) return;

    fprintf(file, "%d %.2f %d\n", user.pin, user.balance, user.transactionCount);

    for (int i = 0; i < 5; i++) {
        fprintf(file, "%.2f ", user.lastTransactions[i]);
    }

    fclose(file);
}

void loadData() {
    FILE *file = fopen("atmdata.txt", "r");
    if (file == NULL) return;

    fscanf(file, "%d %f %d", &user.pin, &user.balance, &user.transactionCount);

    for (int i = 0; i < 5; i++) {
        fscanf(file, "%f", &user.lastTransactions[i]);
    }

    fclose(file);
}

void menu() {
    int choice;

    while (1) {
        printf("\n================ ATM SYSTEM ================\n");
        printf("1. Balance Inquiry\n");
        printf("2. Cash Withdrawal\n");
        printf("3. Cash Deposit\n");
        printf("4. Mini Statement\n");
        printf("5. Change PIN\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: checkBalance(); break;
            case 2: withdrawCash(); break;
            case 3: depositCash(); break;
            case 4: miniStatement(); break;
            case 5: changePIN(); break;
            case 6: saveData(); printf("Thank you!\n"); exit(0);
            default: printf("Invalid option! Try again.\n");
        }
    }
}

int main() {
    loadData();  // load saved data

    printf("=====================================\n");
    printf("         WELCOME TO ATM SYSTEM\n");
    printf("=====================================\n");

    if (!login()) {
        printf("Too many attempts! Card blocked.\n");
        return 0;
    }

    menu();
    return 0;
}

