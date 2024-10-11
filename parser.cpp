#include "Parser.hpp"

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {
}

bool Parser::parse() {
  return Program();
}

Token Parser::currToken() {
  return tokens[current];
}

Token Parser::nextToken() {
  return tokens[current++];
}

bool Parser::nonTerminal(std::string name) {
  if (currToken().name == name) {
    nextToken();
    return true;
  }
  return false;
}

/*
  Type' -> [ ] Type'
  Type' -> ε
*/
bool Parser::TypePrime() {
  if (nonTerminal("Token_[") && nonTerminal("Token_]") && TypePrime()) {
    return true;
  }
  return true;
}

/*
  BasicType -> IntType
  BasicType -> BoolType
  BasicType -> CharType
  BasicType -> StringType
  BasicType -> VoidType
*/
bool Parser::BasicType() {
  if (nonTerminal("IntType") || nonTerminal("BoolType") || 
      nonTerminal("CharType") || nonTerminal("StringType") || 
      nonTerminal("VoidType")) {
    return true;
  }
  return false;
}

/*
  Type->BasicType Type'
*/
bool Parser::Type() {
  if (BasicType() && TypePrime()) {
    return true;
  }
  return false;
}

/*
  Function -> Type Identifier (Params) { StmtList }
*/
bool Parser::Function() {
  if (Type() && 
      nonTerminal("Token_ID") && 
      nonTerminal("Token_(") && 
      Params() && 
      nonTerminal("Token_)") && 
      CompoundStmt()) {
    return true;
  }
  return false;
}

/*
  VarDecl' -> ;
  VarDecl' -> = Expression ;
*/
bool Parser::VarDeclPrime() {
  if (nonTerminal("Token_;")) {
    return true;
  }
  else if (nonTerminal("Token_=") && Expression() && nonTerminal("Token_;")) {
    return true;
  }
  return false;
}

/*
  VarDecl -> Type Identifier VarDecl'
*/
bool Parser::VarDecl() {
  if (Type() && nonTerminal("Token_ID") && VarDeclPrime()) {
    return true;
  }
  return false;
}

/*
  Declaration -> [ Function ]
  Declaration -> VarDecl
*/
bool Parser::Declaration() {
  if (nonTerminal("Token_[") && Function() && nonTerminal("Token_]")) {
    return true;
  }
  else if (VarDecl()) {
    return true;
  }
  return false;
}

/*
  Program' -> Declaration Program'
  Program' -> ε
*/
bool Parser::ProgramPrime() {
  if (Declaration() && ProgramPrime()) {
    return true;
  }
  return true;
}

/*
  Program -> Declaration Program'
*/
bool Parser::Program() {
  if (Declaration() && ProgramPrime()) {
    return true;
  }
  return false;
}

////////////////////////////////////////////////////////////

/*
  Params -> ParamList
  Params -> ε
*/
bool Parser::Params() {
  if (ParamList()) {
    return true;
  }
  return true;
}

/*
  ParamList -> Type Identifier ParamList'
*/
bool Parser::ParamList() {
  if (Type() && nonTerminal("Token_ID") && ParamListPrime()) {
    return true;
  }
  return false;
}

/*
  ParamList' -> , Type Identifier ParamList'
  ParamList' -> ε
*/
bool Parser::ParamListPrime() {
  if (nonTerminal("Token_,") && Type() && nonTerminal("Token_ID") && ParamListPrime()) {
    return true;
  }
  return true;
}

/*
  StmtList -> Statement StmtList'
*/
bool Parser::StmtList() {
  if (Statement() && StmtListPrime()) {
    return true;
  }
  return false;
}

/*
  StmtList' -> Statement StmtList'
  StmtList' -> ε
*/
bool Parser::StmtListPrime() {
  if (Statement() && StmtListPrime()) {
    return true;
  }
  return true;
}

/*
  CompoundStmt -> { StmtList }
*/
bool Parser::CompoundStmt() {
  if (nonTerminal("Token_{") && StmtList() && nonTerminal("Token_}")) {
    return true;
  }
  return false;
}

/*
  Statement -> VarDecl
  Statement -> IfStmt
  Statement -> ForStmt
  Statement -> ReturnStmt
  Statement -> ExprStmt
  Statement -> PrintStmt
  Statement -> { StmtList }
*/
bool Parser::Statement() {
  //return VarDecl() || IfStmt() || ForStmt() || ReturnStmt() || ExprStmt() || PrintStmt() || CompoundStmt();
  return false;
}

