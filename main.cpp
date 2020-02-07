//
//  main.cpp
//  product prototype
//
//  Created by Dipendra Regmi on 20/01/20.
//  Copyright © 2020 Dipendra Regmi. All rights reserved.
//

#include <iostream>
#include "Context.h"
#include "context_generic.h"
#include "generic_executor.h"

struct printName
{
    printName(std::string val, struct context<struct printName>* c):name(val), ctx(c){}
    void operator()()
    {
        static int i = 0;
        std::cout << name << " " << i <<  std::endl;
        if(i < 10)
            ctx->send(*this);
        i++;
    }
    
    std::string name;
    
    struct context<struct printName>* ctx;
};



struct printLastName
{
    printLastName(std::string val, struct context<struct printLastName>* c):lname(val), ctx(c){}
   
    void operator()()
    {
        static int i = 0;
        std::cout << lname << " " << i <<  std::endl;
        if(i < 10)
        ctx->send(*this);
        i++;
    }
    
    std::string lname;
    
    struct context<struct printLastName>* ctx;
};


context_generic* secondtask = nullptr;

struct printFirstNameTask
{
    printFirstNameTask(std::string val):lname(val){}
    
    void operator()(struct context_generic* ctx)
    {
        static int i = 0;
        std::cout << lname << " " << i <<  std::endl;
        if(i < 10)
        {
            printFirstNameTask tmp(lname);
            
            ctx->send(std::move(tmp));
        }
        i++;
    }
    
    std::string lname;
};

struct printLastNameTask
{
    printLastNameTask(std::string val):lname(val){}
    
    void operator()(struct context_generic* ctx)
    {
        static int i = 0;
        std::cout << lname << " " << i <<  std::endl;
        if(i < 10)
        {
            printLastNameTask tmp(lname);
            ctx->send(std::move(tmp));
            if(i < 1)
            {
                
                printFirstNameTask para("dipendra has been promoted to CS2 this yr. Is he happy?");
                secondtask = ctx->createNewContextWith(para);
            }
        }
        i++;
    }
    
    std::string lname;
};


int main(int argc, const char * argv[]) {
    
//    using type = context<printName>*;
//    context<printName>* C1 = new context<printName>;
//    printName firstName("dipendra", C1);
//
//    C1->send(firstName);
//
//    context<printName>* C2 = new context<printName>;
//    printName lastName("regmi", C2);
//
//    C2->send(lastName);
//
//    executor<printName> e;
//    e.addContext(C1);
//    e.addContext(C2);
//    while(1)
//    {
//        C1->send_signal(2);
//        C2->send_signal(2);
//    }
    
    

    context_generic* Global = new context_generic;
    
    printLastNameTask firstName("dipendra");
    auto newContext = Global->createNewContextWith(firstName);
    
    while(1)
    {
        if(secondtask) secondtask->send_signal(2);
        newContext->send_signal(2);
    }
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
