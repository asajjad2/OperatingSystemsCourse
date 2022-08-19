#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

int totalItems;
int *price, *quantity;
char** itemNames;

void* inputFunc(void* arg)
{
    char buffer[256];

    printf("Enter total number of items: ");
    scanf("%d", &totalItems);

    price = (int *) malloc(totalItems * sizeof(int));
    quantity = (int *) malloc(totalItems * sizeof(int));
    itemNames = (char **) malloc(totalItems * sizeof(char*));

    for (int i=0; i<totalItems; i++)
        itemNames[i] = (char *) malloc(32);

    printf("\n");
    for(int i=0; i< totalItems; i++)
    {
        printf("For Item# %d \n", (i+1));
        printf("Enter item name: ");
        scanf("%s", buffer);
        strcpy(itemNames[i], buffer);

        printf("Enter item price: ");
        scanf("%d", &price[i]);

        printf("Enter item quantity: ");
        scanf("%d", &quantity[i]);

        printf("\n");
    }

    pthread_exit(NULL);
}

void *dispFunc(void *vargp)
{
    printf("\nItems Purchased \n\n");

    for (int i=0; i<totalItems; i++)
    {
        printf("- %s", itemNames[i]);
        if (i+1 != totalItems)
            printf("\n");
    }
    printf("\n\n");

    int totalSum=0;
    int sum[totalItems];

    for (int i=0; i<totalItems; i++)
    {
        sum[i] = quantity[i] * price[i];
        totalSum += sum[i];

        printf("%s : %d x %d = %d", itemNames[i], price[i], quantity[i], sum[i]);
        
        if (i+1 != totalItems)
            printf("\n");       
    }

    printf("\n\nSum: ");

    for (int i=0; i<totalItems; i++)
    {
        printf("%d", sum[i]);
        
        if (i+1 == totalItems)
            printf(" = ");
        else
            printf(" + ");

    }

    printf("%d\n", totalSum);
        
    pthread_exit((void *)totalSum);
}

void* calculateTax(void* arg)
{
    int totalSum = *((int *)arg);

    char output[256];
    char tempBuffer[64];

    double tax = totalSum * 0.08;

    printf("\n\nCalculating Tax\n\n");

    printf("%d * 0.08 = %f\n", totalSum, tax);

    double taxedSum = totalSum + tax;

    printf("TaxedSum: %d + %f = %f", tax,  totalSum,taxedSum);


    pthread_exit(NULL);
}

void* calculateSales(void *arg)
{
    int totalSum = *((int *)arg);

    char output[256];
    char tempBuffer[64];

    printf("\n\nCalculating Sale\n\n");

    if(totalSum<250)
        printf("Discount not applicable\n");
    else
        printf("Price: %d -> sales: 10%%\n", totalSum);

    double sales = totalSum * 0.1;
    double SalesSum = totalSum - sales;

    printf("%d * 0.1 = %f ,  %d - %f = %f", totalSum, sales, totalSum, sales, SalesSum);


    pthread_exit(NULL);
}

void* sort(void* arg)
{
    char output[256];
    char tempBuffer[64];
    
    //sleep(1);
    printf("\n\nItems List\n\n");

    int tempPrice[totalItems], temp[totalItems];

    for (int i=0; i<totalItems; i++)
    {
        tempPrice[i]=price[i];
        temp[i]=i;
    }

    for (int i=0; i<totalItems-1; i++)
    {
        for (int j=i+1; j<totalItems; j++)
        {
            if (tempPrice[j] > tempPrice[i])
            {
                int swapVar = tempPrice[i];
                tempPrice[i] = tempPrice[j];
                tempPrice[j] = swapVar;

                swapVar = temp[i];
                temp[i]=temp[j];
                temp[j]=swapVar;
            }
        }
    }

    printf("Items");
    for (int k=0; k<10; k++)
        printf(" ");

    printf("Price\n");

    for (int i=0; i<totalItems; i++)
    {
        printf("%-15s", itemNames[temp[i]]);
        printf("%d\n", tempPrice[i]);
    }

    pthread_exit(NULL);
}

int main()
{
    void* status;
    pthread_t t1, t2, t3, t4, t5;    

    pthread_create(&t1, NULL, inputFunc, NULL);
    pthread_join(t1, NULL);

    pthread_create(&t2, NULL, dispFunc, NULL);
    pthread_join(t2, &status);

    pthread_create(&t3, NULL, calculateTax, &status);
    pthread_join(t3, NULL);

    pthread_create(&t4, NULL, calculateSales, &status);
    pthread_join(t4, NULL);

    pthread_create(&t5, NULL, sort, NULL);
    pthread_join(t5, NULL);

    pthread_exit(0);
}