#include <stdio.h>
#include <stdlib.h>

// Estrutura do nó da lista ligada
struct Node {
    // Dado armazenado no nó
    int data;
    // Ponteiro para o próximo nó
    struct Node* next;
    // 10 -> 20 -> 30 -> NULL 
};

// Função para percorrer a lista e buscar um elemento específico
struct Node* find_element(struct Node* head, int element) {
    // Inicializa o ponteiro atual para o início da lista
   struct Node* current = head; 

   while (current != NULL) {
        // Verifica se o dado do nó atual é igual ao elemento buscado
        if (current->data == element) {
            // Retorna o ponteiro para o nó encontrado
            return current;
        }
        // Move para o próximo nó
        current = current->next;
    }
    // Retorna NULL se o elemento não for encontrado
    return NULL;
}

int main(){
    struct Node* head = NULL;
    struct Node* second = NULL;
    struct Node* third = NULL;

    // Alocando memória para os nós
    head = (struct Node*)malloc(sizeof(struct Node));
    second = (struct Node*)malloc(sizeof(struct Node));
    third = (struct Node*)malloc(sizeof(struct Node));

    head->data = 10; // Atribuindo valor ao primeiro nó
    head->next = second; // Ligando o primeiro nó ao segundo
    
    second->data = 20; // Atribuindo valor ao segundo nó
    second->next = third; // Ligando o segundo nó ao terceiro

    third->data = 30; // Atribuindo valor ao terceiro nó
    third->next = NULL; // O último nó aponta para NULL

    int element_to_find = 20;
    struct Node* found_node = find_element(head, element_to_find);

    if (found_node != NULL) {
        printf("Elemento %d encontrado no nó com endereço: %p\n", element_to_find, (void*)found_node);
    } else {
        printf("Elemento %d não encontrado na lista.\n", element_to_find);
    }
    // Liberando a memória alocada para os nós
    free(head);
    free(second);
    free(third);
    return 0;
}

