#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Fuction Prototypes
int OneWeekCacao();
//function of supplierCheck
int supplierCheck();

//Initialization
float WeekUsage[7];
float total = 0;
float average;
float *ptrWU = WeekUsage; //Pointer to Array of Weekly Usage

//for supplierCheck
typedef struct{
    char name[50];
    char address[100];
    char phone[50];
    int amount;
} cacao;

int main () {
    supplierCheck();
    OneWeekCacao();

    return 0;
}

//Part 1
//For Further Refinement
int OneWeekCacao (){
    puts ("Welcome to philippine airlines.");
    for (int i=0; i<7; i++){
        printf("Enter your usage for day %d in kg", i+1);
        scanf("%f", (ptrWU + i));
        total += *(ptrWU + i); 
    }

    average = total / 7;
    printf("Average usage is %2f", average);
}

int supplierCheck(){
    FILE *file = fopen("suppz.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    cacao suppliers[15];
    char line[500];
    int count = 0;
    float demand;
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; //removes the newline

        if (sscanf(line, "%[^,],%[^,],%[^,],%d", 
            suppliers[count].name, 
            suppliers[count].address, 
            suppliers[count].phone, 
            &suppliers[count].amount) == 4) {
            
            // Increment count only if details are complete and parsed successfully
            count++; 
        } else {
            printf("Error reading line: %s\n", line);
            continue;
        }
    }
    printf("Enter the demand amount: ");
    scanf("%f", &demand);
    printf("\nSuppliers who can meet the demand of %.2f:\n", demand);
    
    for (int i = 0; i < count; i++) {
        if (suppliers[i].amount >= demand) {
            printf("Name: %s\n", suppliers[i].name);
            printf("Address: %s\n", suppliers[i].address);
            printf("Phone: %s\n", suppliers[i].phone);
            printf("Amount: %d\n\n", suppliers[i].amount);
        }
    }
    if (count == 0) {
        printf("No suppliers found.\n");
    }

    fclose(file);
}
