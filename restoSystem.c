#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h> // For getch function
#include <unistd.h> // For sleep function

// Define the FoodData structure
typedef struct FoodData {
    char foodName[31];
    char foodCategory[11];
    int foodPrice;
    int foodStock;
    struct FoodData* prev;
    struct FoodData* next;
} FoodData;

typedef struct BasketItem
{
    char foodName[31];
    int quantity;
    int price;
    struct BasketItem *next;
} BasketItem;

void LargeTitleRestaurant()
{
    printf("=======================================================================\n");
    printf("|   _        _     _    __      __    _    __     _       _           |\n");
    printf("|  | |      | |   | |  |  \\    /  |  | |  |  \\   | |     / \\          |\n");
    printf("|  | |      | |   | |  |   \\  /   |  | |  |   \\  | |    / _ \\         |\n");
    printf("|  | |      | |   | |  | |\\ \\/ /| |  | |  | |\\ \\ | |   / /_\\ \\        |\n");
    printf("|  | |__    | |___| |  | | \\  / | |  | |  | | \\ \\| |  / /   \\ \\       |\n");
    printf("|  |____|   |_______|  | |  \\/  |_|  |_|  |_|  \\___| /_/     \\_\\      |\n");
    printf("|                                                                     |\n");
    printf("|       ____      _     __     _     _     __     _     ______        |\n");
    printf("|      |  __ \\   | |   |  \\   | |   | |   |  \\   | |   / _____\\       |\n");
    printf("|      | |  | |  | |   |   \\  | |   | |   |   \\  | |  |  |  ___       |\n");
    printf("|      | |  | |  | |   | |\\ \\ | |   | |   | |\\ \\ | |  |  | |_ |       |\n");
    printf("|      | |__| |  | |   | | \\ \\| |   | |   | | \\ \\| |  |  |__| |       |\n");
    printf("|      |_____/   |_|   |_|  \\___|   |_|   |_|  \\___|   \\______/       |\n");
    printf("=======================================================================\n");
}

void LargeTitleMenu()
{
    printf("============================================================================\n");
    printf("|                                 MENU                                     |\n");
    printf("|  ___  ______     _       _____        ____    _   _____   ___    ____    |\n");
    printf("| |  _||__  __|   / \\     |  _  \\      |  _ \\  | | |__ __| |  _|  | ___|   |\n");
    printf("| | |_    | |    / _ \\    | |_| |      | |)  | | |   | |   | |_   | |__    |\n");
    printf("| |_  |   | |   / /_\\ \\   |  _  /      |  _ <  | |   | |   |  _|  |__  |   |\n");
    printf("|  _| |   | |  / /   \\ \\  | | \\ \\      | |)  | | |   | |   | |_    __| |   |\n");
    printf("| |___/   |_| /_/     \\_\\ |_|  |_|     |____/  |_|   |_|   |___|  |____|   |\n");
    printf("|                                                                          |\n");
    printf("|                     Taste the Stars in Every Bite!                       |\n");
    printf("|                                                                          |\n");
    printf("============================================================================\n");
}

// Function to create a new node
FoodData* createNode(const char* name, const char* category, int price, int stock) {
    FoodData* newNode = (FoodData*)malloc(sizeof(FoodData));
    if (!newNode) {
        printf("Memory allocation failed\n");
        sleep(3);
        exit(1);
    }
    strncpy(newNode->foodName, name, 30);
    newNode->foodName[30] = '\0';  // Null terminate
    strncpy(newNode->foodCategory, category, 10);
    newNode->foodCategory[10] = '\0';  // Null terminate
    newNode->foodPrice = price;
    newNode->foodStock = stock;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

// Insert at the end
FoodData* insertAtEnd(FoodData* head, const char* name, const char* category, int price, int stock) {
    FoodData* newNode = createNode(name, category, price, stock);
    if (head == NULL) {
        return newNode;
    }
    FoodData* temp = head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
    newNode->prev = temp;
    return head;
}

// Delete a node by food name
FoodData* deleteNode(FoodData* head, const char* name) {
    FoodData* temp = head;

    // Find the node with the given food name
    while (temp != NULL && strcmp(temp->foodName, name) != 0) {
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Food item '%s' not found\n", name);
        sleep(3);
        return head;
    }

    // Adjust pointers
    if (temp->prev != NULL) {
        temp->prev->next = temp->next;
    } else {
        head = temp->next;  // If head is being deleted
    }
    if (temp->next != NULL) {
        temp->next->prev = temp->prev;
    }

    free(temp);  // Free the memory of the deleted node
    printf("Food item '%s' deleted successfully.\n", name);
    sleep(3);
    return head;
}

// Load data from file into the doubly linked list
FoodData* loadFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file: %s\n", filename);
        sleep(3);
        return NULL;
    }

    FoodData* head = NULL;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;

        // Parse the line
        char foodName[31];
        char foodCategory[11];
        int foodPrice;
        int foodStock;
        if (sscanf(line, "%30[^,],%10[^,],%d,%d", foodName, foodCategory, &foodPrice, &foodStock) == 4) {
            head = insertAtEnd(head, foodName, foodCategory, foodPrice, foodStock);
        } else {
            printf("Invalid line format: %s\n", line);
            sleep(3);
        }
    }

    fclose(file);
    return head;
}

