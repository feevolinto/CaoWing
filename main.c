#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structures
typedef struct {
    char name[100];     
    char address[150];   
    char contact[100];  
    char capacity[50];  
} Supplier;

// Core Logic
void inputConsumption(float daily_usage[], int *has_data);
void analyzeConsumption(float daily_usage[], float *total, float *average);
void forecastSupply(float inventory, float daily_avg, float reorder_threshold);
void searchSuppliers(Supplier suppliers[], int count);

// File I/O
void loadSuppliers(Supplier suppliers[], int *count);
void saveWeekData(float usage[], float average); 
int loadWeekData(float usage[], float *average);
void saveReport(float total, float average, float inventory);

// Helpers
void clearInputBuffer();
float getValidFloat(const char *prompt);
int getValidInt(const char *prompt);
void toLowerCase(char *source, char *destination);
char getYesNo(char prompt[]);

int main() {
    // Variables
    Supplier suppliers[100];
    int supplier_count = 0;
    
    float daily_usage[7];
    float total_consumption = 0;
    float average_consumption = 0;
    float current_inventory = 0;
    float reorder_threshold = 0;
    
    int choice;
    int has_data = 0; // Flag to check if usage data exists

    // Initialization 
    // 1. Load Supplier Database automatically on startup
    loadSuppliers(suppliers, &supplier_count);
    
    puts ("============================================");
    puts ("|                  CaoWing                 |");
    puts ("============================================");
    puts ("+------------------------------------------+");
    puts ("|      Cacao Inventory Monitoring and      |");
    puts ("|     Forecasting with Supplier Finder     |");
    puts ("+------------------------------------------+");
    puts (" ");
    puts ("============================================");
    puts ("|              Welcome, User!              |");
    puts ("============================================");

    // 2. Ask to load previous session data
    char load_choice = getYesNo("\n\t[System] Found 'week_cacao.txt'. \n\tPrevious Week Data Detected. \n\tLoad previous week? (y/n): ");
    if (load_choice == 'y' || load_choice == 'Y') {
        if (loadWeekData(daily_usage, &average_consumption)) {
            // Re-calculate total based on loaded avg to ensure consistency
            total_consumption = average_consumption * 7; 
            has_data = 1;
            puts ("\n+------------------------------------------+");
            printf("|    [System] Data loaded successfully.    |\n");
            puts ("+------------------------------------------+");
        } else {
            puts ("\n+------------------------------------------+");
            printf("|  [System] No valid previous data found.   |\n");
            puts ("+------------------------------------------+");
        }
    }

    // Main Menu Loop
    do {
        printf("\n============================================\n");
        printf("                   CaoWing                  \n");
        printf("============================================\n");
        printf("+------------------------------------------+\n");
        printf("|                 MAIN MENU                |\n");
        printf("+------------------------------------------+\n");
        printf("|    [1] Input New Weekly Usage            |\n");
        printf("|    [2] Forecast Supply & View Analysis   |\n");
        printf("|    [3] Search Supplier Database          |\n");
        printf("|    [4] Save Current Week Data            |\n");
        printf("|    [5] Save to Monthly Report            |\n");
        printf("|    [6] Exit                              |\n");
        printf("+------------------------------------------+");
        
        choice = getValidInt("\n\n\tEnter choice: ");

        switch (choice) {
            case 1: // Input
                printf("\n+------------------------------------------+\n");
                printf("|        [1] Input New Weekly Usage        |\n");
                printf("+------------------------------------------+\n");
                inputConsumption(daily_usage, &has_data);
                analyzeConsumption(daily_usage, &total_consumption, &average_consumption);
                break;

            case 2: // Forecast
                if (!has_data) {
                    printf("\n+------------------------------------------+\n");
                    printf("|    [2] Forecast Supply & View Analysis   |\n");
                    printf("+------------------------------------------+\n");
                    printf("\n   [!] No data available. \n   Please Input (1) or Load File first.\n");
                } else {
                    printf("\n+------------------------------------------+\n");
                    printf("|    [2] Forecast Supply & View Analysis   |\n");
                    printf("+------------------------------------------+\n");
                    // Re-display analysis first
                    analyzeConsumption(daily_usage, &total_consumption, &average_consumption);
                    
                    // Ask for inventory details
                    printf("\n********************************************\n");
                    current_inventory = getValidFloat("   Enter remaining inventory (kg): ");
                    reorder_threshold = getValidFloat("   Enter reorder threshold (kg): ");
                    printf("********************************************\n");
                    forecastSupply(current_inventory, average_consumption, reorder_threshold);
                }
                break;

            case 3: // Suppliers
                if (supplier_count == 0) {
                    printf("\n+------------------------------------------+\n");
                    printf("|       [3] Search Supplier Database       |\n");
                    printf("+------------------------------------------+\n");
                    printf("\n  [!] Warning: Supplier database is \n  empty or file missing.\n");
                } else {
                    printf("\n+------------------------------------------+\n");
                    printf("|       [3] Search Supplier Database       |\n");
                    printf("+------------------------------------------+\n");
                    searchSuppliers(suppliers, supplier_count);
                }
                break;

            case 4: // Save Week 
                if (!has_data) {
                    printf("\n+------------------------------------------+\n");
                    printf("|        [4] Save Current Week Data        |\n");
                    printf("+------------------------------------------+\n");
                    printf("\n[!] No data to save.\n");
                } else {
                    saveWeekData(daily_usage, average_consumption);
                    printf("\n+------------------------------------------+\n");
                    printf("|        [4] Save Current Week Data        |\n");
                    printf("+------------------------------------------+\n");
                    printf("+------------------------------------------+\n");
                    printf("|        [Success] Weekly data saved       |\n");
                    printf("|            to 'week_cacao.txt'.          |\n");
                    printf("+------------------------------------------+\n");
                }
                break;

            case 5: // Save Report 
                if (!has_data) {
                    printf("\n+------------------------------------------+\n");
                    printf("|        [5] Save to Monthly Report        |\n");
                    printf("+------------------------------------------+\n");
                    printf("\n   [!] No data to report.\n");
                } else {
                    if (current_inventory <= 0) {
                         // If user hasn't done step 2 yet, ask for inventory now
                         printf("\n+------------------------------------------+\n");
                        printf("|        [5] Save to Monthly Report        |\n");
                        printf("+------------------------------------------+\n");
                        current_inventory = getValidFloat("Enter current inventory for report (kg): ");
                    }
                    saveReport(total_consumption, average_consumption, current_inventory);
                }
                break;

            case 6:
                printf("\n============================================\n");
                printf("         Thank you for using CaoWing!       \n");
                printf("         Where Supply Meets Foresight      \n");
                printf("============================================\n");
                break;

            default:
                printf("\n[!] Invalid option.\n");
        }

        if (choice != 6) {
            printf("\n     +--------------------------------+\n");
            printf("     |   Press Enter to continue...   |\n");
            printf("     +--------------------------------+\n");
            getchar(); // Wait for user
        }

    } while (choice != 6);

    return 0;
}

