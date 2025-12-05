#include <stdio.h>

//Fuction Prototypes
int OneWeekCacao();

//Initialization
float WeekUsage[7];
float total = 0;
float average;
float *ptrWU = WeekUsage; //Pointer to Array of Weekly Usage

int main () {

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



    return 0;
}