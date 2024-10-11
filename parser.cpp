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

/*
  Type' -> [ ] Type'
  Type' -> ε
*/
bool Parser::TypePrime() {
  if (currToken().name == "Token_[") {
    nextToken();
    if (currToken().name == "Token_]") {
      nextToken();
      return TypePrime();
    }
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
  if (currToken().name == "IntType" || currToken().name == "BoolType" || 
      currToken().name == "CharType" || currToken().name == "StringType" || 
      currToken().name == "VoidType") {
    nextToken();
    return true;
  }
  return false;
}

/*
  Type->BasicType Type'
*/
bool Parser::Type() {
  return BasicType() && TypePrime();
}

/*
  Function -> Type Identifier (Params) { StmtList }
*/
bool Parser::Function() {
  if (Type()) {
    if (currToken().name == "Token_ID") {
      nextToken();
      if (currToken().name == "Token_(") {
        nextToken();
        if (Params()) {
          if (currToken().name == "Token_)") {
            nextToken();
            return CompoundStmt();
          }
        }
      }
    }
  }
  return false;
}

/*
  VarDecl' -> ;
  VarDecl' -> = Expression ;
*/
bool Parser::VarDeclPrime() {
  if (currToken().name == "Token_;") {
    nextToken();
    return true;
  }
  else if (currToken().name == "Token_=") {
    nextToken();
    if (Expression()) {
      if (currToken().name == "Token_;") {
        nextToken();
        return true;
      }
    }
  }
  return false;
}

/*
  VarDecl -> Type Identifier VarDecl'
*/
bool Parser::VarDecl() {
  if (Type()) {
    if (currToken().name == "Token_ID") {
      nextToken();
      return VarDeclPrime();
    }
  }
  return false;
}

/*
  Declaration -> [ Function ]
  Declaration -> VarDecl
*/
bool Parser::Declaration() {
  if (currToken().name == "Token_[") {
    nextToken();
    if (Function()) {
      if (currToken().name == "Token_]") {
        nextToken();
        return true;
      }
      else return false;
    }
    else if (VarDecl()) {
      return true;
    }
  }
  return false;
}

/*
  Program' -> Declaration Program'
  Program' -> ε
*/
bool Parser::ProgramPrime() {
  if (Declaration()) { 
    return ProgramPrime();
  }
  return true;
}

/*
  Program -> Declaration Program'
*/
bool Parser::Program() {
  return Declaration() && ProgramPrime();
}

////////////////////////////////////////////////////////////

/*
  Params -> ParamList
  Params -> ε
*/
bool Parser::Params() {
  return ParamList() || true;
}

/*
  ParamList -> Type Identifier ParamList'
*/
bool Parser::ParamList() {
  if (Type()) {
    if (currToken().name == "Token_ID") {
      nextToken();
      return ParamListPrime();
    }
  }
}

/*
  ParamList' -> , Type Identifier ParamList'
  ParamList' -> ε
*/
bool Parser::ParamListPrime() {
  if (currToken().name == "Token_,") {
    nextToken();
    if (Type()) {
      if (currToken().name == "Token_ID") {
        nextToken();
        return ParamListPrime();
      }
    }
  }
  return true;
}

/*
  StmtList -> Statement StmtList'
*/
bool Parser::StmtList() {
  return Statement() && StmtListPrime();
}

/*
  StmtList' -> Statement StmtList'
  StmtList' -> ε
*/
bool Parser::StmtListPrime() {
  if (Statement()) {
    return StmtListPrime();
  }
  return true;
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

/*
  CompoundStmt -> { StmtList }
*/
bool Parser::CompoundStmt() {
  if (currToken().name == "Token_{") {
    nextToken();
    if (StmtList()) {
      if (currToken().name == "Token_}") {
        nextToken();
        return true;
      }
    }
  }
  return false;
}

bool Parser::Statement() {
  //return VarDecl() || IfStmt() || ForStmt() || ReturnStmt() || ExprStmt() || PrintStmt() || CompoundStmt();
  return false;
}

