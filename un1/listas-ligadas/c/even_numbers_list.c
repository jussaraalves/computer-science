#include <stdio.h>
#include <stdlib.h>

struct Node {
    int value;
    struct Node* next;
};

// 5 -> 10 -> 15 -> 20 -> NULL
void find_even_numbers(struct Node* head){
    struct Node* current = head; 
    while (current != NULL) {
        if(current->value % 2 == 0){   
            printf("%d\n", current->value);
        }
        current = current->next;
    }
}

int main(){
    struct Node* head = NULL;
    struct Node* second = NULL;
    struct Node* third = NULL;
    struct Node* fourth = NULL;

    head = malloc(sizeof(struct Node));
    second = malloc(sizeof(struct Node));
    third = malloc(sizeof(struct Node));
    fourth = malloc(sizeof(struct Node));

    head->value = 5;
    head->next = second;

    second->value = 10;
    second->next = third;

    third->value = 15;
    third->next = fourth;

    fourth->value = 20;
    fourth->next = NULL;

    find_even_numbers(head);

}