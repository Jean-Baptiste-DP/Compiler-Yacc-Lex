typedef struct intStack{
    int value;
    struct intStack *next;
}*IntStack;

typedef struct stack{
    IntStack stack;
}*Stack;

/* --- Stack --- */

Stack newStack();
bool StackisEmpty(Stack stack);
void freeIntStack(IntStack stack);
void freeStack(Stack stack);
void changeStack(Stack stack, IntStack intStack);
void removeStack(Stack stack);
void appendInt(Stack stack, int value);
int removeLastValue(Stack stack);
void displayIntStack(IntStack stack);
void displayStack(Stack stack);