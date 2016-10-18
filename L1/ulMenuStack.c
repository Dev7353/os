# include <stdio.h>
# include <stdlib.h>
# include "ulstack.h"

int main(void)
{
    
    ulstack *stack = NULL;
    stack = (ulstack *) malloc(sizeof(ulstack));
    
    if(stack == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    
    while(1)
    {
        printf("\n");
        printf("Enter your choice:\n");
        printf("1) Create Stack\n");
        printf("2) Push Value\n");
        printf("3) Pop Value\n");
        printf("4) Print Number of Elements on Stack\n");
        printf("5) Remove Stack\n");
        printf("6) Exit\n");
        printf(">> ");
        int entry;
        scanf("%i", &entry);
        
        int value;
        
        switch (entry) {
            case 1:
                ULStackNew(stack);
                printf("Stack created!\n");
                break;
            case 2:
                printf("Enter value.\n");
                scanf("%u", &value);
                ULStackPush(stack, (unsigned long)value);
                printf("Value %i pushed!\n", value);
                break;
            case 3:
                value = ULStackPop(stack);
                printf("Pop Stack %d\n", value);
                break;
            case 4:
                printf("Elements: %i\n", GetULStackNumberOfElements(stack));
                break;
            case 5:
                ULStackDispose(stack);
                printf("Stack deleted!\n");
                break;
            case 6:
                return 0;
        }
        printf("\n");
    }
    
    
    free(stack);
}