// Functions

void inputConsumption(float daily_usage[], int *has_data) {
    printf("\n********************************************\n");
    printf(" --- Enter Cacao Usage for 7 Days (kg) ---\n");
    printf("********************************************\n");
    for (int i = 0; i < 7; i++) {
        char prompt[20];
        sprintf(prompt, "\t\tDay %d: ", i + 1);
        daily_usage[i] = getValidFloat(prompt);
    }
    *has_data = 1;
}

void analyzeConsumption(float daily_usage[], float *total, float *average) {
    float sum = 0;
    
    // 1. Calculate Sum and Average first
    for (int i = 0; i < 7; i++) {
        sum += daily_usage[i];
    }
    *total = sum;
    *average = sum / 7.0;

    // 2. Define Safety Zone (20% tolerance)
    float tolerance = *average * 0.20;
    float stableMin = *average - tolerance;
    float stableMax = *average + tolerance;

    int stableDays = 0;
    int highDays = 0;
    int lowDays = 0;

    // 3. Count the days
    printf("\n============================================\n");
    printf("              WEEKLY BREAKDOWN             \n");
    printf("============================================\n");
    printf("+------------------------------------------+\n");
    for (int i = 0; i < 7; i++) {
        printf("\tDay %d: %.2f kg", i + 1, daily_usage[i]);

        if (daily_usage[i] >= stableMin && daily_usage[i] <= stableMax) {
            stableDays++;
            printf(" %-18s \n", "| (Normal)");
        } else if (daily_usage[i] > stableMax) {
            highDays++;
            printf(" %-18s \n", "| (High)");
        } else {
            lowDays++;
            printf(" %-18s \n", "| (Low)");
        }
    }
    printf("+------------------------------------------+\n");

    printf("\n============================================\n");
    printf("             CONSUMPTION SUMMARY            \n");
    printf("============================================\n");
    printf("+------------------------------------------+\n");
    printf("      Total Usage:   %.2f kg               \n", *total);
    printf("      Daily Average: %.2f kg               \n", *average);
    printf("+------------------------------------------+\n");

    // 4. Final Decision
    printf("\n============================================\n");
    printf("                  STATUS                    \n");
    printf("============================================\n");
    printf("+------------------------------------------+\n");
    if (stableDays >= 5) {
        printf("\t STABLE Consumption\n");
    } else if (highDays > lowDays) {
        printf("\t FLUCTUATING (Trending High)\n");
    } else {
        printf("\t FLUCTUATING (Trending Low)\n");
    }
    printf("+------------------------------------------+\n");
}

