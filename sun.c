#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Product {
    char name[50];
    int price;
    struct Product* next;
} Product;

typedef struct Queue {
    Product* front;
    Product* rear;
} Queue;

Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = queue->rear = NULL;
    return queue;
}

Product* createProduct(char name[], int price) {
    Product* product = (Product*)malloc(sizeof(Product));
    strcpy(product->name, name);
    product->price = price;
    product->next = NULL;
    return product;
}

void enqueue(Queue* queue, Product* product) {
    if (queue->rear == NULL) {
        queue->front = queue->rear = product;
        return;
    }
    queue->rear->next = product;
    queue->rear = product;
}

Product* dequeue(Queue* queue) {
    if (queue->front == NULL)
        return NULL;

    Product* temp = queue->front;

    queue->front = queue->front->next;
    if (queue->front == NULL)
        queue->rear = NULL;

    return temp;
}

Product* removeProduct(Queue* queue, int index) {
    if (queue->front == NULL)
        return NULL;

    Product* temp = queue->front;
    Product* prev = NULL;

    if (index == 0) {
        queue->front = queue->front->next;
        if (queue->front == NULL)
            queue->rear = NULL;
        return temp;
    }

    for (int i = 0; temp != NULL && i < index; i++) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
        return NULL;

    prev->next = temp->next;
    if (queue->rear == temp)
        queue->rear = prev;

    return temp;
}

void cancelPayment(Queue* cart, Queue* paid, int index) {
    Product* product = removeProduct(paid, index);
    if (product != NULL) {
        enqueue(cart, createProduct(product->name, product->price));
        free(product);
    }
}

void display(Queue* queue) {
    Product* temp = queue->front;
    int index = 0;
    while (temp != NULL) {
        printf("%d. 상품명: %s, 가격: %d\n", index, temp->name, temp->price);
        temp = temp->next;
        index++;
    }
    printf("=====================================\n");
}

void pay(Queue* cart, Queue* paid) {
    Product* temp = cart->front;
    int index = 0;
    while (temp != NULL) {
        printf("상품명: %s, 가격: %d\n", temp->name, temp->price);
        printf("이 상품을 결제하시겠습니까? (y/n): ");
        char choice;
        scanf(" %c", &choice);
        if (choice == 'y' || choice == 'Y') {
            enqueue(paid, createProduct(temp->name, temp->price));
            printf("결제 완료\n");
            removeProduct(cart, index);
        } else {
            index++;
        }
        temp = temp->next;
    }
    printf("결재 끝\n");
}

int main() {
    Queue* cart = createQueue();
    Queue* paid = createQueue();
    char name[50];
    int price;
    int choice;

    while(1) {
        printf("\n1. 상품을 입력받고 장바구니에 넣기\n");
        printf("2. 현재 장바구니에 들어 있는 상품 조회\n");
        printf("3. 상품 결제하기\n");
        printf("4. 결제 리스트 조회\n");
        printf("5. 결제 취소하기\n");
        printf("6. 종료\n");
        printf("선택: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                printf("상품명: ");
                scanf("%s", name);
                printf("가격: ");
                scanf("%d", &price);
                enqueue(cart, createProduct(name, price));
                break;
            case 2:
                printf("\n장바구니:\n");
                display(cart);
                break;
            case 3:
                pay(cart, paid);
                break;
            case 4:
                printf("결제 리스트:\n");
                display(paid);
                break;
            case 5:
                printf("결제 취소할 상품의 인덱스: ");
                int index;
                scanf("%d", &index);
                cancelPayment(cart, paid, index);
                printf("결제 취소 완료\n");
                break;
            case 6:
                return 0;
            default:
                printf("잘못된 선택입니다.\n");
        }
    }

    return 0;
}