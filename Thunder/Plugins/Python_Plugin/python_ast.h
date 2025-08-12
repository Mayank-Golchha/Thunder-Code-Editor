#ifndef PYTHON_AST_H
#define PYTHON_AST_H

#include <unordered_set>
#include <vector>

#include "python_tokens.h"

enum class PYTHON_DECLARATION_TYPE{
    PYTHON_FUNCTION,
    PYTHON_VARIABLE,
    PYTHON_PARAMTER,
    PYTHON_CLASS
};


typedef struct PythonStack* pystack;

struct PythonLoc{
    int startLine;
    int endLine;
    int start;
    int end;
};

struct PythonStack{
    int data;
    pystack next;
};

struct PythonBlock;

struct PythonDeclaration{
    std::string name;
    int usage; //will count how much time it is being used
    std::vector<std::string> parameters; //for function
    int isreturning; //will function return something
    PythonBlock* block; //if function then will be a block else null
    PYTHON_DECLARATION_TYPE type;

    PythonDeclaration(std::string name_,PYTHON_DECLARATION_TYPE type_){
        name = name_;
        type = type_;
        isreturning = 0;
        usage = 0;
        block = NULL;
    }

    PythonDeclaration(std::string name_,PYTHON_DECLARATION_TYPE type_,PythonBlock* block,std::vector<std::string>* params){
        name = name_;
        type = type_;
        isreturning = 0;
        usage = 0;
        block = block;
        parameters = *params;
    }

};

struct PythonBlock{
    std::unordered_set<PythonDeclaration*> declaration_statements;
    std::unordered_set<Python_Token*> expression_statements;
    //  PythonBlock* sub_blocks;
    PythonBlock* parent_block = NULL;
};

PythonLoc* python_location_struct(int startX,int startY,int endX,int endY){
    PythonLoc* loc = new PythonLoc{startY,endY,startX,endX};
    return loc;
}

pystack newPythonStack(int data,pystack next){
    return new PythonStack{data,next};
}

void pushPythonStack(pystack* stack,int data){
    *stack = newPythonStack(data,*stack);
}

int popPythonStack(pystack* stack){
    int d = (*stack)->data;
    pystack s = *stack;
    *stack = (*stack)->next;
    // delete *stack;
    return d;
}

#endif