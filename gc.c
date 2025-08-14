#define STACK_MAX 256

// Create objects type in the VM
typedef enum{
    INT,
    DOUBLE,
    PAIR
}ObjectType;

// Create objects
typedef struct fObject{

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
        typedef struct{
            struct fObject* head;
            struct fObject* tail
        };
    };
}Object;

// Create a VM as an array of Object
typedef struct{
    Object* stack[STACK_MAX]
    int stackSize;
}VM;

VM* newVM(){
    VM* vm = malloc(sizeof(VM));
    vm->stackSize = 0;
    return vm;
}

void push(VM* vm, Object* obj){
    assert(vm->stackSize < STACK_MAX, "Stack overflow!");
    vm->stack[vm->stackSize++] = value;
}

Object* pop(VM* vm){
    assert(vm->stackSize > 0, "Stack underflow!");
    vm->stack[--vm->stackSize];
}

Object* createObject(ObjectType objType){
    Object* obj = malloc(sizeof(Object));
    obj->marked = 0;
    obj->type = objType;
    return obj;
}

void pushInt(VM* vm, int value){
    Object* obj = createObject(INT);
    obj->intValue = value;
    push(vm, obj);
}

void pushDouble(VM* vm, double value){
    Object* obj = createObject(DOUBLE);
    obj->intValue = value;
    push(vm, obj);
}

void pushPair(VM* vm){
    Object* obj = createObject(PAIR);
    obj->tail = pop(vm);
    obj->head = pop(vm);

    push(vm,obj);
}

void markAll(VM* vm){

    for(size_t i=0; i<vm->stackSize; i++){
        mark(vm, stack[i])
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

