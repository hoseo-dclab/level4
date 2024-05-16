#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int queue_full();
int queue_empty();
void basket(char product_name[], int product_price);
void payment_list(char product_name[], int product_price);
void payment_print();
void cancle_payment();
void product_input();

struct NODE { // 상품을 저장할 구조체
    char data[100];
    int price;
    struct NODE* next;
};

struct NODE* front = NULL;
int node_cnt = 0;

void enqueue(char product_name[], int product_price) {
    if(!queue_full()) {
        struct NODE* new_node = malloc(sizeof(struct NODE));
        if(new_node == NULL) {
            return;
        }
        strcpy(new_node -> data, product_name);
        new_node -> price = product_price;
        new_node -> next = NULL;
        node_cnt++;
        if(front == NULL) {
            front = new_node;
        } else {
            struct NODE* temp = front;
            while(temp -> next != NULL) {
                temp = temp -> next;
            }
            temp -> next = new_node;
        }
    }
}

int dequeue() {
    if(!queue_empty()) {
        struct NODE* temp = front;
        int value = front -> price;
        front = front -> next;
        free(temp);
        node_cnt--;
        return value;
    }
    return -1;
}

int queue_full() {
    return node_cnt == 10;
}

int queue_empty() {
    return node_cnt == 0;
}

void basket(char product_name[], int product_price) { // 상품을 장바구니에 담기
    enqueue(product_name, product_price);
}

void basket_print() { // 장바구니에 담긴 상품을 출력
    struct NODE* temp = front;
    int i = 1;
    while(temp != NULL) {
        printf("%d. %s : %d\n", i++, temp -> data, temp -> price);
        temp = temp -> next;
    }
}

void payment() { // 결제를 진행
    struct NODE* temp = front;
    struct NODE* prev = NULL;
    int i = 1;
    while(temp != NULL) { // 장바구니에 담긴 상품을 1개씩 결제여부를 결정
        printf("%d. %s : %d\n", i++, temp -> data, temp -> price);
        printf("결제하시겠습니까? (1: 예, 2: 아니오) : ");
        int num;
        scanf("%d", &num);
        if(num == 1) {
            if(prev == NULL) {
                front = front -> next;
            } else {
                prev -> next = temp -> next;
            }
            payment_list(temp -> data, temp -> price);
            struct NODE* toFree = temp;
            temp = temp -> next;
            free(toFree);
            node_cnt--;
        } else {
            prev = temp;
            temp = temp -> next;
        }
    }
}

struct PAYMENT { // 결제 내역을 저장할 구조체
    char data[100];
    int price;
    struct PAYMENT* next;
};

struct PAYMENT* payment_front = NULL;

void payment_list(char product_name[], int product_price) { // 결제 내역을 저장
    struct PAYMENT* new_node = malloc(sizeof(struct PAYMENT));
    if(new_node == NULL) {
        return;
    }
    strcpy(new_node -> data, product_name);
    new_node -> price = product_price;
    new_node -> next = NULL;
    if(payment_front == NULL) {
        payment_front = new_node;
    } else {
        struct PAYMENT* temp = payment_front;
        while(temp -> next != NULL) {
            temp = temp -> next;
        }
        temp -> next = new_node;
    }
}

void payment_print() { // 결제 내역을 출력
    struct PAYMENT* temp = payment_front;
    int i = 1;
    while(temp != NULL) {
        printf("%d. %s : %d\n", i++, temp -> data, temp -> price);
        temp = temp -> next;
    }
}

void cancle_payment() { // 결제 취소
    struct PAYMENT* temp = payment_front;
    int i = 1;
    while(temp != NULL) {
        printf("%d. %s : %d\n", i++, temp -> data, temp -> price);
        temp = temp -> next;
    }
    printf("결제 취소할 상품의 번호를 입력하세요 : ");
    int num;
    scanf("%d", &num);
    if(num == 1) {
        struct PAYMENT* temp = payment_front;
        payment_front = payment_front -> next;
        basket(temp -> data, temp -> price);
        free(temp);
    }
}

void product_input() { // 상품명과 가격을 입력받아 장바구니에 담기
    printf("상품명과 가격을 입력하세요 : ");
    char product_name[100];
    int product_price;
    scanf("%s %d", product_name, &product_price);
    basket(product_name, product_price);
}

void main() {
    int num = 0;
    while(1) {
        printf("1.상품 입력 2.장바구니 조회 3.결제 여부 결정 4.결제 상품 조회 5.결제 취소 6. 종료\n");
        scanf("%d", &num);
        switch (num) {
            case 1: // 상품 입력
                product_input();
                break;
            case 2: // 장바구니 출력
                basket_print();
                break;
            case 3: // 결제 여부 체크
                payment();
                break;
            case 4: // 결제 내역 출력
                payment_print();
                break;
            case 5: // 결제 취소
                cancle_payment();
                break;
            case 6: // 프로그램 종료
                return;
        }
    }
}
