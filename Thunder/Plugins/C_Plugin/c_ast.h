#ifndef C_AST_H
#define C_AST_H


#include <unordered_set>
#include <vector>

#include "C_tokens.h"

enum class C_DECLARATION_TYPE{
    C_FUNCTION,
    C_VARIABLE,
    C_PARAMTER,
    C_STRUCT
};


typedef struct CStack* cstack;


struct CStack{
    int data;
    cstack next;
};

struct CBlock;

struct CDeclaration{
    std::string name;
    int usage; //will count how much time it is being used
    std::vector<std::string> parameters; //for function
    int isreturning; //will function return something
    CBlock* block; //if function then will be a block else null
    C_DECLARATION_TYPE type;

    CDeclaration(std::string name_,C_DECLARATION_TYPE type_){
        name = name_;
        type = type_;
        isreturning = 0;
        usage = 0;
        block = NULL;
    }

    CDeclaration(std::string name_,C_DECLARATION_TYPE type_,CBlock* block,std::vector<std::string>* params){
        name = name_;
        type = type_;
        isreturning = 0;
        usage = 0;
        block = block;
        parameters = *params;
    }

};

struct CBlock{
    std::unordered_set<CDeclaration*> declaration_statements;
    std::unordered_set<C_Token*> expression_statements;
    //  PythonBlock* sub_blocks;
    CBlock* parent_block = NULL;
};


cstack newPythonStack(int data,cstack next){
    return new CStack{data,next};
}

void pushCStack(cstack* stack,int data){
    *stack = newPythonStack(data,*stack);
}

int popCStack(cstack* stack){
    int d = (*stack)->data;
    cstack s = *stack;
    *stack = (*stack)->next;
    // delete *stack;
    return d;
}


#endif 
