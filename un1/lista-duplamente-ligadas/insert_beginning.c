#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
    struct Node *prev;
} Node;
// 10 -> 20 -> 30 -> NULL
// 5
Node* insertBeginning(Node* head, int value) {
    Node* newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        return head;
    }
    newNode->value = value;
    newNode->next = head;
    newNode->prev = NULL;
    if (head != NULL) {
        head->prev = newNode;
    }
    
    return newNode;
}

int main() {
    Node* head = NULL;

    head = insertBeginning(head, 10);
    head = insertBeginning(head, 20);
    head = insertBeginning(head, 30);

    Node* current = head;

    while (current != NULL) {
        printf("%d -> ", current->value);
        current = current->next;
    }

    printf("NULL\n");

    return 0;
}