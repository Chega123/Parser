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


/*Function -> Type Identifier (Params) { StmtList }*/

bool Type() {}
bool params() {}
bool StmtList() {}


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
            else return fail();
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

