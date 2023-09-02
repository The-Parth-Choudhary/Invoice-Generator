#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct items
{
    char item[20];
    float price;
    int qty;
};

// Function to load menu data from "menu.txt" or create it if it doesn't exist
void loadMenu(struct items menu[], int *menuSize)
{
    FILE *menuFile = fopen("menu.txt", "rb");
    if (menuFile == NULL)
    {
        // If the file doesn't exist, create an empty one
        menuFile = fopen("menu.txt", "wb");
        if (menuFile == NULL)
        {
            printf("Error creating menu file.\n");
            exit(1);
        }
        fclose(menuFile);
        return;
    }

    *menuSize = 0;

    while (*menuSize < 50 && fread(&menu[*menuSize], sizeof(struct items), 1, menuFile) == 1)
    {
        (*menuSize)++;
    }

    fclose(menuFile);
}

// Function to save menu data to "menu.txt"
void saveMenu(struct items menu[], int menuSize)
{
    FILE *menuFile = fopen("menu.txt", "wb");
    if (menuFile == NULL)
    {
        printf("Error opening menu file for saving.\n");
        exit(1);
    }

    for (int i = 0; i < menuSize; i++)
    {
        fwrite(&menu[i], sizeof(struct items), 1, menuFile);
    }

    fclose(menuFile);
}

// Function to display menu items and their prices
void displayMenu(struct items menu[], int menuSize)
{
    printf("Menu Items:\n");
    printf("--------------------------------------------------\n");
    printf("Item\t\tPrice\n");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < menuSize; i++)
    {
        printf("%s\t\t%.2f\n", menu[i].item, menu[i].price);
    }
    printf("--------------------------------------------------\n");
}

// Function to add a new item to the menu
void addItem(struct items menu[], int *menuSize)
{
    if (*menuSize >= 50)
    {
        printf("Menu is full. Cannot add more items.\n");
        return;
    }

    printf("Enter the name of the new item: ");
    scanf("%s", menu[*menuSize].item);

    printf("Enter the price of the new item: ");
    scanf("%f", &menu[*menuSize].price);

    (*menuSize)++;
    saveMenu(menu, *menuSize);

    printf("Item added to the menu successfully.\n");
}

// Function to update the price of an existing item
void updateItem(struct items menu[], int menuSize)
{
    char itemName[20];
    printf("Enter the name of the item to update: ");
    scanf("%s", itemName);

    for (int i = 0; i < menuSize; i++)
    {
        if (strcmp(menu[i].item, itemName) == 0)
        {
            printf("Enter the new price for %s: ", itemName);
            scanf("%f", &menu[i].price);
            saveMenu(menu, menuSize);
            printf("Price updated successfully.\n");
            return;
        }
    }

    printf("Item not found in the menu.\n");
}

// Function to delete an item from the menu
void deleteItem(struct items menu[], int *menuSize)
{
    char itemName[20];
    printf("Enter the name of the item to delete: ");
    scanf("%s", itemName);

    for (int i = 0; i < *menuSize; i++)
    {
        if (strcmp(menu[i].item, itemName) == 0)
        {
            for (int j = i; j < *menuSize - 1; j++)
            {
                strcpy(menu[j].item, menu[j + 1].item);
                menu[j].price = menu[j + 1].price;
            }
            (*menuSize)--;
            saveMenu(menu, *menuSize);
            printf("Item deleted successfully.\n");
            return;
        }
    }

    printf("Item not found in the menu.\n");
}

int main()
{
    system("cls");
    int menuSize = 0;
    struct items menu[50];
    int choice;

    // Load menu data from "menu.txt" or create it if it doesn't exist
    loadMenu(menu, &menuSize);

    while (1)
    {
        printf("\nMenu Management System\n");
        printf("1. Display Menu\n");
        printf("2. Add Item\n");
        printf("3. Update Item Price\n");
        printf("4. Delete Item\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            system("cls");
            displayMenu(menu, menuSize);
            break;
        case 2:
            system("cls");
            addItem(menu, &menuSize);
            break;
        case 3:
            system("cls");
            updateItem(menu, menuSize);
            break;
        case 4:
            system("cls");
            displayMenu(menu, menuSize);
            deleteItem(menu, &menuSize);
            break;
        case 5:
            printf("Exiting the Menu Management System.\n");
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
