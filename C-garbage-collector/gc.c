#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define STACK_MAX 256
#define INITIAL_THRESHOLD 5


typedef enum{
    INT,
    DOUBLE,
    PAIR
}ObjectType;


typedef struct fObject{

    struct fObject* next;

    // Mark
    unsigned char marked;

    // Type
    ObjectType type;

    union{
        /* Value for INT*/
        int intValue;
        /* Value for DOUBLE*/
        double doubleValue;
        /* Value for PAIR_INT*/
        struct{
            struct fObject* head;
            struct fObject* tail;
        };
    };
}Object;


// Create a VM as an array of Object
typedef struct{

    int numObject; // Total allocated Object from last gc
    int maxObject; // Max number of object to activate the gc

    Object* firstObject; // Objects created

    Object* stack[STACK_MAX]; // Object in stack
    int stackSize;
}VM;


VM* newVM();
void push(VM* vm, Object* obj);
Object* pop(VM* vm);
void gc(VM* vm);
Object* createObject(VM* vm, ObjectType objType);
void pushInt(VM* vm, int value);
void pushDouble(VM* vm, double value);
void pushPair(VM* vm);
void mark(Object* obj);
void sweep(VM* vm);
void printStack(VM* vm);
void printAllObjects(VM* vm);
void printObject(Object* obj, bool address);


VM* newVM(){
    VM* vm = malloc(sizeof(VM));

    vm->numObject = 0;
    vm->maxObject = INITIAL_THRESHOLD;

    vm->firstObject = NULL;

    vm->stackSize = 0;

    puts("Virtual Machine ready.\n");
    return vm;
}

Object* createObject(VM* vm, ObjectType objType){

    if(vm->numObject == vm->maxObject) gc(vm);

    Object* obj = malloc(sizeof(Object));
    obj->marked = 0;
    obj->type = objType;
    obj->next = vm->firstObject;

    vm->firstObject = obj;
    vm->numObject++;
    return obj;
}

Object* pop(VM* vm){
    assert(vm->stackSize > 0);
    return vm->stack[--vm->stackSize];
}

void pushInt(VM* vm, int value){
    Object* obj = createObject(vm, INT);
    obj->intValue = value;
    push(vm, obj);
}

void pushDouble(VM* vm, double value){
    Object* obj = createObject(vm, DOUBLE);
    obj->doubleValue = value;
    push(vm, obj);
}

void pushPair(VM* vm){
    Object* obj = createObject(vm, PAIR);
    obj->tail = pop(vm);
    obj->head = pop(vm);
    push(vm,obj);
}

void push(VM* vm, Object* obj){
    assert(vm->stackSize < STACK_MAX);
    vm->stack[vm->stackSize++] = obj;
}

void markAll(VM* vm){
    for(size_t i=0; i<vm->stackSize; i++){
        mark(vm->stack[i]);
    }
}

void mark(Object* obj){
    if(obj->marked) return; // for no loop

    obj->marked = 1;

    if(obj->type == PAIR){
        mark(obj->tail);
        mark(obj->head);
    }
}

void sweep(VM* vm) {
    Object** obj = &vm->firstObject;

    while (*obj != NULL) {
        if ((*obj)->marked == 0) {
            Object* unreached = *obj;
            *obj = (*obj)->next;  // aggiorna la lista

            free(unreached);
            vm->numObject--;
        } else {
            (*obj)->marked = 0;
            obj = &(*obj)->next;
        }
    }
}

void printObject(Object* obj, bool address){

    switch(obj->type){

        case INT:
            if(address){
                printf("I-[%d](%d)", obj->intValue, obj);
            }else{
                printf("I-[%d]", obj->intValue);
            }
            break;

        case DOUBLE:
            if(address){
                printf("D-[%.2f](%d)", obj->doubleValue, obj);
            }else{
                printf("D-[%.2f]", obj->doubleValue);
            }
            break;
        
        case PAIR:
            printf("P-[%d][%d]", obj->head, obj->tail);
    }
}

void printStack(VM* vm){
    puts("Stack status:");

    for(size_t i=0; i<vm->stackSize; i++){

        printObject(vm->stack[i], false);
        if(i != (vm->stackSize)-1) printf(" -> ");
    }
    printf("\n\n");
}

void printAllObjects(VM* vm){
    puts("Objects status:");

    Object* obj = vm->firstObject;
    while(obj != NULL){
        printObject(obj, true);
        printf("\n");
        obj = obj->next;
    }
    puts("");
}

void gc(VM* vm) {
    puts("Garbage collector activated.");

    puts("Before:");
    printStack(vm);
    printAllObjects(vm);

    markAll(vm);
    sweep(vm);
    vm->numObject = 0;

    puts("After:");
    printStack(vm);
    printAllObjects(vm);
}

void main(){
    
    printf("Activating threshold: %d\n", INITIAL_THRESHOLD);
    
    VM* vm = newVM();

    pushInt(vm, 10);
    pushDouble(vm, 3.23);
    pushInt(vm, 1);
    pushDouble(vm, 2.24);
    pop(vm);
    pushPair(vm);
    pushInt(vm, 13);
    pushInt(vm, 24);
    pushDouble(vm, 34.1);
    pushPair(vm);
    pop(vm);
    pushInt(vm, 2);
    pushInt(vm, 6);

    puts("Final:");
    printStack(vm);
    printAllObjects(vm);
}
