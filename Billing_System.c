#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct items {
    char item[20];
    float price;
    int qty;
};

struct orders {
    char customer[50];
    char date[50];
    int noOfItem;
    struct items itm[50];
};

// Function to load menu data from "menu.txt"
// Function to load menu data from "menu.txt" (format used by code 1)
void loadMenu(struct items menu[], int *menuSize) {
    FILE *menuFile = fopen("menu.txt", "rb");
    if (menuFile == NULL) {
        printf("Error opening menu file.\n");
        exit(1);
    }

    *menuSize = 0;

    while (*menuSize < 50 && fread(&menu[*menuSize], sizeof(struct items), 1, menuFile) == 1) {
        (*menuSize)++;
    }

    fclose(menuFile);
}


// Function to check if an item is available in the menu
int isItemAvailable(struct items menu[], int menuSize, const char *itemName) {
    for (int i = 0; i < menuSize; i++) {
        if (strcmp(menu[i].item, itemName) == 0) {
            return i; // Return the index of the item if found
        }
    }
    return -1; // Return -1 if the item is not found
}

void generateBillHeader(char name[50], char date[30]) {
    printf("\n\n");
    printf("\t\tDA DHABA");
    printf("\n\t   -----------------");
    printf("\nDate: %s", date);
    printf("\nInvoice To: %s", name);
    printf("\n");
    printf("---------------------------------------\n");
    printf("Items\t\t");
    printf("Qty\t\t");
    printf("Total\t\t");
    printf("\n---------------------------------------\n");
}

void generateBillBody(char item[30], int qty, float price) {
    printf("%s\t\t", item);
    printf("%d\t\t", qty);
    printf("%.2f\t\t", qty * price);
    printf("\n");
}

void generateBillFooter(float total) {
    printf("\n");
    float dis = 0.1 * total;
    float netTotal = total - dis;
    float cgst = 0.08 * netTotal, grandTotal = netTotal + cgst;
    printf("---------------------------------------\n");
    printf("Sub Total\t\t\t%.2f", total);
    printf("\nDiscount @10%s\t\t\t%.2f", "%", dis);
    printf("\n\t\t\t\t-------");
    printf("\nNet Total\t\t\t%.2f", netTotal);
    printf("\nGST @8%s\t\t\t\t%.2f", "%", cgst);
    printf("\n---------------------------------------");
    printf("\nGrand Total\t\t\t%.2f", grandTotal);
    printf("\n---------------------------------------\n");
}

int main() {
    int opt;
    int n;
    struct orders ord;
    struct orders order;
    char name[50];

    char saveBill = 'y';
    char contFlag = 'y';
    FILE *fp;

    struct items menu[50];
    int menuSize = 0;

    // Load menu data from "menu.txt"
    loadMenu(menu, &menuSize);

    while (contFlag == 'y') {
        system("cls");

        float total = 0;
        int invoiceFound = 0;
        int invoiceGenerated = 0;

        printf("\t============DA DHABA============");
        printf("\n\nPlease select your preferred operation:\t");
        printf("\n\n1. Generate Invoice");
        printf("\n2. Show all Invoices");
        printf("\n3. Search Invoice");
        printf("\n4. Exit");

        printf("\n\nYour choice:\t");
        scanf("%d", &opt);
        fgetc(stdin);

        switch (opt) {
            case 1:
                system("cls");

                printf("\nPlease enter the name of the customer:\t");
                fgets(ord.customer, 50, stdin);
                ord.customer[strlen(ord.customer) - 1] = 0;
                strcpy(ord.date, __DATE__);
                printf("\nPlease enter number of items:\t");
                scanf("%d", &n);
                ord.noOfItem = n;

                for (int i = 0; i < n; i++) {
                    fgetc(stdin);
                    printf("\n\n");
                    printf("Please enter the item %d:\t", i + 1);
                    fgets(ord.itm[i].item, 20, stdin);
                    ord.itm[i].item[strlen(ord.itm[i].item) - 1] = 0;

                    // Check if the entered item is available
                    int itemIndex = isItemAvailable(menu, menuSize, ord.itm[i].item);
                    if (itemIndex != -1) {
                        printf("Please enter the quantity:\t");
                        scanf("%d", &ord.itm[i].qty);
                        ord.itm[i].price = menu[itemIndex].price;
                        total += ord.itm[i].qty * ord.itm[i].price;
                    } else {
                        printf("Item not found in the menu. Invoice will not be generated for this item.\n");
                        ord.itm[i].qty = 0;
                        ord.itm[i].price = 0;
                    }
                }

                // Check if the invoice is empty
                for (int i = 0; i < n; i++) {
                    if (ord.itm[i].qty > 0) {
                        invoiceGenerated = 1;
                        break;
                    }
                }

                if (invoiceGenerated) {
                    generateBillHeader(ord.customer, ord.date);
                    for (int i = 0; i < ord.noOfItem; i++) {
                        if (ord.itm[i].qty > 0) {
                            generateBillBody(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
                        }
                    }

                    generateBillFooter(total);

                    printf("\nDo you want to save the invoice [y/n]:\t");
                    scanf(" %c", &saveBill);
                    if ((saveBill == 'y' || saveBill == 'Y') && invoiceGenerated) {
                        fp = fopen("Bill.txt", "ab+");
                        fwrite(&ord, sizeof(struct orders), 1, fp);
                        if (fwrite != 0) {
                            printf("\nYour invoice is successfully saved.\n");
                        } else {
                            printf("\nError occurred during saving!\n");
                        }
                        fclose(fp);
                    }
                } else {
                    printf("\nNo items in the invoice. Invoice will not be generated.\n");
                }

                break;

            case 2:
                system("cls");
                fp = fopen("Bill.txt", "rb");
                printf("\n   *****Your Previous Invoices*****\n");

                if (fp == NULL) {
                    perror("Error opening file");
                    break;
                }

                while (fread(&order, sizeof(struct orders), 1, fp)) {
                    total = 0;
                    generateBillHeader(order.customer, order.date);

                    for (int i = 0; i < order.noOfItem; i++) {
                        generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                        total += order.itm[i].qty * order.itm[i].price;
                    }

                    generateBillFooter(total);
                }

                fclose(fp);

                break;

            case 3:
                printf("\nEnter the name of the customer:\t");
                fgets(name, 50, stdin);
                name[strlen(name) - 1] = 0;
                system("cls");
                fp = fopen("Bill.txt", "rb");

                printf("\t*****Previous Invoice of %s*****", name);

                while (fread(&order, sizeof(struct orders), 1, fp)) {
                    total = 0;
                    if (!strcmp(order.customer, name)) {
                        generateBillHeader(order.customer, order.date);

                        for (int i = 0; i < order.noOfItem; i++) {
                            generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                            total += order.itm[i].qty * order.itm[i].price;
                        }

                        generateBillFooter(total);

                        invoiceFound = 1;
                    }
                }

                if (!invoiceFound) {
                    printf("\nSorry, the invoice for %s is not available\n", name);
                }

                fclose(fp);

                break;

            case 4:
                printf("\n\tHave a nice day Sir/Madam \n\n");
                exit(0);

            default:
                printf("Sorry invalid option\n");
                break;
        }

        printf("\nDo you want to perform another operation? [y/n]:\t");
        scanf(" %c", &contFlag);

        system("cls");
    }

    printf("\n\n");

    return 0;
}
