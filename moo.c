#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 100

typedef struct {
    char name[MAX_NAME_LENGTH];
    int price;
} Product;

typedef struct LinkedNode {
    Product data;
    struct LinkedNode* link;
} Node;

Node* front = NULL;
Node* rear = NULL;
Node* top = NULL;

void error(const char* str) {
    fprintf(stderr, "%s\n", str);
    exit(1);
}

int is_queue_empty() {
    return front == NULL;
}

void init_queue() {
    front = rear = NULL;
}

int queue_size() {
    Node* p;
    int count = 0;
    for (p = front; p != NULL; p = p->link)
        count++;
    return count;
}

void enqueue(Product e) {
    Node* p = (Node*)malloc(sizeof(Node));
    p->data = e;
    p->link = NULL;
    if (is_queue_empty())
        front = rear = p;
    else {
        rear->link = p;
        rear = p;
    }
}

Product dequeue() {
    Node* p;
    Product e;
    if (is_queue_empty())
        error("큐 공백 에러");
    p = front;
    front = front->link;
    if (front == NULL)
        rear = NULL;
    e = p->data;
    free(p);
    return e;
}

Product queue_peek() {
    if (is_queue_empty())
        error("큐 공백 에러");
    return front->data;
}

void destroy_queue() {
    while (is_queue_empty() == 0)
        dequeue();
}

void print_queue(const char* msg) {
    Node* p;
    printf("%s[%2d] = ", msg, queue_size());
    for (p = front; p != NULL; p = p->link)
        printf("%s(%d) ", p->data.name, p->data.price);
    printf("\n");
}

int is_stack_empty() {
    return top == NULL;
}

void init_stack() {
    top = NULL;
}

Product stack_peek() {
    if (is_stack_empty())
        error("스택 공백 에러");
    return top->data;
}

Product pop() {
    Node* p;
    Product e;
    if (is_stack_empty())
        error("스택 공백 에러");
    p = top;
    top = p->link;
    e = p->data;
    free(p);
    return e;
}

void destroy_stack() {
    while (is_stack_empty() == 0)
        pop();
}

int stack_size() {
    Node* p;
    int count = 0;
    for (p = top; p != NULL; p = p->link)
        count++;
    return count;
}

void push(Product e) {
    Node* p = (Node*)malloc(sizeof(Node));
    p->data = e;
    p->link = top;
    top = p;
}



void print_stack(const char* msg) {
    Node* p;
    printf("%s[%2d] = ", msg, stack_size());
    for (p = top; p != NULL; p = p->link)
        printf("%s(%d) ", p->data.name, p->data.price);
    printf("\n");
}

void addToCart() {
    Product product;
    printf("상품명을 입력하세요: ");
    scanf_s("%s", product.name, sizeof(product.name));
    printf("상품 가격을 입력하세요: ");
    scanf_s("%d", &product.price);
    enqueue(product);
    printf("상품이 장바구니에 추가되었습니다.\n");
}

void viewCart() {
    if (is_queue_empty()) {
        printf("장바구니가 비어 있습니다.\n");
        return;
    }
    print_queue("현재 장바구니");
}

void paymentProcess() {
    if (is_queue_empty()) {
        printf("장바구니가 비어 있습니다.\n");
        return;
    }
    while (!is_queue_empty()) {
        Product product = queue_peek();
        printf("상품명: %s, 가격: %d\n", product.name, product.price);
        printf("결제하시겠습니까? (Y/N): ");
        char choice;
        scanf_s(" %c", &choice);
        if (choice == 'Y' || choice == 'y') {
            push(dequeue());
            printf("상품이 결제되었습니다.\n");
        }
        else {
            dequeue();
            printf("상품이 취소되었습니다.\n");
        }
    }
}

void viewPaymentList() {
    if (is_stack_empty()) {
        printf("결제 리스트가 비어 있습니다.\n");
        return;
    }
    print_stack("결제된 상품 리스트");
}

void cancelPayment() {
    if (is_stack_empty()) {
        printf("결제 리스트가 비어 있습니다.\n");
        return;
    }
    int index;
    printf("취소할 상품의 인덱스를 입력하세요: ");
    scanf_s("%d", &index);
    if (index < 0 || index >= stack_size()) {
        printf("잘못된 인덱스입니다.\n");
        return;
    }
    Node* p = top;
    for (int i = 0; i < index; i++)
        p = p->link;
    Product product = p->data;
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = product;
    temp->link = NULL;
    if (is_queue_empty())
        front = rear = temp;
    else {
        rear->link = temp;
        rear = temp;
    }
    p = top;
    if (index == 0)
        top = top->link;
    else {
        for (int i = 0; i < index - 1; i++)
            p = p->link;
        p->link = p->link->link;
    }
    printf("상품이 결제 취소되었습니다.\n");
}

int main() {
    int choice;
    init_queue();
    init_stack();
    while (1) {
        printf("\n상품 관리 프로그램\n");
        printf("1. 상품 입력 및 장바구니에 추가\n");
        printf("2. 장바구니 조회\n");
        printf("3. 상품 결제\n");
        printf("4. 결제 리스트 조회\n");
        printf("5. 결제 취소\n");
        printf("0. 종료\n");
        printf("선택: ");
        scanf_s("%d", &choice);
        switch (choice) {
            case 1:
                addToCart();
                break;
            case 2:
                viewCart();
                break;
            case 3:
                paymentProcess();
                break;
            case 4:
                viewPaymentList();
                break;
            case 5:
                cancelPayment();
                break;
            case 0:
                printf("프로그램을 종료합니다.\n");
                exit(0);
            default:
                printf("잘못된 선택입니다.\n");
        }
    }
    return 0;
}