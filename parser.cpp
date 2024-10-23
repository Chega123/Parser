#include "parser.hpp"

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
    if (current >= tokens.size()) {
        return true; 
    }
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
ExprStmt ::= Expression ;
ExprStmt ::= ;
*/
bool Parser::ExprStmt(){
  if(Expression()&& nonTerminal("Token_;")){
    return true;
  }
  else if(nonTerminal("Token_;")){
    return true;
  }
  return false;
}

/*
PrintStmt -> print ( ExprList ) ;
*/
bool Parser::PrintStmt(){
  if (nonTerminal("Token_print")&&
      nonTerminal("Token_(")&&
      ExprList()&&
      nonTerminal("Token_)")&&
      nonTerminal("Token_;")){
    return true;
  }
  return false;
}

/*
ReturnStmt -> return Expression ;
*/
bool Parser::ReturnStmt(){
  if (nonTerminal("Token_return")&&
      Expression()&& 
      nonTerminal("Token_;")){
    return true;
  }
  return false;
}

/*
ForStmt -> for ( ExprStmt Expression ; ) Statement
*/
bool Parser::ForStmt(){
  if (nonTerminal("Token_for") &&
      nonTerminal("Token_(") && 
      ExprStmt() && 
      Expression() && 
      nonTerminal("Token_;") && 
      nonTerminal("Token_)") &&
      Statement()) {
    return true;
  }
  return false;
}
/*
AuxIf -> else {Statement}
AuxIf -> ‘’
*/
bool Parser::AuxIf(){
  if (nonTerminal("Token_else") && 
      nonTerminal("Token_{") && 
      Statement() && 
      nonTerminal("Token_}")) {
    return true;
  }
  return true;
}

