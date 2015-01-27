/*
 * grammar:
 *   P ---> E '\0'
 *   E ---> T {('+'|'-') T}
 *   T ---> S {('*'|'/') S}
 *   S ---> digit | '(' E ')'
 */
class Parser{
   int level;
   char next;
   istream& is;
   function<void(char)> cb;

   char pull(){
        return char_traits<char>::to_char_type(is.get());
   }

   void scan(){
       do{
           next=pull();
       }
       while(isspace(next));
   }

public:
   Parser(istream& is_,function<void(char)> cb_) :
      level(0), next(), is(is_), cb(cb_)
    {}

   void run() {
      scan();
      E();
      if (next!='\0'){
          exit(1);
      }
   }

private:
   void E(){
      T();
      while (next=='+'||next=='-'){
         cb(next); 
         scan();
         T();
      }
   }

   void T(){
      S();
      while (next=='*'||next=='/'){
         cb(next); 
         scan();
         S();
      }
   }

   void S(){
      if (isdigit(next)){
         cb(next);
         scan();
      }
      else if(next=='('){
         cb(next); 
         scan();
         E();
         if (next==')'){
             cb(next); 
             scan();
         }else{
             exit(2);
         }
      }
      else{
         exit(3);
      }
   }
};

int main() {
    std::istringstream is("1+1");
    bool done=false;
    char c;
    std::execution_context main_ctx(
        std::execution_context::current() );
    // invert control flow
    std::execution_context parser_ctx(
        std::fixedsize(),
        [&is,&main_ctx,&done,&c](){
            Parser p(is,
                     [&main_ctx,&c](char ch){
                        c=ch;
                        // resume main-context
                        main_ctx.jump_to();
                     });
            p.run();
            done=true;
        });

    // user-code pulls parsed data from parser
    while(!done){
        // resume parser-context
        parser_ctx.jump_to();
        printf("Parsed: %c\n",c);
    }
}