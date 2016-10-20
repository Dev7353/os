# include <stdio.h>
# include <stdlib.h>
# include "ulstack.h"

int main(void)
{
    ulstack *stack = NULL;
    stack = (ulstack *) malloc(sizeof(ulstack));
    if(stack == NULL)
    {
        perror("Memory allocation failed\n");
        return 1;
    }
    
    int entry; // users choice
    unsigned long value; // push and pop
    
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
        
        if (scanf("%i", &entry) != 1)
        {
            printf("Only numbers are allowed. Try again\n");
            while( getchar() != '\n' ); // clean buffer from scanf to prevent infinite print
                                        // source http://stackoverflow.com/questions/32594465/how-to-prevent-an-infinite-loop-with-scanf-failure
            continue;
        }
        
        switch (entry) {
            case 1:
                if(stack_created == 1)
                {
                    perror("You already created a Stack.\n");
                    continue;
                }
                ULStackNew(stack);
                printf("Stack created!\n");
                stack_created = 1;
                break;
            case 2:
                if(stack_created == 0)
                {
                    perror("You don't have a initialized Stack. Please create a Stack.\n");
                    continue;
                }
                printf("Enter value.\n");
                scanf("%lu", &value);
                ULStackPush(stack, value);
                printf("Push %lu on Stack!\n", value);
                break;
            case 3:
                if(stack_created == 0)
                {
                    perror("You don't have a initialized Stack. Please create a Stack.\n");
                    continue;
                }
                else if(stack->logLength == 0)
                {
                    perror("You can't pop from a empty Stack. Please push a value.\n");
                    continue;
                }
                value = ULStackPop(stack);
                printf("Pop from Stack: %lu\n", value);
                break;
            case 4:
                printf("Elements on Stack: %i\n", GetULStackNumberOfElements(stack));
                break;
            case 5:
                if(stack_created == 0)
                {
                    perror("You don't have a initialized Stack. Please create a Stack.");
                    continue;
                }
                ULStackDispose(stack);
                printf("Stack deleted!\n");
                stack_created = 0;
                break;
            case 6:
                free(stack);
                return 0;
                
            default:
                printf("Not supported choice. Try again.\n");
                continue;
        }
        printf("\n");
    }
}
