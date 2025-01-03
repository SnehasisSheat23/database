#include <iostream>
#include <string>
#include<vector>

using namespace std;


typedef enum
{
    TOKEN_PUSH ,
    TOKEN_INTO ,
    TOKEN_VALUE,
    TOKEN_DESTROY ,
    TOKEN_FROM,
    TOKEN_FETCH,
    TOKEN_STRING ,
    TOKEN_INTEGER ,
    TOKEN_LEFT_PAR,
    TOKEN_RIGHT_PAR,
    TOKEN_IN,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_CONSTRUCT,
    TOKEN_TABLE


 }TOKEN_SET;

 struct TOKEN{
      TOKEN_SET TOKEN_TYPE;
      string value;
 };

 string token_to_string(TOKEN_SET token)
 {
      switch (token)
      {
        case TOKEN_PUSH     :  return "TOKEN PUSH";
        case TOKEN_INTO     :  return "TOKEN INTO";
        case TOKEN_VALUE    :  return "TOKEN VALUE";
        case TOKEN_DESTROY :  return "TOKEN DESTROY";
        case TOKEN_FROM     :  return "TOKEN FROM";
        case TOKEN_FETCH    :  return "TOKEN FETCH";
        case TOKEN_STRING   :  return "TOKEN STRING";
        case TOKEN_INTEGER :  return "TOKEN INTEGER";
        case TOKEN_LEFT_PAR : return "TOKEN LEFT_PAR";
        case TOKEN_RIGHT_PAR: return "TOKEN RIGHT_PAR";
        case TOKEN_IN        :  return "TOKEN IN";
        case TOKEN_COMMA     :  return "TOKEN COMMA";
        case TOKEN_SEMICOLON  :  return "TOKEN SEMICOLON";
        case TOKEN_CONSTRUCT   :  return "TOKEN CONSTRUCT";
        case TOKEN_TABLE :  return "TOKEN TABLE";
    }
    return &"[!] ERROR : Token not accepted " [token];
}


class tokeniser
{
 private:
   int cursor;
   int length;
   char current;
   string local;
   vector<TOKEN*> TOKEN_LIST;

   char advance(){
     if(cursor >= length-1){
       current = '\0';
       return current;
     }else{
       current = local[cursor++];
       return current;
     }
   }

   void display()
   {
     int counter = 0;
     for(TOKEN * CUR_TOKEN : TOKEN_LIST)
     {
       cout << ++counter<<")"<<CUR_TOKEN->value<<"";
       cout<< token_to_string(CUR_TOKEN->TOKEN_TYPE)<<endl;
     }


   }
   void skip(){
     while(current == ' ' && current != '\0')
       advance();
   }

   TOKEN * tokenizeAlpha(){
     TOKEN* newtoken = new TOKEN;
     string temp = "";
     while(isalpha(current)){
       temp.push_back(current);
       advance();
     }
     newtoken->TOKEN_TYPE = TOKEN_STRING;
     newtoken -> value = temp;
     return newtoken;
   }

   TOKEN * tokenizeDigit(){
     TOKEN * newtoken = new TOKEN;
     string temp = "";
     while(isdigit(current)){
       temp.push_back(current);
       advance();

     }
     newtoken->TOKEN_TYPE = TOKEN_INTEGER;
     newtoken -> value = temp;
     return newtoken;

   }

   TOKEN * tokenizeSpecial(TOKEN_SET NEW_TOKEN_TYPE){
     TOKEN * newtoken = new TOKEN;
     string temp = "";
     newtoken->TOKEN_TYPE = TOKEN_STRING;
     newtoken -> value = temp;
     advance();
     return newtoken;

   }







 public:
   tokeniser()
   {}

   void initialize(string input){
     cursor = 0;
     local = input;
     length = input.length();
     current = local[cursor++];
   }
   void tokenize() {
     while(current)
     {
       skip();
       if(isalpha(current)){
         TOKEN_LIST.push_back(tokenizeAlpha());
       }
       else if(isdigit(current)){
         TOKEN_LIST.push_back(tokenizeDigit());
       }
       switch(current)
       {
         case '(' :
         {
           TOKEN_LIST.push_back(tokenizeSpecial(TOKEN_LEFT_PAR));
         }
         case ')' :
         {
           TOKEN_LIST.push_back(tokenizeSpecial(TOKEN_RIGHT_PAR));
         }
         case ',' :
         {
           TOKEN_LIST.push_back(tokenizeSpecial(TOKEN_COMMA));
         }
         case ';' :
         {
           TOKEN_LIST.push_back(tokenizeSpecial(TOKEN_SEMICOLON));
         }
         default :
         {
           cout << "[!] ERROR : Token not accepted " << current << endl;
         }
       }
     }
     display();
   }
 };

class Inputeval {
    private:
        tokeniser * MainTokeniser ;
    public:
        Inputeval() {
            MainTokeniser = new tokeniser;
        }
        void handle (string input ) {
            MainTokeniser->initialize(input);
            MainTokeniser -> tokenize();


        }

};


int main() {
  string input ;
  Inputeval *main_io = new Inputeval();

  while (true) {
    cout << "our db :  " << endl;
    getline(cin,input);
    main_io->handle(input);
  }
}