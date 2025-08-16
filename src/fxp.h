/*
fxp: lightweight math function parser and utility library
*/

/*
---------
CONSTANTS
---------
*/
#define STREND '\0'

/*
------
MACROS
------
*/

#define isOperator(expr) (expr == '+' || expr == '-' || expr == '*' || expr == '/' || expr == '^')
#define isBracket(expr) (expr == '(' || expr == ')' || expr == '[' || expr == ']')
#define len(arr) (int)(sizeof(arr)/sizeof(arr[0]))

#define appendChar(str, ch, len) str[len] = ch; str[len+1] = STREND

#define char2str(str, ch) str[0] = ch; str[1] = STREND

#define imax(a, b) (int) a > (int) b ? a : b

#define isnegative(str) (str[0] == '-' && str[1] != '\0' && isdigit(str[1])) 

#define evaluate_f(fox, x) evaluate_tree(fox->exprTree, x); 

/*
-----
ENUMS
-----
*/

typedef enum
{False, True}Bool; 

//type of data that a node contains in `val`
typedef enum{
    NODE_TYPE,  //Node contains another node
    STRING_TYPE //Node contains a string
}node_datatype;

typedef enum{
  NONE, 
  OPERATOR,
  OPERAND,
  CLOSED_BRACKET,
  OPEN_BRACKET, 
  FUNCTION 
} Expr_Token; 
/* 
Structs 
*/
typedef struct{
    
    void* next;
    void* val;

    void* lChild; 
    void* rChild;
    int level; 
}NODE; //multipurpose node

typedef struct
{
    NODE* top;
    int len;
    node_datatype type; 
}Stack;

/* structure for single variate functions*/
typedef struct
{
    NODE* exprTree; 

}FOX; // for f(x) or f o x 

typedef struct {
    const char* name;
    double (*func)(double);
} function_entry;

/* 
-------------
f o x METHODS
-------------
*/

/*
create a FOX struct from a string (math) expression.
NOTE: 
    can only create a FOX struct from expressions with a single variable 'x',
    multivariate functions or functions of another variable are not currently supported
Usage:  
    FOX* f = initfunc("3sin(x) + 5"); 
*/

FOX* initfunc(char* expr); 
void freeFox(FOX* f); 
/* 
-------------
STACK METHODS
-------------
*/
Stack* create_stack(node_datatype type);
NODE* create_node(void* val, node_datatype datatype);

void free_node(NODE*); 
void freeStack(Stack* stack);   //free all pointers
void push(Stack* stack, void* val); 

/*
returns val stored in top node 
NOTE: call free(val) when done using popped value
*/
void* pop(Stack* stack); 
void* peek(Stack* stack);

//debugging:
void printstack(Stack* stack); 

/* 
convert infix string expr to postfix expression using stack 

Usage:
    char postfix [200];
    int err = infix_to_postfix("x^2 + 4x + 3", postfix, 200); 

Parameters:
    char* expr     - string containing infix expression
    char* postfix  - string to write postfix expression to
    int buffersize - size of writeable buffer (i.e., how many bytes is allocated to 'postfix' variable)

Returns:
    int errcode:
        0           - no errors
        -999        - a error occured 
*/
int infix_to_postfix(char* expr, char* postfix, int buffersize); 
#define CONCAT_OVERFLOW(s1, s2, buffer) (strlen(s1) + strlen(s2) +1 > (size_t)buffer)

/*
Checks if a string is a "function operator" i.e., log, cos, sin, sqrt ...
as oposed to an operator represented by a char, like '+', '-', '*', '-' ...

The reason for the name is because "sin", "cos", etc, can be thought of as "string operators"
and '-', '+', etc as "character operators" in the context of the infix to postfix conversion algorithm
*/
Bool isStrOpr(char* expr); 
Bool isConstant(char* expr);
/* 
Compare char operators oper1 and opr2 by precedence
-1 lower precedence
0  equal precedence
1  higher precedence
*/
int cmpopr(char opr1, char opr2); 

/* 
-----------------------
EXPRESSION TREE METHODS 
-----------------------
*/

/* Free all memory allocated by the expression tree */
void freeTree(NODE* root);     
/* Create expression tree from postfix string */
NODE* create_expression_tree(char* postfix);

double evaluate_tree(NODE*node, double x); 

/* debugging */
void rprinttree(NODE* node, char* padding, char* pointer); 
void printTree(NODE* root);

/*
 * Apply operations on A and B.
 *
 * If is_stropr is true, applies operation only on B
 */
double eval(double A, double B, char* operation, Bool is_stropr, Bool* err);

/*
 * Linearly interpolate x coordinate between two points.
 * Usage: const double xprime = linear_interpolation_x()
 *
 * |  *  (x1, y1)
 * |
 * |
 * |         x (yPrime)
 * | 
 * |              * (x2, y2)
 * |------------------------- 
 */ 
double linear_interpolation_x(double x1, double y1, double x2, double y2, double yPrime);

Bool double_equal(double a, double b); 
