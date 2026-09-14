#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*------------------------*/
/*        Structures      */
/*------------------------*/

typedef struct Digit {
    struct Digit     * col; 
    struct Digit     * next;       
} Digit;

typedef Digit * Colossal;

typedef struct StackNode{
    Colossal           k; 
    struct StackNode * next;
}StackNode;

typedef struct Stack{
    StackNode        * top;
}Stack;

/*------------------------*/
/* Functions Declarations */
/*------------------------*/

void FreeColossal(Colossal  k);

Colossal Addition(Colossal a, Colossal b);

Colossal Merge(Colossal exp, Colossal rest); 

/*------------------------*/
/*  Functions for stack   */
/*------------------------*/

/**
 * Initializes and allocates memory for a new empty stack.
 * Returns a pointer to the created Stack, or NULL if allocation fails.
 */
Stack * StackInit(void){
    Stack * stack = (Stack *)malloc(sizeof(Stack));
    if (!stack) return NULL;
    
    stack->top = NULL;  
    return stack;
}

/**
 * Checks if the given stack is empty or uninitialized.
 * Returns true if the stack is empty, otherwise false.
 */
bool IsEmpty(Stack * stack){
    if (stack == NULL) return true;   
    return (stack->top == NULL); 
}

/**
 * Removes and returns the top element (Colossal number) from the stack.
 * Returns NULL if the stack is empty.
 */
Colossal Pop(Stack * stack){
    if (IsEmpty(stack)) return NULL;  

    StackNode * oldtop = stack->top;
    Colossal oldkol = oldtop->k;

    stack->top = stack->top->next;

    free(oldtop);
    return oldkol;
}

/**
 * Frees all elements currently held in the stack and then frees the stack itself.
 * Used for memory cleanup at the end of the program.
 */
void clearStack(Stack * stack){
    if (!stack) return;

    while (!IsEmpty(stack)) {
        FreeColossal(Pop(stack));
    }
    free(stack);
}

/**
 * Pushes a given Colossal number onto the top of the stack.
 */
void Push(Stack * stack, Colossal Col){
    StackNode * new = (StackNode *)malloc(sizeof(StackNode));
    if(!new) return;

    new->k = Col;
    new->next = stack->top;
    stack->top = new;
}

/**
 * Returns the top Colossal number from the stack without removing it.
 * Returns NULL if the stack is empty.
 */
Colossal Top(Stack * stack){
    if (IsEmpty(stack)) return NULL;  

    return stack->top->k;
}

/*------------------------------*/
/* Help functions for colossals */
/*------------------------------*/

/**
 * Creates a new digit for a Colossal number, linking an exponent (col) 
 * with the rest of the number structure (rest).
 */
Colossal NewDigit(Colossal col, Colossal rest){
    Digit * c = (Digit *) malloc(sizeof(Digit));
    Colossal result = NULL;

    if (c) {
        c->col = col;
        c->next = rest;
        result = c;
    }

    return result;
}

/**
 * Recursively frees the memory allocated for a Colossal number 
 * and all its sub-digits.
 */
void FreeColossal(Colossal k) {
    if (k == NULL) return;
    FreeColossal(k->col);
    FreeColossal(k->next);
    free(k);
}

/**
 * Creates and returns a deep copy of the given Colossal number.
 */
Colossal CopyColossal(Colossal k){
    if (!k) return NULL;
    return NewDigit(CopyColossal(k->col), CopyColossal(k->next));
}

/**
 * Recursively prints the textual representation of a Colossal number.
 * Translates the internal structure back to the '1' and '0' bracket format.
 */
void PrintColossal(Colossal k) {
    if (!k) {
        putchar('0');
        return;
    }
    putchar('1');
    PrintColossal(k->col);
    PrintColossal(k->next);
}

/**
 * Recursively parses a textual representation of a Colossal number ('1' and '0')
 * from standard input and constructs the corresponding memory structure.
 */
Colossal ParseColossal(void) {
    int c = getchar();

    Colossal result = NULL;

    if(c == '1'){
        Colossal col = ParseColossal();
        Colossal rest = ParseColossal();
        Colossal singledigit = NewDigit(col, NULL);

        result = Addition(singledigit, rest);

        FreeColossal(singledigit);
        FreeColossal(rest);
    }
    return result;
}

/**
 * Recursively compares two Colossal numbers based on their values.
 * Returns: 0 if equal, -1 if first < second, 1 if first > second.
 */
int Comparison(Colossal first, Colossal second){
    if (!first && !second) return  0; // first = second ->  0
    if (!first)            return -1; // first < second -> -1
    if (!second)           return  1; // first > second ->  1

    int result = Comparison(first->col, second->col);

    if(result != 0) return result;

    return Comparison(first->next, second->next);
}