// Save the linked list back to the file
void saveToFile(const char* filename, FoodData* head) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open file for writing: %s\n", filename);
        sleep(3);
        return;
    }

    FoodData* temp = head;
    while (temp != NULL) {
        fprintf(file, "%s,%s,%d,%d\n", temp->foodName, temp->foodCategory, temp->foodPrice, temp->foodStock);
        temp = temp->next;
    }

    fclose(file);
}

// Edit a node by food name
FoodData* editNode(FoodData* head, const char* name, const char* newCategory, int newPrice, int newStock) {
    FoodData* temp = head;

    // Find the node with the given food name
    while (temp != NULL && strcmp(temp->foodName, name) != 0) {
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Food item '%s' not found\n", name);
        sleep(3);
        return head;
    }

    // Update the node
    strncpy(temp->foodCategory, newCategory, 10);
    temp->foodCategory[10] = '\0';  // Null terminate
    temp->foodPrice = newPrice;
    temp->foodStock = newStock;

    printf("Food item '%s' updated successfully.\n", name);
    sleep(3);
    return head;
}

// Display the list in forward direction
void displayForward(FoodData* head) {
    FoodData* temp = head;
    printf("Food List (Forward):\n");
    while (temp != NULL) {
        printf("Name: %s, Category: %s, Price: %d, Stock: %d\n",
                temp->foodName, temp->foodCategory, temp->foodPrice, temp->foodStock);

        temp = temp->next;
    }
    printf("\n");
}

// Function to count the number of items in the menu
int getMenuSize(FoodData *head)
{
    int count = 0;
    while (head != NULL)
    {
        count++;
        head = head->next;
    }
    return count;
}

// Function to convert linked list to an array
FoodData **convertMenuToArray(FoodData *head)
{
    int size = getMenuSize(head);
    FoodData **menuArray = (FoodData **)malloc(size * sizeof(FoodData *));

    int index = 0;
    while (head != NULL)
    {
        menuArray[index++] = head;
        head = head->next;
    }

    return menuArray;
}

// Function to sort menu array based on user choice
void sortMenu(FoodData **menu, int size, int sortBy)
{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            int compare = 0;
            if (sortBy == 1)
            { // Sort by Name
                compare = strcmp(menu[i]->foodName, menu[j]->foodName);
            }
            else if (sortBy == 2)
            { // Sort by Price
                compare = menu[i]->foodPrice - menu[j]->foodPrice;
            }
            else if (sortBy == 3)
            { // Sort by Stock
                compare = menu[i]->foodStock - menu[j]->foodStock;
            }

            if (compare > 0)
            {
                FoodData *temp = menu[i];
                menu[i] = menu[j];
                menu[j] = temp;
            }
        }
    }
}

