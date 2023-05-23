#ifndef _TREENODE_H_

#define _TREENODE_H_

#include "llvm/ADT/STLExtras.h"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <llvm/IR/Value.h>
#include <string>
#include <vector>

using namespace std;

class EmitContext;

class TreeNode {
public:
  TreeNode(int lineNo) : lineNo(lineNo) {}
  virtual ~TreeNode() {}
  virtual llvm::Value *emitter(EmitContext &emitContext) {
    return nullptr;
  }
  virtual void generateJson(string &s) {}
public:
  int lineNo;
};

class ExpressionNode : public TreeNode {
public:
  ExpressionNode(int lineNo) : TreeNode(lineNo) {}
  virtual llvm::Value *getAddr(EmitContext &emitContext) {
    return nullptr;
  }
  //virtual llvm::Value *emitter(EmitContext &emitContext);
};

class StatementNode : public TreeNode {
public:
  StatementNode(int lineNo) : TreeNode(lineNo) {}
  //virtual llvm::Value *emitter(EmitContext &emitContext);
};


class CharNode: public ExpressionNode {
  public:
    string value;

  public:
    CharNode(string value, int lineNo) : ExpressionNode(lineNo),value(value) {}
    llvm::Value *emitter(EmitContext &emitContext);
};

class BlockNode: public ExpressionNode {
  public:
    vector<StatementNode *> statementList;
  public:
    BlockNode(int lineNo):ExpressionNode(lineNo){}
    BlockNode(vector<StatementNode *> statementList,int lineNo):ExpressionNode(lineNo),statementList(statementList) {}
    llvm::Value *emitter(EmitContext &emitContext);
};

class BinaryOpNode: public ExpressionNode {
  public:
    int op;
    ExpressionNode& l;
    ExpressionNode& r;
  public:
    BinaryOpNode(int op,ExpressionNode& l,ExpressionNode& r,int lineNo):op(op),l(l),r(r),ExpressionNode(lineNo) {}
    llvm::Value *emitter(EmitContext &emitContext);
};

// indentifierNode "sdfsdf"
class AssignmentNode: public ExpressionNode {
  public:
    int op;
    IdentifierNode& l;
    ExpressionNode& r;
  public:
    AssignmentNode(IdentifierNode& l,ExpressionNode& r,int lineNo):l(l),r(r),ExpressionNode(lineNo) {}
    llvm::Value *emitter(EmitContext &emitContext);
};


class ArrayElementNode: public ExpressionNode {
  public:
    int op;
    IdentifierNode& l;
    ExpressionNode& r;
  public:
    AssignmentNode(IdentifierNode& l,ExpressionNode& r,int lineNo):l(l),r(r),ExpressionNode(lineNo) {}
    llvm::Value *emitter(EmitContext &emitContext);
};


class ArrayElementNode : public ExpressionNode {   
  public:
    IdentifierNode& identifier;
    ExpressionNode& index;
  public:
    ArrayElementNode(IdentifierNode& identifier, ExpressionNode &index, int lineNo) : ExpressionNode(lineNo), identifier(identifier), index(index) {}
    llvm::Value *emitter(EmitContext &emitContext);
    llvm::Value *getAddr(EmitContext &emitContext);
    void generateJson(string &s);
};

class ArrayElementAssignNode : public ExpressionNode {   //identifier[expression] 表示数组中某个元素
public:
  ArrayElementAssignNode(IdentifierNode &identifier, ExpressionNode &index, ExpressionNode &rhs, int lineNo) : ExpressionNode(lineNo), identifier(identifier), index(index), rhs(rhs) {}
  llvm::Value *emitter(EmitContext &emitContext);
public:
  IdentifierNode& identifier;
  ExpressionNode& index;
  ExpressionNode& r;
};


#endif