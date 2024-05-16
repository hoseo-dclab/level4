#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUCT_NAME_SIZE 20

int balance = 5000;

typedef struct Info {
    char *productName;
    int price;
} Item;

typedef struct LinkedQueueNode {
    Item item;
    struct LinkedQueueNode* link;
} Product;

Product* front = NULL;
Product* rear = NULL;

typedef struct DLinkedList {
    Item item;
    struct DLinkedList* prev;
    struct DLinkedList* next;
} PayedProduct;

PayedProduct org;    // 헤드 노드 (헤드 포인터가 아님)

// init_basket 함수 정의
/*
 * @Name : init_basket
 * @Description : 장바구니를 초기화합니다. front와 rear를 NULL로 설정합니다.
 */
void init_basket()
{
    front = rear = NULL;
}

// init_payment_list 함수 정의
/*
 * @Name : init_payment_list
 * @Description : 결제 목록을 초기화합니다. org의 next를 NULL로 설정합니다.
 */
void init_payment_list() { org.next = NULL; }

// get_head 함수 정의
/*
 * @Name : get_head
 * @Description : 결제 목록의 헤드 노드를 반환합니다.
 */
PayedProduct* get_head() { return (PayedProduct *) org.next; }

// basket_size 함수 정의
/*
 * @Name : basket_size
 * @Description : 장바구니에 담긴 상품의 수를 반환합니다.
 */
int basket_size()
{
    Product *p;
    int count = 0;

    for (p = front; p != NULL; p = p->link)
        count++;

    return count;
}

// payment_size 함수 정의
/*
 * @Name : payment_size
 * @Description : 결제된 상품의 수를 반환합니다.
 */
int payment_size()
{
    PayedProduct* pp;
    int count = 0;
    for (pp = get_head(); pp != NULL; pp = (PayedProduct *) pp->next)
        count++;
    return count;
}

// get_entry 함수 정의
/*
 * @Name : get_entry
 * @Param : pos - 조회할 위치 인덱스(0부터 시작)
 * @Description : 주어진 위치에 해당하는 PayedProduct 노드를 반환합니다.
 */
PayedProduct* get_entry(int pos)
{
    PayedProduct* pp = &org;
    for (int i = -1; i<pos; i++, pp = (PayedProduct *) pp->next)
        if (pp == NULL) break;
    return pp;
}

// is_basket_empty 함수 정의
/*
 * @Name : is_basket_empty
 * @Description : 장바구니가 비어있는지 여부를 반환합니다.
 */
int is_basket_empty()
{
    return front == NULL;
}

// is_payment_empty 함수 정의
/*
 * @Name : is_payment_empty
 * @Description : 결제 목록이 비어있는지 여부를 반환합니다.
 */
int is_payment_empty()
{
    return payment_size() > 0;
}

// enqueue 함수 정의
/*
 * @Name : enqueue
 * @Param : item - enqueue할 Item 구조체
 * @Description : 주어진 Item을 덱의 뒤쪽에 추가합니다.
 *                 덱이 비어있을 경우, front와 rear를 새로운 노드로 설정합니다.
 *                 덱이 비어있지 않을 경우, rear의 link를 새로운 노드로 설정하고, rear를 새로운 노드로 업데이트합니다.
 */
void enqueue(Item item)
{
    Product* p = (Product*)malloc(sizeof(Product));
    p->item.productName = malloc(strlen(item.productName));

    strcpy(p->item.productName, item.productName);
    p->item.price = item.price;

    p->link = NULL;

    if (is_basket_empty())
        front = rear = p;
    else {
        rear->link = p;
        rear = p;
    }
}

// insert_next 함수 정의
/*
 * @Name : insert_next
 * @Param : before - 새로운 노드를 삽입할 노드의 이전 노드 포인터, pp - 삽입할 새로운 노드 포인터
 * @Description : 주어진 이전 노드 이후에 새로운 노드를 삽입합니다.
 *                 이전 노드의 next를 새로운 노드로, 새로운 노드의 prev를 이전 노드로 설정합니다.
 */
void insert_next(PayedProduct *before, PayedProduct *pp)
{
    PayedProduct* pp_before;

    pp->prev = (PayedProduct *) before;
    pp_before = (PayedProduct *) before->next;
    pp->next = (PayedProduct *) pp_before;

    if (before->next != NULL) pp_before->prev = pp;
    before->next = pp;
}

// insert 함수 정의
/*
 * @Name : insert
 * @Param : pos - 삽입할 위치 인덱스(0부터 시작), item - 삽입할 Item 구조체
 * @Description : 주어진 위치에 새로운 노드를 삽입합니다.
 *                 주어진 위치에 삽입하기 위해 이전 노드를 찾고, 그 이전 노드 이후에 새로운 노드를 삽입합니다.
 */
void insert(int pos, Item item)
{
    PayedProduct *new_pp, *prev;

    prev = get_entry(pos - 1);
    if (prev != NULL) {
        new_pp = (PayedProduct*)malloc(sizeof(PayedProduct));
        new_pp->item = item;
        new_pp->prev = NULL;
        new_pp->next = NULL;

        insert_next(prev, new_pp);
    }
}

// dequeue 함수 정의
/*
 * @Name : dequeue
 * @Description : 덱의 가장 앞쪽에 있는 상품을 제거하고 해당 상품 정보를 반환합니다.
 */