void displayMenuBySorting(FoodData *head)
{
    if (!head)
    {
        printf("The menu is empty!\n");
        sleep(3);
        return;
    }

    int sortBy;
    printf("\n\nSort Menu by:\n");
    printf("1. Name\n");
    printf("2. Price\n");
    printf("3. Stock\n");
    printf("Enter your choice: ");
    scanf("%d", &sortBy);
    getchar(); // Consume newline

    int menuSize = getMenuSize(head);
    FoodData **menuArray = convertMenuToArray(head);
    sortMenu(menuArray, menuSize, sortBy);

    // Dynamically find and store unique categories
    char categories[100][11]; // Assuming a maximum of 100 categories
    int categoryCount = 0;

    for (int i = 0; i < menuSize; i++)
    {
        int found = 0;
        for (int j = 0; j < categoryCount; j++)
        {
            if (strcmp(categories[j], menuArray[i]->foodCategory) == 0)
            {
                found = 1;
                break;
            }
        }
        if (!found)
        {
            strcpy(categories[categoryCount], menuArray[i]->foodCategory);
            categoryCount++;
        }
    }

    // Display the menu grouped by categories
    // printf("\n--- Menu by Categories ---\n");
    for (int i = 0; i < categoryCount; i++)
    {
        printf("\n\nCategory: %s\n", categories[i]);
        printf("=========================================================================\n");

        int theFirstOfFoodCategory = 1;
        for (int j = 0; j < menuSize; j++)
        {
            if (strcmp(menuArray[j]->foodCategory, categories[i]) == 0)
            {
                if (theFirstOfFoodCategory == 1) {
                    printf("| Name: %-31s | Price: %-7d | Stock: %-5d |\n",
                            menuArray[j]->foodName, menuArray[j]->foodPrice, menuArray[j]->foodStock);

                    theFirstOfFoodCategory = 0;
                }else {
                    printf("|-----------------------------------------------------------------------|\n");

                    printf("| Name: %-31s | Price: %-7d | Stock: %-5d |\n",
                            menuArray[j]->foodName, menuArray[j]->foodPrice, menuArray[j]->foodStock);
                }
            }
        }

        printf("=========================================================================\n");
    }
    printf("----------------------------\n");

    free(menuArray); // Free allocated memory
}

// Function to display menu grouped by category
void displayMenuByCategory(FoodData **head)
{
    if (!(*head))
    {
        printf("The menu is empty!\n");
        sleep(3);
        return;
    }

    // Dynamically find and store unique categories
    char categories[100][11]; // Assuming a maximum of 100 categories
    int categoryCount = 0;

    FoodData *temp = *head;
    while (temp != NULL)
    {
        int found = 0;
        for (int i = 0; i < categoryCount; i++)
        {
            if (strcmp(categories[i], temp->foodCategory) == 0)
            {
                found = 1;
                break;
            }
        }
        if (!found)
        {
            strcpy(categories[categoryCount], temp->foodCategory);
            categoryCount++;
        }
        temp = temp->next;
    }

    // Display the menu grouped by categories
    // printf("\n--- Menu by Categories ---\n");
    for (int i = 0; i < categoryCount; i++)
    {
        printf("\n\nCategory: %s\n", categories[i]);
        printf("=========================================================================\n");

        temp = *head;
        int theFirstOfFoodCategory = 1;
        while (temp != NULL)
        {
            if (strcmp(temp->foodCategory, categories[i]) == 0)
            {
                if (theFirstOfFoodCategory == 1)
                {
                    printf("| Name: %-31s | Price: %-7d | Stock: %-5d |\n",
                            temp->foodName, temp->foodPrice, temp->foodStock);
                    
                    theFirstOfFoodCategory = 0;
                }else {
                    printf("|-----------------------------------------------------------------------|\n");

                    printf("| Name: %-31s | Price: %-7d | Stock: %-5d |\n",
                            temp->foodName, temp->foodPrice, temp->foodStock);
                }
            }
            temp = temp->next;
        }

        printf("=========================================================================");
    }
    printf("\n----------------------------\n");
}

