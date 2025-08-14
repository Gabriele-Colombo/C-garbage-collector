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

    int numObject; // total allocated Object
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
void printObjects(VM* vm);
void printObject(Object* obj);


VM* newVM(){
    VM* vm = malloc(sizeof(VM));
    vm->firstObject = NULL;
    vm->stackSize = 0;
    vm->numObject = 0;
    vm->maxObject = INITIAL_THRESHOLD;

    puts("Virtual Machine ready.\n");
    return vm;
}

Object* createObject(VM* vm, ObjectType objType){

    if(vm->numObject == vm->maxObject) gc(vm);

    Object* obj = malloc(sizeof(Object));

    obj->next = vm->firstObject;
    vm->firstObject = obj;

    obj->marked = 0;
    obj->type = objType;

    vm->numObject++;
    return obj;
}

Object* pop(VM* vm){
    assert(vm->stackSize > 0);
    Object* obj = vm->stack[--vm->stackSize];
    puts("Object poped.");
    return obj;
}

void pushInt(VM* vm, int value){
    Object* obj = createObject(vm, INT);
    obj->intValue = value;
    push(vm, obj);

    printf("Object pushed: %d\n", value);
    printf("Number of objects: %d\n", vm->numObject);
}

void pushDouble(VM* vm, double value){
    Object* obj = createObject(vm, DOUBLE);
    obj->intValue = value;
    push(vm, obj);

    printf("Object pushed: %f\n", value);
    printf("Number of objects: %d\n", vm->numObject);
}

void pushPair(VM* vm){
    Object* obj = createObject(vm, PAIR);
    obj->tail = pop(vm);
    obj->head = pop(vm);
    push(vm,obj);

    printf("Object pushed:");
    printObject(obj);
    puts("");
    printf("Number of objects: %d\n", vm->numObject);
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

void printObject(Object* obj){

    if(obj->type == INT){
        printf("[%d]->", obj->intValue);
        return;
    }
    
    if(obj->type == DOUBLE){
        printf("[%f]->", obj->doubleValue);
        return;
    }

    printObject(obj->head);
    printObject(obj->tail);
}

void printStack(VM* vm){
    for(size_t i=0; i<vm->stackSize; i++){
        printObject(vm->stack[i]);
    }
}

void printObjects(VM* vm){

    Object* obj = vm->firstObject;
    while(obj != NULL){
        printObject(obj);
        obj = obj->next;
    }
}

void gc(VM* vm) {
    puts("Garbage collector activated.");
    printStack(vm);
    puts("");
    printObjects(vm);
    puts("");

    markAll(vm);
    sweep(vm);

    printStack(vm);
    puts("");
    printObjects(vm);
    puts("");
}

void main(){
    
    printf("Activating threshold: %d\n", INITIAL_THRESHOLD);
    
    VM* vm =newVM();

    pushInt(vm, 10);
    pushInt(vm, 3);
    pushInt(vm, 1);
    pushInt(vm, 2);
    pop(vm);
    pushInt(vm, 12);
    pushInt(vm, 13);
}