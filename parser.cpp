#include <iostream>
#include <string>
using namespace std;

struct token {
    string name;
    token(string nam) { name = nam; }
};

token NextWord() {
    return tokens[current_token++];
}
token tokens[];
int current_token = 0;  
token word;


/*Type' -> [ ] Type'
Type' -> ''
*/
bool TypePrime() {
    if (word.name == "Token_[") {
        word = NextWord();
        if (word.name == "Token_]") {
            word = NextWord();
            if (TypePrime()) {
                return true;
            }
        }
    }
    return true;
}

/*BasicType -> IntType
BasicType -> BoolType
BasicType -> CharType
BasicType -> StringType
BasicType -> VoidType
*/
bool BasicType() {

    if (word.name == "IntType"|| word.name == "IntType"|| word.name == "BoolType"|| word.name == "CharType"|| word.name == "StringType"|| word.name == "VoidType") {
        return true;
    }
    return false;
}

/*Type->BasicType Type'*/

bool Type() {
    if (BasicType()) {
        if (TypePrime()) {
            return true;
        }
    }
}
bool params() {}
bool StmtList() {}

/*Function -> Type Identifier (Params) { StmtList }*/

bool Function() {
    if (Type()) {
        if (word.name == "Token_Identifier") {
            word = NextWord();
            if (word.name == "Token_(") {
                word = NextWord();
                if (params()) {
                    if (word.name == "Token_)") {
                        word = NextWord();
                        if (word.name == "Token_{") {
                            word = NextWord();
                            if (StmtList()) {
                                if (word.name == "Token_}") {
                                    word = NextWord();
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool Expresion() {}


/*VarDecl' -> ;
VarDecl' -> = Expression ;
*/
bool VarDeclPrime() {
    if (word.name == "Token_;") {
        word = NextWord();
    }
    else if (word.name == "Token_=") {
        word = NextWord();
        if (Expresion()) {
            if (word.name == "Token_;") {
                word = NextWord();
                return true;
            }
        }
    }
}


/*VarDecl -> Type Identifier VarDecl'*/
bool VarDecl(){
    if (Type()) {
        if (word.name == "Token_Identifier") {
            word = NextWord();
            if (VarDeclPrime()) {
                return true;
            }
        }
    }
    return false;
}



/*Declaration -> [ Function ]
Declaration -> VarDecl
*/
bool Declaration() {
    if (word.name == "Token_[") {
        word = NextWord();
        if (Function()) {
            if (word.name == "Token_]") { word = NextWord(); return true; }
            else return false;
        }
    }
    else if (VarDecl()) {
        return true;
    }

    return false;

}
/*Program' -> Declaration Program'
Program' -> ''
*/
bool ProgramPrime() {
    if (Declaration()) { 
        if (ProgramPrime()) {
            return true;
        }   
    }
    return true;
}

/*Program -> Declaration Program'
*/
bool Program() {
    if(Declaration()) return ProgramPrime();
    return false;
}

int main()
{
     word = NextWord();
     Program();

}