// Free the entire list
void freeList(FoodData* head) {
    FoodData* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// Modular function to handle adding a food item
FoodData* handleAddFoodItem(FoodData* head, const char* filename) {
    char name[31], category[11];
    int price, stock;

    printf("Enter food name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;  // Remove newline
    printf("Enter food category: ");
    fgets(category, sizeof(category), stdin);
    category[strcspn(category, "\n")] = 0;  // Remove newline
    printf("Enter food price: ");
    scanf("%d", &price);
    printf("Enter food stock: ");
    scanf("%d", &stock);
    getchar();  // Consume newline

    head = insertAtEnd(head, name, category, price, stock);
    saveToFile(filename, head);
    printf("Food item added successfully.\n");
    sleep(3);

    return head;
}

// Modular function to handle editing a food item
FoodData* handleEditFoodItem(FoodData* head, const char* filename) {
    char name[31], category[11];
    int price, stock;

    printf("Enter food name to edit: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;  // Remove newline
    printf("Enter new food category: ");
    fgets(category, sizeof(category), stdin);
    category[strcspn(category, "\n")] = 0;  // Remove newline
    printf("Enter new food price: ");
    scanf("%d", &price);
    printf("Enter new food stock: ");
    scanf("%d", &stock);
    getchar();  // Consume newline

    head = editNode(head, name, category, price, stock);
    saveToFile(filename, head);

    return head;
}

// Modular function to handle deleting a food item
FoodData* handleDeleteFoodItem(FoodData* head, const char* filename) {
    char name[31];

    printf("Enter food name to delete: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;  // Remove newline

    head = deleteNode(head, name);
    saveToFile(filename, head);

    return head;
}

// Admin menu
void adminMenu(FoodData** head, const char* filename) {
    char choice;

    system("cls"); // Clear screen
    while (1) {
        printf("\n--- Admin Menu ---\n");
        printf("1. Display Food List\n");
        printf("2. Add Food Item\n");
        printf("3. Edit Food Item\n");
        printf("4. Delete Food Item\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        choice = getch(); // Get a single character

        if (choice < '1' || choice > '5') {
            printf("Invalid choice. Try again.\n");
            continue;
        }

        if (choice == '1') {
            displayMenuBySorting(*head);
            printf("Please enter any key to continue with the menu.\n");
            getch(); // Wait for a key press
            system("cls"); // Clear screen
        }

        if (choice == '2') {
            *head = handleAddFoodItem(*head, filename);
            system("cls"); // Clear screen
        }

        if (choice == '3') {
            displayMenuByCategory(head);
            *head = handleEditFoodItem(*head, filename);
            system("cls"); // Clear screen
        }

        if (choice == '4') {
            displayMenuByCategory(head);
            *head = handleDeleteFoodItem(*head, filename);
            system("cls"); // Clear screen
        }

        if (choice == '5') {
            printf("Exiting admin menu.\n");
            return;
        }

        sleep(3);
    }
}

// Display the items in the basket
void displayBasket(BasketItem *basket)
{
    BasketItem *temp = basket;
    printf("\n--- Basket ---\n");
    printf("%-30s %-10s %-6s\n", "Name", "Quantity", "Price");
    int total = 0;
    while (temp != NULL)
    {
        printf("%-30s %-10d %-6d\n", temp->foodName, temp->quantity, temp->price * temp->quantity);
        total += temp->price * temp->quantity;
        temp = temp->next;
    }
    printf("Total: %d\n\n", total);
}

void displayBasketByFoodName(BasketItem *basket)
{
    if (!basket)
    {
        printf("Your basket is empty!\n");
        sleep(3);
        return;
    }

    printf("\n--- Basket Contents ---\n");

    // Dynamically find and store unique food names
    char foodNames[100][31]; // Assuming a maximum of 100 unique food items in the basket
    int foodCount = 0;

    BasketItem *temp = basket;
    while (temp != NULL)
    {
        int found = 0;
        for (int i = 0; i < foodCount; i++)
        {
            if (strcmp(foodNames[i], temp->foodName) == 0)
            {
                found = 1;
                break;
            }
        }
        if (!found)
        {
            strcpy(foodNames[foodCount], temp->foodName);
            foodCount++;
        }
        temp = temp->next;
    }

    // Group and display basket items by food name
    int totalPrice = 0;
    for (int i = 0; i < foodCount; i++)
    {
        printf("\nFood: %s\n", foodNames[i]);
        printf("----------------------------\n");

        temp = basket;
        while (temp != NULL)
        {
            if (strcmp(temp->foodName, foodNames[i]) == 0)
            {
                printf("Quantity: %d, Price per Unit: %d, Subtotal: %d\n",
                       temp->quantity, temp->price, temp->quantity * temp->price);
                totalPrice += temp->quantity * temp->price;
            }
            temp = temp->next;
        }
    }

    printf("\nTotal Price: %d\n", totalPrice);
    printf("----------------------------\n");
}

// Add a food item to the basket
BasketItem *addToBasket(BasketItem *basket, const char *name, int quantity, int price)
{
    BasketItem *newItem = (BasketItem *)malloc(sizeof(BasketItem));
    if (!newItem)
    {
        printf("Memory allocation failed\n");
        sleep(3);
        exit(1);
    }
    strncpy(newItem->foodName, name, 30);
    newItem->foodName[30] = '\0'; // Null terminate
    newItem->quantity = quantity;
    newItem->price = price;
    newItem->next = basket; // Insert at the beginning
    return newItem;
}

// Modular function to handle adding food to the basket
BasketItem *handleAddToBasket(FoodData *head, BasketItem *basket)
{
    char foodName[31];
    int quantity;

    printf("Enter food name to add: ");
    fgets(foodName, sizeof(foodName), stdin);
    foodName[strcspn(foodName, "\n")] = 0; // Remove newline

    // Search for the food item in the menu
    FoodData *temp = head;
    while (temp != NULL && strcmp(temp->foodName, foodName) != 0)
    {
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("Food item not found!\n");
        sleep(3);
    }
    else if (temp->foodStock <= 0)
    {
        printf("Out of stock!\n");
        sleep(3);
    }
    else
    {
        printf("Enter quantity: ");
        scanf("%d", &quantity);
        getchar(); // Consume newline

        if (quantity > temp->foodStock)
        {
            printf("Not enough stock!\n");
        }
        else
        {
            basket = addToBasket(basket, temp->foodName, quantity, temp->foodPrice);
            temp->foodStock -= quantity;
            printf("Added to basket.\n");
        }
    }

    return basket;
}

// Function to remove an item from the basket
BasketItem *removeFromBasket(BasketItem *basket, const char *name)
{
    BasketItem *temp = basket;
    BasketItem *prev = NULL;

    while (temp != NULL)
    {
        if (strcmp(temp->foodName, name) == 0)
        {
            if (prev == NULL)
            { // First item in the basket
                basket = temp->next;
            }
            else
            {
                prev->next = temp->next;
            }
            printf("Removed %s from the basket.\n", temp->foodName);
            free(temp);
            sleep(3);
            return basket;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("Item not found in the basket!\n");
    sleep(3);
    return basket;
}

// Free the basket
void freeBasket(BasketItem *basket)
{
    BasketItem *temp;
    while (basket != NULL)
    {
        temp = basket;
        basket = basket->next;
        free(temp);
    }
}

// User menu
void userMenu(FoodData **head)
{
    system("cls"); // Clear screen

    BasketItem *basket = NULL;
    char choice;
    char foodName[31];
    int quantity;

    while (1)
    {
        LargeTitleMenu();
        printf("1. View Menu\n");
        printf("2. Add Food to Basket\n");
        printf("3. Remove Food from Basket\n");
        printf("4. View Basket\n");
        printf("5. Checkout and Pay\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        choice = getch();

        if (choice < '1' || choice > '6')
        {
            printf("Invalid choice. Try again.\n");
            sleep(3);
            system("cls"); // Clear screen
            continue;
        }

        if (choice == '1')
        {
            displayMenuBySorting(*head);
            printf("Please enter any key to continue with the menu.\n");
            getch(); // Wait for a key press
            system("cls"); // Clear screen
        }

        if (choice == '2')
        {
            displayMenuByCategory(head);
            basket = handleAddToBasket(*head, basket);
            system("cls"); // Clear screen
        }

        if (choice == '3')
        {
            displayBasketByFoodName(basket);
            printf("Enter food name to remove from basket: ");
            fgets(foodName, sizeof(foodName), stdin);
            foodName[strcspn(foodName, "\n")] = 0; // Remove newline

            basket = removeFromBasket(basket, foodName);
            system("cls"); // Clear screen
        }
        
        if (choice == '4')
        {
            displayBasketByFoodName(basket);
            printf("Please enter any key to continue with the menu.\n");
            getch(); // Wait for a key press
        }

        if (choice == '5')
        {
            printf("\n--- Payment Receipt ---\n");
            displayBasketByFoodName(basket);
            freeBasket(basket);
            basket = NULL;
            printf("Thank you for your purchase!\n");
            printf("Please enter any key to continue with the menu.\n");
            getch(); // Wait for a key press
            return;
        }

        if (choice == '6')
        {
            printf("Exiting user menu.\n");
            freeBasket(basket);
            return;
        }
    }
}

void LandingMenu(FoodData **head, const char *filename, char secretAdminMenuKey)
{
    char choice;

    while (1) {
        system("cls"); // Clear screen

        LargeTitleRestaurant();
        printf("1. User Menu\n");
        printf("2. Exit\n");
        printf("Enter your choice: ");
        choice = getch(); // Get a single character

        if (choice == secretAdminMenuKey) {
            adminMenu(head, filename);
        }

        if (choice == '1') {
            userMenu(head);
        } else

        if (choice == '2') {
            printf("Exiting the program.\n");
            return;
        }

        if (choice != '1' && choice != '2') {
            printf("Invalid choice. Try again.\n");
        }

        sleep(3);
    }
}

int main() {
    system("cls"); // Clear screen
    const char* filename = "menu.txt";

    char secretAdminMenuKey = 'm';

    // Load data from file into the doubly linked list
    FoodData* head = loadFromFile(filename);

    // Run the landing menu
    LandingMenu(&head, filename, secretAdminMenuKey);

    // Free the linked list
    freeList(head);

    return 0;
}
