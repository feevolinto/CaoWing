#include <stdio.h>

/* Function prototypes */
void OneWeekCacao(float usage[]);
float ComputeAverage(float usage[]);
void SaveWeekToFile(float usage[], float average);
int LoadWeekFromFile(float usage[], float *average);
void DisplayWeekData(float usage[], float average);
char GetYesNo(char prompt[]);

int main() {
    float WeekUsage[7];
    float average;
    char choice;

    /* Ask if the user wants to view past data */
    choice = GetYesNo("Do you want to view previous data? (y/n): ");

    if (choice == 'y' || choice == 'Y') {
        if (LoadWeekFromFile(WeekUsage, &average)) {
            DisplayWeekData(WeekUsage, average);
        } else {
            printf("No previous data available.\n");
        }
    }

    /* Enter new weekly data */
    printf("\nEnter new cacao usage data for one week.\n");
    OneWeekCacao(WeekUsage);

    /* Compute average */
    average = ComputeAverage(WeekUsage);

    /* Display the new data */
    DisplayWeekData(WeekUsage, average);

    /* Save data to file */
    SaveWeekToFile(WeekUsage, average);

    printf("\nData saved successfully.\n");

    return 0;
}

/* Input daily usage for 7 days with error handling */
void OneWeekCacao(float usage[]) {
    for (int i = 0; i < 7; i++) {
        int valid = 0;
        while (!valid) {
            printf("Day %d usage (kg): ", i + 1);
            if (scanf("%f", &usage[i]) != 1) {
                while (getchar() != '\n'); // clear invalid input
                printf("Invalid input! Please enter a number.\n");
            } else if (usage[i] < 0) {
                printf("Usage cannot be negative. Please enter a non-negative value.\n");
            } else {
                valid = 1; // input is valid
            }
        }
    }
}

/* Compute weekly average */
float ComputeAverage(float usage[]) {
    float total = 0;
    for (int i = 0; i < 7; i++) {
        total += usage[i];
    }
    return total / 7;
}

/* Save daily usage and weekly average to file with "Average:" label */
void SaveWeekToFile(float usage[], float average) {
    FILE *fp = fopen("week_cacao.txt", "w");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < 7; i++) {
        fprintf(fp, "%.2f\n", usage[i]);
    }

    fprintf(fp, "Average: %.2f\n", average);
    fclose(fp);
}

/* Load daily usage and weekly average from file */
int LoadWeekFromFile(float usage[], float *average) {
    FILE *fp = fopen("week_cacao.txt", "r");
    if (fp == NULL) return 0;

    char label[10]; // buffer to store "Average:"
    for (int i = 0; i < 7; i++) {
        fscanf(fp, "%f", &usage[i]); // load daily values
    }

    fscanf(fp, "%s %f", label, average); // read "Average:" + value
    fclose(fp);
    return 1;
}

/* Display daily usage and weekly average */
void DisplayWeekData(float usage[], float average) {
    printf("\nWeekly Cacao Usage:\n");
    for (int i = 0; i < 7; i++) {
        printf("Day %d: %.2f kg\n", i + 1, usage[i]);
    }
    printf("Weekly Average: %.2f kg\n", average);
}

/* Get a validated yes/no response from user */
char GetYesNo(char prompt[]) {
    char choice;
    int valid = 0;
    while (!valid) {
        printf("%s", prompt);
        scanf(" %c", &choice);
        while (getchar() != '\n'); // clear extra input
        if (choice == 'y' || choice == 'Y' || choice == 'n' || choice == 'N') {
            valid = 1;
        } else {
            printf("Invalid input! Please enter 'y' or 'n'.\n");
        }
    }
    return choice;
}