void forecastSupply(float inventory, float daily_avg, float reorder_threshold) {
    float days_to_empty = (daily_avg > 0) ? (inventory / daily_avg) : 999;
    float days_to_reorder = (daily_avg > 0) ? ((inventory - reorder_threshold) / daily_avg) : 999;

    printf("\n============================================\n");
    printf("             SUPPLY FORECAST                \n");
    printf("============================================\n");
    printf("+------------------------------------------+\n");
    printf("\tInventory lasts: %.1f days              \n", days_to_empty);
    printf("+------------------------------------------+\n");

    printf("\n============================================\n");
    printf("                  STATUS                    \n");
    printf("============================================\n");
    printf("+------------------------------------------+\n");
    
    if (inventory <= reorder_threshold) {
        printf("| ALERT:   Inventory BELOW reorder point!  |\n");
        printf("|          Order Immediately.              |\n");
    } else if (days_to_reorder <= 3) {
        printf("| WARNING: Reorder point in %.1f days.     |\n", days_to_reorder);
    } else {
        printf("| STATUS:  Safe levels.                    |\n");
    }
    printf("+------------------------------------------+\n");
}

void searchSuppliers(Supplier suppliers[], int count) {
    char target[50], target_lower[50], address_lower[150];
    int found = 0;

    printf("\n============================================\n");
    printf("           SEARCH SUPPLIER DATABASE         \n");
    printf("============================================\n");
    printf("Enter location keyword (e.g., Davao, Panabo): ");
    if (fgets(target, 50, stdin) != NULL) {
        target[strcspn(target, "\n")] = 0;
    }
    toLowerCase(target, target_lower);

    printf("\n============================================\n");
    printf("         SUPPLIERS IN '%s'                  \n", target);
    printf("============================================\n");
    printf("+------------------------------------------+\n");
    
    for (int i = 0; i < count; i++) {
        toLowerCase(suppliers[i].address, address_lower);
        
        // Search for keyword in address
        if (strstr(address_lower, target_lower) != NULL) { 
            printf(" %-20s | %-15s \n", suppliers[i].name, suppliers[i].contact);
            printf(" Location: %-30s \n", suppliers[i].address);
            printf("+------------------------------------------+\n");
            found = 1;
        }
    }

    if (!found) {
        printf("|   No suppliers found in that location.   |\n");
        printf("+------------------------------------------+\n");
    }
}

// FILE I/O FUNCTIONS

void loadSuppliers(Supplier suppliers[], int *count) {
    FILE *file = fopen("suppliers.txt", "r");
    if (file == NULL) return; // Silent fail if no file, that's okay

    while (*count < 100 && fscanf(file, " %99[^,], %149[^,], %99[^,], %49[^\n]", 
            suppliers[*count].name, 
            suppliers[*count].address, 
            suppliers[*count].contact,
            suppliers[*count].capacity) == 4) {
        (*count)++;
    }
    fclose(file);
    if (*count > 0) printf("\n[System] Loaded %d suppliers from database.\n", *count);
}

// Save specifically for reloading state later (Program 1 style)
void saveWeekData(float usage[], float average) {
    FILE *fp = fopen("week_cacao.txt", "w");
    if (fp == NULL) {
        printf("[!] Error saving file.\n");
        return;
    }
    for (int i = 0; i < 7; i++) {
        fprintf(fp, "%.2f\n", usage[i]);
    }
    fprintf(fp, "Average: %.2f\n", average);
    fclose(fp);
}

// Load specifically to restore state
int loadWeekData(float usage[], float *average) {
    FILE *fp = fopen("week_cacao.txt", "r");
    if (fp == NULL) return 0;

    char label[20];
    for (int i = 0; i < 7; i++) {
        if (fscanf(fp, "%f", &usage[i]) != 1) { 
            fclose(fp); return 0; 
        }
    }
    fscanf(fp, "%s %f", label, average);
    fclose(fp);
    return 1;
}

// Save for long-term reporting 
void saveReport(float total, float average, float inventory) {
    FILE *file = fopen("report.txt", "a");
    if (file == NULL) {
        printf("\n============================================\n");
        printf("                   ERROR                    \n");
        printf("============================================\n");
        printf("+------------------------------------------+\n");
        printf("|      [!] Error opening report file.      |\n");
        printf("+------------------------------------------+\n");
        return;
    }
    fprintf(file, "Weekly Total: %.2f kg | Avg: %.2f kg | Inventory Remaining: %.2f kg\n", total, average, inventory);
    fclose(file);
    
    printf("\n============================================\n");
    printf("                  SUCCESS                   \n");
    printf("============================================\n");
    printf("+------------------------------------------+\n");
    printf("|       Data saved to 'report.txt'         |\n");
    printf("+------------------------------------------+\n");
}

// Helper Functions

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
            printf("[!] Invalid input. Positive number required.\n");
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

char getYesNo(char prompt[]) {
    char choice;
    int valid = 0;
    while (!valid) {
        printf("%s", prompt);
        scanf(" %c", &choice);
        clearInputBuffer();
        if (choice == 'y' || choice == 'Y' || choice == 'n' || choice == 'N') {
            valid = 1;
        } else {
            // If file doesn't exist, we might not want to spam invalid, 
            // but here we just return 'n' if they type garbage to keep flow
            printf("Please enter 'y' or 'n'.\n");
        }
    }
    return choice;
}

void toLowerCase(char *source, char *destination) {
    int i = 0;
    while (source[i] != '\0') {
        destination[i] = tolower(source[i]);
        i++;
    }
    destination[i] = '\0';
}