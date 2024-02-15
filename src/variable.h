#define VARIABLE

enum DataType{
    INTEGER,
    FLOAT,
    BOOLEAN,
    POINTER,
    LINE,
    OPERATOR
};

enum Operator{
    PLUS,
    MOINS,
    UMOINS,
    FOIS,
    DIVISE,
    MODULO,
    NOT,
    AND,
    OR,
    EQ,
    NEQ,
    GE,
    LE,
    GT,
    LT
};

typedef struct{
    enum DataType dataType;
    union{
        int integer;
        float floattant;
        void* pointer;
        int line;
        enum Operator operator;
        int boolean;
    };
} Variable;