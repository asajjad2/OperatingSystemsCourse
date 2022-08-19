#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

struct item
{
    char name[10];
    int price;
    int qty;
    int totalPrice;
};

void swapItemStructure(struct item *srcItem, struct item *destItem)
{
    char name[10];

    strcpy(name, srcItem->name);
    int price = srcItem->price;
    int qty = srcItem->qty;
    int totalPrice = srcItem->totalPrice;

    strcpy(srcItem->name, destItem->name);
    srcItem->price = destItem->price;
    srcItem->qty = destItem->qty;
    srcItem->totalPrice = destItem->totalPrice;

    destItem->price = price;
    destItem->qty = qty;
    strcpy(destItem->name, name);
    destItem->totalPrice = totalPrice;
}

struct item *item_arr;
int totalItems;

void *displayPrices(void *arg)
{
    int totalSum = 0;

    for (int i = 0; i < totalItems; i++)
    {
        totalSum += item_arr[i].totalPrice;
    }

    printf("Thread1:\n\n");
    printf("\tItems Purchased = ");
    for (int i = 0; i < totalItems; i++)
    {
        printf("%s ", item_arr[i].name);
    }
    printf("\n");
    for (int i = 0; i < totalItems; i++)
    {
        printf("\t%s = %d x %d = %d", item_arr[i].name, item_arr[i].qty, item_arr[i].price, item_arr[i].totalPrice);
        printf("\n");
    }
    printf("\tSum = ");
    for (int i = 0; i < totalItems; i++)
    {
        if (i + 1 == totalItems)
            printf("%d=", item_arr[i].totalPrice);
        else
            printf("%d+", item_arr[i].totalPrice);
    }
    printf(" %d\n\n", totalSum);
    pthread_exit((void *)totalSum);
}

void *calculateTax(void *arg)
{
    int totalSum = *((int *)arg);
    float tax = totalSum / 1.8;
    printf("Thread2:\n\n");
    printf("\t%d/(1+0.8) = %f\n", totalSum, tax);
    printf("\tTaxed Sum : %d+%f = %f\n\n", totalSum, tax, totalSum + tax);
    pthread_exit(NULL);
}
void *calculateSales(void *arg)
{
    int totalSum = *((int *)arg);
    float sale = 0.1;
    printf("Thread3:\n\n");
    printf("\tPrice = %d, Sale = %f\n", totalSum, sale);
    printf("\t%d*%f = %f\n", totalSum, sale, totalSum * sale);
    printf("\t%d-%f = %f\n\n", totalSum, totalSum * sale, totalSum - (totalSum * sale));
    pthread_exit(NULL);
}
void *sort(void *arg)
{

    printf("Thread4:\n\n");
    for (int i = 0; i < totalItems; i++)
    {
        for (int j = i + 1; j < totalItems; j++)
        {
            if (item_arr[i].totalPrice < item_arr[j].totalPrice)
            {
                swapItemStructure(&item_arr[i], &item_arr[j]);
            }
        }
    }

    for (int i = 0; i < totalItems; i++)
    {
        printf("\t%s\t%d\n", item_arr[i].name, item_arr[i].totalPrice);
    }
}

int main()
{
    // taking input
    printf("How many items do you have?\n");
    scanf("%d", &totalItems);
    item_arr = (struct item *)malloc(totalItems * sizeof(struct item));
    for (int i = 0; i < totalItems; i++)
    {
        printf("Enter item name, price and qty:\n");
        scanf("%s %d %d", item_arr[i].name, &item_arr[i].price, &item_arr[i].qty);
    }
    for (int i = 0; i < totalItems; i++)
    {
        item_arr[i].totalPrice = item_arr[i].price * item_arr[i].qty;
    }
    // for (int i = 0; i < totalItems; i++)
    // {
    //     printf("%s\t%d\t%d\n", item_arr[i].name, item_arr[i].price, item_arr[i].qty);
    // }

    pthread_t tid1, tid2, tid3, tid4;
    void *status;
    pthread_create(&tid1, NULL, displayPrices, NULL);
    pthread_join(tid1, &status);
    pthread_create(&tid2, NULL, calculateTax, &status);
    pthread_join(tid2, NULL);
    pthread_create(&tid3, NULL, calculateSales, &status);
    pthread_join(tid3, NULL);
    pthread_create(&tid4, NULL, sort, NULL);
    pthread_join(tid4, NULL);

    // printf("%d\n", (int)status);

    free(item_arr);
}