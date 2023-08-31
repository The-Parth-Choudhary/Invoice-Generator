#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct items
{
    char item[20];
    float price;
    int qty;
};

struct orders
{
    char customer[50];
    char date[50];
    int noOfItem;
    struct items itm[50];
};

void generateBillHeader(char name[50], char date[30])
{
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
    printf("\n---------------------------------------");
    printf("\n\n");
}

void generateBillBody(char item[30], int qty, float price)
{
    printf("%s\t\t", item);
    printf("%d\t\t", qty);
    printf("%.2f\t\t", qty * price);
    printf("\n");
}

void generateBillFooter(float total)
{
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

int main()
{
    int opt;
    int n;
    struct orders ord;
    struct orders order;
    char name[50];

    char saveBill = 'y';
    char contFlag = 'y';
    FILE *fp;

    while (contFlag == 'y')
    {
        // to clear console
        system("cls");

        float total = 0;
        int invoiceFound = 0;

        // Generating Dashboard
        printf("\t============DA DHABA============");
        printf("\n\nPlease select your prefered operation:\t");
        printf("\n\n1. Generate Invoice");
        printf("\n2. Show all Invoices");
        printf("\n3. Search Invoice");
        printf("\n4. Exit");

        printf("\n\nYour choice:\t");
        scanf("%d", &opt);
        fgetc(stdin);

        switch (opt)
        {
        case 1:
            system("cls");

            // Taking Input
            printf("\nPlease enter the name of the customer:\t");
            fgets(ord.customer, 50, stdin);
            ord.customer[strlen(ord.customer) - 1] = 0;
            strcpy(ord.date, __DATE__);
            printf("\nPlease enter number of items:\t");
            scanf("%d", &n);
            ord.noOfItem = n;

            for (int i = 0; i < n; i++)
            {
                fgetc(stdin);
                printf("\n\n");
                printf("Please enter the item %d:\t", i + 1);
                fgets(ord.itm[i].item, 20, stdin);
                ord.itm[i].item[strlen(ord.itm[i].item) - 1] = 0;
                printf("Please enter the quantity:\t");
                scanf("%d", &ord.itm[i].qty);
                if (!strcmp(ord.itm[i].item, "Pizza"))
                {
                    ord.itm[i].price = 250;
                }
                else if(!strcmp(ord.itm[i].item, "Burgur")){
                    ord.itm[i].price = 75;
                }
                else if(!strcmp(ord.itm[i].item, "Coffee")){
                    ord.itm[i].price = 150;
                }
                else if(!strcmp(ord.itm[i].item, "Fries")){
                    ord.itm[i].price = 100;
                }
                else{
                    printf("Sorry for inconvinence but %s is not available", ord.itm[i].item);
                    ord.itm[i].price = 0;
                }

                total += ord.itm[i].qty * ord.itm[i].price;
            }

            // Generating Bill
            generateBillHeader(ord.customer, ord.date);
            for (int i = 0; i < ord.noOfItem; i++)
            {
                generateBillBody(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
            }

            generateBillFooter(total);

            // Saving Bill
            printf("\nDo you want to save the invoice [y/n]:\t");
            scanf("%s", &saveBill);
            if (saveBill == 'y')
            {
                fp = fopen("Bill.txt", "ab+");
                fwrite(&ord, sizeof(struct orders), 1, fp);
                if (fwrite != 0)
                {
                    printf("\nYour invoice is successfully saved.");
                }
                else
                {
                    printf("\nErroe occurred during saving!");
                }
                fclose(fp);
            }

            break;

        case 2:
            system("cls");
            fp = fopen("Bill.txt", "rb");
            printf("\n   *****Your Previous Invoices*****\n");

            if (fp == NULL)
            {
                perror("Error opening file");
                break;
            }

            // Reading File
            while (fread(&order, sizeof(struct orders), 1, fp))
            {
                total = 0;
                generateBillHeader(order.customer, order.date);

                for (int i = 0; i < order.noOfItem; i++)
                {
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

            // Reading File
            while (fread(&order, sizeof(struct orders), 1, fp))
            {
                total = 0;
                if (!strcmp(order.customer, name))
                {
                    generateBillHeader(order.customer, order.date);

                    for (int i = 0; i < order.noOfItem; i++)
                    {
                        generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                        total += order.itm[i].qty * order.itm[i].price;
                    }

                    generateBillFooter(total);

                    invoiceFound = 1;
                }
            }

            if (!invoiceFound)
            {
                printf("\nSorry the invoice for %s is not available\n", name);
            }

            fclose(fp);

            break;

        case 4:
            printf("\n\tHave a nice day Sir/Mam \n\n");
            exit(0);

        default:
            printf("Sorry invalid option");
            break;
        }

        printf("\nDo you want to perform another operation? [y/n]:\t");
        scanf("%s", &contFlag);

        system("cls");
    }

    printf("\n\n");

    return 0;
}
