#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Use at least one user-defined function for computations (e.g., forecasting supply).
typedef struct {
    char name[100];     
    char address[150];   
    char contact[100];  
    char capacity[50];  
} Supplier;

// --- FUNCTION PROTOTYPES ---
void loadSuppliers(Supplier suppliers[], int *count);
void inputConsumption(float daily_usage[]);
void analyzeConsumption(float daily_usage[], float *total, float *average);
void forecastSupply(float inventory, float daily_avg, float reorder_threshold);
void viewSuppliers(Supplier suppliers[], int count);
void saveReport(float total, float average, float inventory);
void toLowerCase(char *source, char *destination); // New helper function

// Helper functions
void clearInputBuffer();
float getValidFloat(const char *prompt);
int getValidInt(const char *prompt);

int main() {
    // Initializing Variables
    // Use arrays, strings, and pointers for storing consumption data and supplier names.
    Supplier suppliers[100];
    int supplier_count = 0;
    float daily_usage[7];
    float current_inventory = 0, reorder_threshold = 0;
    float total_consumption = 0, average_consumption = 0;
    int choice;
    int has_data = 0; 

    // 2. Load Database
    loadSuppliers(suppliers, &supplier_count);

    // 3. Main Menu Loop
    do {
        printf("\n========================================\n");
        printf("       CaoWing Management System\n");
        printf("========================================\n");
        printf("[1] Input Weekly Consumption\n");
        printf("[2] Forecast Supply & Reorder Point\n");
        printf("[3] Find Suppliers by Location\n");
        printf("[4] Save Report to File\n");
        printf("[5] Exit\n");
        
        choice = getValidInt("Enter choice: ");

        switch (choice) {
            case 1:
                inputConsumption(daily_usage);
                analyzeConsumption(daily_usage, &total_consumption, &average_consumption);
                has_data = 1;
                break;

            case 2:
                if (has_data == 0) {
                    printf("\n[!] Error: No consumption data found. Option 1 first.\n");
                } else if (average_consumption <= 0) {
                    printf("\n[!] Error: Average consumption is 0.\n");
                } else {
                    current_inventory = getValidFloat("\nEnter current inventory (kg): ");
                    reorder_threshold = getValidFloat("Enter reorder threshold (kg): ");
                    forecastSupply(current_inventory, average_consumption, reorder_threshold);
                }
                break;

            case 3:
                if (supplier_count == 0) {
                    printf("\n[!] Warning: Database is empty.\n");
                } else {
                    viewSuppliers(suppliers, supplier_count);
                }
                break;

            case 4:
                if (has_data == 0) {
                    printf("\n[!] Error: No data available to save.\n");
                } else {
                    if (current_inventory <= 0) {
                        current_inventory = getValidFloat("Enter current inventory to save (kg): ");
                    }
                    saveReport(total_consumption, average_consumption, current_inventory);
                }
                break;

            case 5:
                printf("\nExiting system. Goodbye!\n");
                break;

            default:
                printf("\n[!] Invalid option. Try 1-5.\n");
        }
        
        if (choice != 5) {
            printf("\nPress Enter to continue...");
            getchar(); 
        }

    } while (choice != 5);

    return 0;
}

// FUNCTIONS

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

float getValidFloat(const char *prompt) {
    float value;
    int status;
    do {
        printf("%s", prompt);
        status = scanf("%f", &value);
        clearInputBuffer(); 
        if (status != 1 || value < 0) {
            printf("[!] Invalid input. Enter a positive number.\n");
        } 
    } while (status != 1 || value < 0);
    return value;
}

int getValidInt(const char *prompt) {
    int value;
    int status;
    printf("%s", prompt);
    status = scanf("%d", &value);
    clearInputBuffer();
    if (status != 1) return -1;
    return value;
}

// Helper function to convert string to lowercase for search comparison
void toLowerCase(char *source, char *destination) {
    int i = 0;
    while (source[i] != '\0') {
        destination[i] = tolower(source[i]);
        i++;
    }
    destination[i] = '\0';
}

