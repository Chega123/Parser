#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

class Token {
public:
  std::string name, text;
  size_t line, col;
  
};

class Parser {
public:
  Parser(const std::vector<Token>& tokens);
  bool parse();

private:
  std::vector<Token> tokens;
  size_t current;

  Token currToken();
  Token nextToken();

  bool nonTerminal(std::string name);
  bool Program();
  bool ProgramPrime();
  bool Declaration();
  bool VarDecl();
  bool VarDeclPrime();
  bool Type();
  bool TypePrime();
  bool BasicType();
  bool Function();
  bool Params();
  bool ParamList();
  bool ParamListPrime();
  bool CompoundStmt();
  bool StmtList();
  bool StmtListPrime();
  bool Statement();
  bool IfStmt();
  bool ForStmt();
  bool AuxIf();
  bool ReturnStmt();
  bool PrintStmt();
  bool ExprStmt();
  bool ExprList();
  bool ExprListPrime();
  bool Expression();
  bool AuxExpression();
  bool OrExpr();
  bool OrExprPrime();
  bool AndExpr();
  bool AndExprPrime();
  bool EqExpr();
  bool EqExprprime();
  bool RelExpr();
  bool RelExprPrime();
  bool Expr();
  bool ExprPrime();
  bool Term();
  bool TermPrime();
  bool Unary();
  bool Factor();
  bool FactorPrime();
  bool Primary();
  bool AuxPrimary();
};

#endif // PARSER_HPP