/*
IfStmt -> if ( Expression ) {Statement} AuxIf
*/
bool Parser::IfStmt() {
  if (nonTerminal("Token_if") && 
      nonTerminal("Token_(") && 
      Expression() && 
      nonTerminal("Token_)") && 
      nonTerminal("Token_{") && 
      Statement() && 
      nonTerminal("Token_}")&&
      AuxIf()) {
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
  Statement -> PrintStmt
*/
bool Parser::Statement() {
  return VarDecl() || IfStmt() || ForStmt() || ReturnStmt() || ExprStmt() || CompoundStmt() || PrintStmt();
}

/*
auxPrimary ::= ( ExprList )
auxPrimary ::= ''
*/
bool Parser::AuxPrimary(){
  if (nonTerminal("Token_(")  &&
      ExprList() && 
      nonTerminal("Token_)")){
    return true;
  }
  return true;
}

/*
Primary ::= Identifier auxPrimary
Primary ::= IntegerLiteral -->num
Primary ::= CharLiteral  --> 'char'
Primary ::= StringLiteral --> "text_string"
Primary ::= BooleanLiteral --> True o false
Primary ::= ( Expression )
*/

bool Parser::Primary(){
  if(nonTerminal("Token_ID") && AuxPrimary()){
    return true;
  }
  if(nonTerminal("Token_Num")){
    return true;
  }
  if(nonTerminal("Token_True")||nonTerminal("Token_False")){
    return true;
  }
  if (nonTerminal("Token_Comilla_doble") &&
      nonTerminal("Token_Text_string") &&
      nonTerminal("Token_Comilla_doble")){
    return true;
  }
  if (nonTerminal("Token_Comilla") &&
      nonTerminal("Token_ID")&&
      nonTerminal("Token_Comilla")){
    return true;
  }
  if (nonTerminal("Token_(")  &&
      Expression() && 
      nonTerminal("Token_)")){
    return true;
  }
  return false;
}

/*
Factor' ::= [ Expression ] Factor'
Factor' ::= ''
*/
bool Parser::FactorPrime(){
  if (nonTerminal("Token_[")&&
      Expression()&&
      nonTerminal("Token_]")&&
      FactorPrime()){
    return true;
  }
  return true;
}

/*
Factor ::= Primary Factor'
*/
bool Parser::Factor(){
  if(Primary() && FactorPrime()){
    return true;
  }
  return false;
}

/*
Unary ::= ! Unary
Unary ::= - Unary
Unary ::= Factor
*/
bool Parser::Unary(){
  if (nonTerminal("Token_!") || nonTerminal("Token_-") || Factor()) {
    return true;
  }
  return false;
}

/*
Term' ::= * Unary Term'
Term' ::= / Unary Term'
Term' ::= % Unary Term'
Term' ::= ''
*/
bool Parser::TermPrime(){
  if ((nonTerminal("Token_*") || nonTerminal("Token_/") || nonTerminal("Token_%")) &&
      Unary() &&
      TermPrime()){
    return true;
  }
  return true;
}

/*
Term ::= Unary Term'
*/
bool Parser::Term(){
  if(Unary() && TermPrime()){
    return true;
  }
  return false;
}

/*
Expr' ::= + Term Expr'
Expr' ::= - Term Expr'
Expr' ::= ''
*/
bool Parser::ExprPrime(){
  if ((nonTerminal("Token_+") || nonTerminal("Token_-") ) &&
      Term()&&
      ExprPrime()){
    return true;
  }
  return true;
}

/*
Expr ::= Term Expr'
*/
bool Parser::Expr(){
  if(Term() && ExprPrime()){
    return true;
  }
  return false;
}

/*
RelExpr' ::= < Expr RelExpr'
RelExpr' ::= > Expr RelExpr'
RelExpr' ::= <= Expr RelExpr'
RelExpr' ::= >= Expr RelExpr'
RelExpr' ::= ''
*/
bool Parser::RelExprPrime(){
  if ((nonTerminal("Token_<") || nonTerminal("Token_>") || nonTerminal("Token_<=")||nonTerminal("Token_>=")) &&
      Expr()&&
      RelExprPrime()){
    return true;
  }
  return true;
}

/*
RelExpr ::= Expr RelExpr'
*/
bool Parser::RelExpr(){
  if(Expr()&&RelExprPrime()){
    return true;
  }
  return false;
}

/*
EqExpr' ::= == RelExpr EqExpr'
EqExpr' ::= != RelExpr EqExpr'
EqExpr' ::= ''
*/
bool Parser::EqExprprime(){
  if ((nonTerminal("Token_==") || nonTerminal("Token_!=")) &&
      RelExpr()&&
      EqExprprime()){
    return true;
  }
  return true;
}

/*
EqExpr ::= RelExpr EqExpr'
*/
bool Parser::EqExpr(){
  if(RelExpr() && EqExprprime()){
    return true;
  }
  return false;
}

/*
AndExpr' ::= && EqExpr AndExpr'
AndExpr' ::= ''
*/
bool Parser:: AndExprPrime(){
  if (nonTerminal("Token_&&")&&
      EqExpr()&&
      AndExprPrime()){
    return true;
  }
  return true;
}

/*
AndExpr ::= EqExpr AndExpr'
*/
bool Parser::AndExpr(){
  if(EqExpr()&&AndExprPrime()){
    return true;
  }
  return false;
}

/*
OrExpr' ::= || AndExpr OrExpr'
OrExpr' ::= ''
*/
bool Parser::OrExprPrime(){
  if (nonTerminal("Token_||")&&
      AndExpr()&&
      OrExprPrime()){
    return true;
  }
  return true;
}

/*
OrExpr ::= AndExpr OrExpr'
*/
bool Parser::OrExpr(){
  if (AndExpr()&& OrExprPrime()){
    return true;
  }
  return false;
}

/*
AuxExpression ::= = Expression
AuxExpression ::= ''
*/
bool Parser::AuxExpression(){
  if (nonTerminal("Token_=") && Expression()){
    return true;
  }
  return true;
}

/*
Expression ::= OrExpr AuxExpression
*/

bool Parser::Expression(){
  if (OrExpr()&&AuxExpression()){
    return true;
  }
  return false;
}

/*
ExprList' ::= , Expression ExprList'
ExprList' ::= ''
*/
bool Parser::ExprListPrime(){
  if (Expression()&& ExprListPrime()){
    return true;
  }
  return true;
}

/*
ExprList ::= Expression ExprList'
*/
bool Parser::ExprList(){
  if (Expression()&& ExprListPrime()){
    return true;
  }
  return false;
}