Item dequeue()
{
    Product* p;
    Item item;

    p = front;
    front = front->link;

    if (front == NULL)
        rear = NULL;

    item= p->item;
    free(p);

    return item;
}

// remove_curr 함수 정의
/*
 * @Name : remove_curr
 * @Param : pp - 삭제할 노드의 포인터
 * @Description : 주어진 노드를 리스트에서 삭제합니다.
 *                 삭제할 노드의 이전 노드와 다음 노드의 연결을 수정합니다.
 */
void remove_curr(PayedProduct *pp)
{
    if (pp->prev != NULL) pp->prev->next = pp->next;
    if (pp->next != NULL) pp->next->prev = pp->prev;
}

// delete 함수 정의
/*
 * @Name : delete
 * @Param : pos - 삭제할 노드의 위치 인덱스(0부터 시작)
 * @Description : 주어진 위치에 해당하는 노드를 삭제합니다.
 *                 주어진 위치에 해당하는 노드를 찾고, 해당 노드를 리스트에서 삭제합니다.
 */
void delete(int pos)
{
    PayedProduct* pp = get_entry(pos);
    if (pp != NULL)
        remove_curr(pp);
}

// peek 함수 정의
/*
 * @Name : peek
 * @Description : 덱의 가장 앞쪽에 있는 상품의 정보를 반환합니다.
 */
Item peek()
{
    return front->item;
}

// print_basket 함수 정의
/*
 * @Name : print_basket
 * @Description : 현재 장바구니에 담긴 상품들의 정보를 출력합니다.
 */
void print_basket()
{
    Product *p;
    printf("장바구 상품 수 : %d\n", basket_size());

    int count = 0;
    for (p = front; p != NULL; p = p->link)
        printf("%d. %s %d원\n", ++count, p->item.productName, p->item.price);
}

// print_payment_list 함수 정의
/*
 * @Name : print_payment_list
 * @Description : 현재 결제된 상품들의 정보를 출력합니다.
 */
void print_payment_list()
{
    PayedProduct* pp;
    printf("결제된 상품 수 : %d\n", payment_size());

    int count = 0;
    for (pp = get_head(); pp != NULL; pp = pp->next)
        printf("%d. %s %d원\n", ++count, pp->item.productName, pp->item.price);
}

// pay_for_product 함수 정의
/*
 * @Name : pay_for_product
 * @Param : item - 결제할 Item 구조체, pay - 결제 여부를 나타내는 문자('y' 또는 'n')
 * @Description : 주어진 Item을 결제합니다.
 *                 만약 결제를 진행할 경우, 잔액이 부족하면 에러 메시지를 출력합니다.
 *                 결제를 진행하지 않을 경우, 주어진 Item을 덱의 뒤쪽에 추가합니다.
 */
void pay_for_product(Item item, char pay)
{
    if(pay == 'y') {
        if (balance < item.price) {
            printf("잔액이 부족합니다.\n");
            return;
        }

        balance -= item.price;
        insert(payment_size() - 1, item);

        printf("잔액 : %d원\n", balance);
    } else {
        enqueue(item);
    }
}

// cancel_payment 함수 정의
/*
 * @Name : cancel_payment
 * @Param : pos - 취소할 상품의 인덱스(0부터 시작)
 * @Description : 주어진 인덱스에 해당하는 결제된 상품을 취소하고, 취소된 상품을 덱의 뒤쪽에 추가합니다.
 *                 또한, 취소된 상품의 가격을 잔액에 추가합니다.
 */
void cancel_payment(int pos)
{
    Item item = get_entry(pos)->item;
    delete(pos);

    enqueue(item);

    balance += item.price;

    printf("결제가 취소되었습니다.\n");
}

// print_status 함수 정의
/*
 * @Name : print_status
 * @Description : 현재 잔액과 장바구니, 결제 목록의 상태를 출력합니다.
 */
void print_status()
{
    printf("현재 잔액 : %d\n", balance);
    print_basket();
    print_payment_list();
}


int main() {
    int choice;
    char productName[MAX_PRODUCT_NAME_SIZE] = {0,};
    int price;
    char pay;
    int pos;

    printf("프로그램 시작\n");
    init_basket();
    init_payment_list();

    printf("1.상품 담기  2.상품 결제하기  3.결제 취소하기  4.현재 상태 확인  5.종료\n");

    while (1) {
        printf("원하는 기능의 번호를 입력하세요 : ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("상품 이름을 입력하세요 : ");
                scanf("%s", productName);

                printf("상품 가격을 입력하세요 : ");
                scanf("%d", &price);

                Item newItem;

                newItem.productName = productName;
                newItem.price = price;

                enqueue(newItem);

                break;
            case 2:
                for(int i = 0 ; i < 3 ; i++) {
                    if (is_basket_empty())
                        break;

                    Item item = dequeue();

                    getchar();
                    printf("%s %d원 결제(y/n) : ", item.productName, item.price);
                    scanf("%c", &pay);

                    pay_for_product(item, pay);
                }

                break;
            case 3:
                if (is_payment_empty() == 0) {
                    printf("취소할 상품이 없습니다.\n");
                    break;
                }

                printf("취소할 상품의 번호를 입력하세요(1~%d) : ", payment_size());
                scanf("%d", &pos);

                cancel_payment(pos-1); // 코드 작성
                break;
            case 4:
                print_status(); // 코드 작성
                break;
            case 5:
                printf("시스템 종료");
                return 1;
            default:
                printf("잘못된 입력입니다\n");
                break;
        }
    }
}