void loadSuppliers(Supplier suppliers[], int *count) {
    FILE *file = fopen("suppliers.txt", "r");
    
    if (file == NULL) {
        printf("\n[!] File Error: 'suppliers.txt' not found.\n");
        return;
    }
    // Reads format: Name, Address, Contact, Capacity
    while (*count < 100 && fscanf(file, " %99[^,], %149[^,], %99[^,], %49[^\n]", 
            suppliers[*count].name, 
            suppliers[*count].address, 
            suppliers[*count].contact,
            suppliers[*count].capacity) == 4) {
        (*count)++;
    }
    
    fclose(file);
    printf("\n[System] Successfully loaded %d suppliers.\n", *count);
}

void inputConsumption(float daily_usage[]) {
    printf("\n--- Enter Cacao Usage for 7 Days (kg) ---\n");
    for (int i = 0; i < 7; i++) {
        char prompt[20];
        sprintf(prompt, "Day %d: ", i + 1);
        daily_usage[i] = getValidFloat(prompt);
    }
}

void analyzeConsumption(float daily_usage[], float *total, float *average) {
    float sum = 0;
    float min_usage = daily_usage[0];
    float max_usage = daily_usage[0];

    for (int i = 0; i < 7; i++) {
        sum += daily_usage[i];
        if (daily_usage[i] < min_usage) min_usage = daily_usage[i];
        if (daily_usage[i] > max_usage) max_usage = daily_usage[i];
    }

    *total = sum;
    *average = sum / 7.0;

    printf("\n--- Analysis Results ---\n");
    printf("Weekly Total: %.2f kg\n", *total);
    printf("Daily Average: %.2f kg\n", *average);

    if ((max_usage - min_usage) > (*average * 0.5)) {
        printf("Status: Usage is FLUCTUATING widely.\n");
    } else {
        printf("Status: Usage is STABLE.\n");
    }
}

void forecastSupply(float inventory, float daily_avg, float reorder_threshold) {
    float days_to_empty = inventory / daily_avg;
    float days_to_reorder = (inventory - reorder_threshold) / daily_avg;

    printf("\n--- Supply Forecast ---\n");
    printf("Current inventory will last: %.1f days\n", days_to_empty);

    if (inventory <= reorder_threshold) {
        printf("ALERT: Inventory is BELOW safety threshold! Reorder IMMEDIATELY.\n");
    } else if (days_to_reorder <= 3) {
        printf("WARNING: You will reach reorder point in %.1f days.\n", days_to_reorder);
    } else {
        printf("STATUS: Safe. Reorder recommended in %.1f days.\n", days_to_reorder);
    }
}

void viewSuppliers(Supplier suppliers[], int count) {
    char target[50];
    char target_lower[50];     // To store lowercase search term
    char address_lower[150];   // To store lowercase address for comparison
    int found = 0;

    printf("\nEnter location keyword (e.g., Davao, Panabo, Calinan): ");
    if (fgets(target, 50, stdin) != NULL) {
        target[strcspn(target, "\n")] = 0;
    }

    // Convert input to lowercase
    toLowerCase(target, target_lower);

    printf("\n--- Search Results for '%s' ---\n", target);
    for (int i = 0; i < count; i++) {
        // Convert the supplier's address to lowercase for checking
        toLowerCase(suppliers[i].address, address_lower);

        // Check if target is inside address (case-insensitive)
        if (strstr(address_lower, target_lower) != NULL) { 
            printf("Name:     %s\n", suppliers[i].name);
            printf("Address:  %s\n", suppliers[i].address);
            printf("Contact:  %s\n", suppliers[i].contact);
            printf("Capacity: %s\n", suppliers[i].capacity);
            printf("-----------------------------------\n");
            found = 1;
        }
    }

    if (!found) {
        printf("No suppliers found matching \"%s\".\n", target);
    }
}

void saveReport(float total, float average, float inventory) {
    FILE *file = fopen("report.txt", "a");
    if (file == NULL) {
        printf("[!] Error: Could not write to 'report.txt'.\n");
        return;
    }
    fprintf(file, "Weekly Total: %.2f kg | Avg: %.2f kg | Remaining: %.2f kg\n", total, average, inventory);
    fclose(file);
    printf("\n[Success] Report saved to 'report.txt'.\n");
}