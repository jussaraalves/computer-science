#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Node {
    double value;
    struct Node* next;
};

void find_even_numbers_list(struct Node* head){
    struct Node* current = head;
    while (current != NULL) {
        if(fmod(current->value, 1.0) == 0.0){
            printf("%g\n", current->value);
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

    second->value = 10.5;
    second->next = third;

    third->value = 15;
    third->next = fourth;

    fourth->value = 20;
    fourth->next = NULL;

    find_even_numbers_list(head);
}