/**
 * Initializes and returns a Colossal number representing the value 1.
 * Value 1 is mathematically 2^0, represented by an empty exponent.
 */
Colossal initOne(void){
    return NewDigit(NULL, NULL);
}

/*------------------------------*/
/*    Arithmetic Operations     */
/*------------------------------*/

/**
 * Adds two Colossal numbers recursively. Merges components and handles 
 * carry-over operations (when exponents match, 2^x + 2^x = 2^(x+1)).
 */
Colossal Addition(Colossal a, Colossal b) {
    if (!a) return CopyColossal(b);
    if (!b) return CopyColossal(a);

    int cmp = Comparison(a->col, b->col);

    if (cmp == 0) {
        Colossal one = initOne();
        Colossal nextExp = Addition(a->col, one);
        FreeColossal(one);

        Colossal restSum = Addition(a->next, b->next);

        Colossal carry = NewDigit(nextExp, NULL);
        Colossal result = Addition(carry, restSum);

        FreeColossal(carry);
        FreeColossal(restSum);
        return result;
    }

    if (cmp == 1) {
        return Merge(CopyColossal(a->col), Addition(a->next, b));
    }

    return Merge(CopyColossal(b->col), Addition(a, b->next));
}

/**
 * Helper function for addition. Merges an exponent digit with the rest 
 * of a Colossal number. Resolves collisions if the exponent already exists.
 */
Colossal Merge(Colossal exp, Colossal rest) {
    if (rest && Comparison(exp, rest->col) == 0) {
        Colossal digit = NewDigit(exp, NULL);
        Colossal result = Addition(digit, rest);
        FreeColossal(digit);
        FreeColossal(rest);
        return result;
    }
    return NewDigit(exp, rest);
}

/**
 * Multiplies two Colossal numbers.
 * Leverages the mathematical property that 2^a * 2^b = 2^(a+b), 
 * computing cross-additions for all exponents and summing the partial results.
 */
Colossal Multiplication(Colossal a, Colossal b){
    Colossal result = NULL;

    for(Digit * i = a; i != NULL; i = i->next){ 
        for(Digit * j = b; j != NULL; j = j->next){

            // a_i + b_j
            Colossal exp = Addition(i->col, j->col);

            // 2^(a_i + b_j)
            Colossal twotopowerexp = NewDigit(exp, NULL);

            Colossal new = Addition(result, twotopowerexp);

            FreeColossal(result);
            FreeColossal(twotopowerexp);

            result = new;
        }
    }
    return result; 
}

/*-------------------------------*/
/*  Normalization & Input Loop   */
/*-------------------------------*/

/**
 * Normalizes a Colossal number by restructuring it so that all digits 
 * are strictly sorted in descending order and any duplicate exponents 
 * are merged via addition.
 */
Colossal Normalize(Colossal k) {
    Colossal result = NULL;

    while(k) {
        Colossal next = k->next;
        Colossal exp = Normalize(k->col);

        k->col = NULL;
        k->next = NULL;
        free(k);

        Colossal new = NewDigit(exp, NULL);
        Colossal old = result;
        result = Addition(old, new);

        FreeColossal(old);
        FreeColossal(new);

        k = next;
    }
    return result;
}

/**
 * Main execution loop for the calculator. Reads operations (+, *, ^, :, .) 
 * and Colossal number formats from standard input, dynamically managing the stack.
 */
void LoadingData(Stack * s){
    int c;
    while((c = getchar()) != EOF){
        if(c == '0' || c == '1'){
            ungetc(c, stdin);
            Push(s, Normalize(ParseColossal()));
        } 
        else if(c == '.'){
            Colossal k = Pop(s);
            PrintColossal(k);   
            printf("\n");
            FreeColossal(k);
        }
        else if(c == ':'){
            Push(s, CopyColossal(Top(s)));
        }
        else if(c == '^'){
            Colossal k = Pop(s);
            Colossal m = Normalize(NewDigit(k, NULL));
            Push(s, m);
        }
        else if(c == '+' || c == '*'){ 
            Colossal k = Pop(s);
            Colossal m = Pop(s);
            if(c == '+'){
                Push(s, Addition(k, m));  
            }
            if(c == '*'){
                Push(s, Multiplication(k, m));  
            }
            FreeColossal(k); 
            FreeColossal(m);  
        }
    }
}

/**
 * Program entry point. Initializes the stack, starts parsing the input stream, 
 * and ensures all memory is properly freed upon termination.
 */
int main(void){
    Stack * s = StackInit();
    LoadingData(s);
    clearStack(s);
    return 0;
}
