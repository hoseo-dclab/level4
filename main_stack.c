#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUCT_NAME_SIZE 20

int cash = 5000; // 충전은 1000원 씩

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

typedef struct LinkedStackNode {
    Item item;		       // 데이터 영역
    struct LinkedNode* link;		// 다음 노드를 가리키는 포인터 변수
} ReceiveProduct;

ReceiveProduct* top = NULL;

// init_basket 함수 정의
/*
 * @Name : init_basket
 * @Description : 장바구니를 초기화합니다. front와 rear를 NULL로 설정합니다.
 */
void init_basket() { front = rear = NULL; }

// init_payment_list 함수 정의
/*
 * @Name : init_payment_list
 * @Description : 결제 목록을 초기화합니다. org의 next를 NULL로 설정합니다.
 */
void init_payment_list() { org.next = NULL; }

// init_receive_stack 함수 정의
/*
 * @Name : init_receive_stack
 * @Description : 받은 상품 스택을 초기화합니다. top을 NULL로 설정합니다.
 */
void init_receive_stack() { top = NULL; }

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

// receive_size 함수 정의
/*
 * @Name : receive_size
 * @Description : 받은 상품의 수를 반환합니다.
 */
int receive_size()
{
    ReceiveProduct *rp;
    int count = 0;
    for (rp = top; rp != NULL; rp = rp->link)
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
    for (int i = -1; i != pos; i++, pp = (PayedProduct *) pp->next)
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

// is_receive_empty 함수 정의
/*
 * @Name : is_receive_empty
 * @Description : 반송 스택이 비어있는지 여부를 반환합니다.
 */
int is_receive_empty()
{
    return top == NULL;
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

// push 함수 정의
/*
 * @Name : push
 * @Param : item - 스택에 추가할 Item 구조체
 * @Description : 주어진 Item을 반송 스택의 최상위에 추가합니다.
 */
void push(Item item)
{
    ReceiveProduct * rp = (ReceiveProduct*)malloc(sizeof(ReceiveProduct));	// 동작 설명 :

    rp->item = item;
    rp->link = (struct LinkedNode *) top;

    top = rp;
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

// delete_payment 함수 정의
/*
 * @Name : delete_payment
 * @Param : pos - 삭제할 노드의 위치 인덱스(0부터 시작)
 * @Description : 주어진 위치에 해당하는 노드를 삭제합니다.
 *                 주어진 위치에 해당하는 노드를 찾고, 해당 노드를 리스트에서 삭제합니다.
 */
Item delete_payment(int pos)
{
    PayedProduct* pp = get_entry(pos);
    Item item = pp->item;

    remove_curr(pp);

    return item;
}

// pop 함수 정의
/*
 * @Name : pop
 * @Description : 반송 스택의 최상위 아이템을 제거하고 해당 아이템을 반환합니다.
 */
Item pop()
{
    ReceiveProduct * rp;

    rp = top;
    top = (ReceiveProduct *) rp->link;

    Item item = rp->item;
    free(rp);

    return item;
}

// peek 함수 정의
/*
 * @Name : peek
 * @Description : 덱의 가장 앞쪽에 있는 상품의 정보를 반환합니다.
 */
Item peek_payment_list()
{
    return front->item;
}

// peek_receive_stack 함수 정의
/*
 * @Name : peek_receive_stack
 * @Description : 반송 스택의 최상위 아이템을 조회합니다.
 */
Item peek_receive_stack()
{
    return top->item;
}

// print_basket 함수 정의
/*
 * @Name : print_basket
 * @Description : 현재 장바구니에 담긴 상품들의 정보를 출력합니다.
 */
void print_basket()
{
    Product *p;
    printf("장바구니 상품 수 : %d\n", basket_size());

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

// print_receive_stack 함수 정의
/*
 * @Name : print_receive_stack
 * @Description : 반송 스택의 아이템들을 출력합니다.
 */
void print_receive_stack()
{
    ReceiveProduct * rp;
    printf("받은 상품 수 : %d\n", receive_size());

    int count = 0;
    for (rp = (ReceiveProduct *) top; rp != NULL; rp = (ReceiveProduct *) rp->link)
        printf("%d. %s %d원\n", ++count, rp->item.productName, rp->item.price);
}

// pay_for_product 함수 정의
/*
 * @Name : pay_for_product
 * @Param : item - 결제할 Item 구조체, pay - 결제 여부를 나타내는 문자('y' 또는 'n')
 * @Description : 주어진 Item을 결제합니다.
 *                 잔액이 부족하면 에러 메시지를 출력합니다.
 *                 결제를 진행하지 않을 경우, 상품을 장바구니 제일 뒤로 보냅니다.
 * @Return : -1 - 결제 거부
 *            0 - 잔액 부족
 *            1 - 결제 성공
 */
int pay_for_product(Item item, char pay)
{
    if(pay == 'y') {
        // 코드작성
    } else {
        // 코드작성
    }
}

// receive_product 함수 정의
/*
 * @Name : receive_product
 * @Description : 주어진 결제 한 상품을 앞에서 부터 최대 3개 받습니다.
 * @Return : 가장 오래된 결제 상품
 */
Item receive_product()
{
    // 코드 작성
}

// cancel_payment 함수 정의
/*
 * @Name : cancel_payment
 * @Param : pos - 취소할 상품의 인덱스(0부터 시작)
 * @Description : 주어진 인덱스에 해당하는 결제된 상품을 취소하고, 취소된 상품을 덱의 뒤쪽에 추가합니다.
 *
 */
void cancel_payment(int pos)
{
    // 코드 작성
}

// send_back_product 함수 정의
/*
 * @Name : send_back_product
 * @Description : 반송된 상품을 장바구니로 되돌립니다. 잔액에 반송된 상품의 가격을 추가합니다.
 */
void send_back_product() {
    // 코드 작성
}

// print_status 함수 정의
/*
 * @Name : print_status
 * @Description : 현재 잔액과 장바구니, 결제 목록의 상태를 출력합니다.
 */
void print_status()
{
    printf("현재 잔액 : %d\n", cash);
    print_basket();
    print_payment_list();
    print_receive_stack();
}

int main() {
    int choice;
    char productName[MAX_PRODUCT_NAME_SIZE] = {0,};
    int price;
    char pay;
    char back;
    int pos;

    printf("프로그램 시작\n");
    init_basket();
    init_payment_list();
    init_receive_stack();

    printf("1.상품 담기 2.상품 결제 3.결제 취소  4.상품 배송 5.상품 반송 6.잔액 충전 7.현재 상태 확인 8.종료\n");

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
                    printf("%s %d 결제(y/n) : ", item.productName, item.price);
                    scanf("%c", &pay);

                    int pay_result = pay_for_product(item, pay); //코드 작성 1

                    if(pay_result == 0 )
                        printf("잔액이 부족합니다.\n");
                    else if (pay_result == 1 )
                        printf("잔액 : %d원\n", cash);
                    else
                        printf("해당 제품은 장바구니 끝으로 이동합니다.\n");
                }

                break;
            case 3:
                if (is_payment_empty() == 0) {
                    printf("취소할 상품이 없습니다.\n");
                    break;
                }

                printf("취소할 상품의 번호를 입력하세요(1~%d) : ", payment_size());
                scanf("%d", &pos);

                cancel_payment(pos-1); // 코드 작성 2
                printf("결제가 취소되었습니다.\n");
                break;
            case 4:
                printf("배송 시작\n");

                Item receive_item;

                for(int i = 0 ; i < 3 ; i++) {
                    if(get_entry(0) == NULL)
                        break;

                    receive_item = receive_product(); // 코드 작성 3

                    printf("%d. %s %d원\n", i+1, receive_item.productName, receive_item.price);
                }
                break;
            case 5:
                getchar();
                printf("가장 최근 받은 상품 반송(y/n) : ");
                scanf("%c", &back);

                if(is_receive_empty()) {
                    printf("반송 가능한 상품이 없습니다.\n");
                    break;
                }

                if(pay == 'y') {
                    send_back_product(); // 코드 작성 4
                    printf("반송된 상품은 장바구니로 돌아갑니다.\n");
                }

                break;
            case 6:
                cash += 1000;
                printf("현재 잔액 : %d\n", cash);
                break;
            case 7:
                print_status();
                break;
            case 8:
                printf("시스템 종료");
                return 1;
            default:
                printf("잘못된 입력입니다\n");
                break;
        }
    }
}