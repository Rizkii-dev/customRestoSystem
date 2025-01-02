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

// Function to create a new node
FoodData* createNode(const char* name, const char* category, int price, int stock) {
    FoodData* newNode = (FoodData*)malloc(sizeof(FoodData));
    if (!newNode) {
        printf("Memory allocation failed\n");
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
    return head;
}

// Load data from file into the doubly linked list
FoodData* loadFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file: %s\n", filename);
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
        return head;
    }

    // Update the node
    strncpy(temp->foodCategory, newCategory, 10);
    temp->foodCategory[10] = '\0';  // Null terminate
    temp->foodPrice = newPrice;
    temp->foodStock = newStock;

    printf("Food item '%s' updated successfully.\n", name);
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
            displayForward(*head);
        }

        if (choice == '2') {
            *head = handleAddFoodItem(*head, filename);
            system("cls"); // Clear screen
        }

        if (choice == '3') {
            displayForward(*head);
            *head = handleEditFoodItem(*head, filename);
            system("cls"); // Clear screen
        }

        if (choice == '4') {
            displayForward(*head);
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

// User menu
void userMenu (FoodData** head, const char* filename) {
    // Will be added later
}

void LandingMenu(FoodData **head, const char *filename, char secretAdminMenuKey)
{
    char choice;

    while (1) {
        system("cls"); // Clear screen

        printf("\n--- Welcome to the Restaurant Management System ---\n");
        printf("1. User Menu\n");
        printf("2. Exit\n");
        printf("Enter your choice: ");
        choice = getch(); // Get a single character

        if (choice == secretAdminMenuKey) {
            adminMenu(head, filename);
        }

        if (choice == '1') {
            userMenu(head, filename);
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


    // Testing the functions

    // const char* filename = "menu.txt";

    // // Load data from file into the doubly linked list
    // FoodData* head = loadFromFile(filename);

    // // Display the list
    // if (head) {
    //     displayForward(head);
    // } else {
    //     printf("No data loaded.\n");
    // }

    // // Add a new item
    // head = insertAtEnd(head, "Hotdog", "Snack", 4000, 25);
    // saveToFile(filename, head);
    // printf("Added 'Hotdog'.\n");

    // // Delete an item
    // head = deleteNode(head, "Pizza");
    // saveToFile(filename, head);
    // printf("Deleted 'Pizza'.\n");

    // // Edit an item
    // head = editNode(head, "Burger", "FastFood", 5500, 18);
    // saveToFile(filename, head);
    // printf("Edited 'Burger'.\n");

    // // Display the updated list
    // displayForward(head);

    // // Free the list
    // freeList(head);

    // return 0;
}
