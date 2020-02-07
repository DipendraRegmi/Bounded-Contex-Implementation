//
//  context_generic.h
//  product prototype
//
//  Created by Dipendra Regmi on 05/02/20.
//  Copyright © 2020 Dipendra Regmi. All rights reserved.
//

#ifndef context_generic_h
#define context_generic_h


#include <queue>
#include <thread>
#include <vector>
#include "generic_executor.h"

struct context_generic
{
private:
    
    struct uniformBase
    {
        virtual void operator()(context_generic*)=0;
        uniformBase()
        {
        }
        virtual ~uniformBase() {}
    };
    
    template <typename T>
    struct uniform : uniformBase
    {
        uniform(T task):taskP(task){}
        
        void operator()(context_generic* ctx)
        {
            taskP(ctx);
        }
        T taskP;
    };
    
public:
    
    template <typename T>
    context_generic* createNewContextWith(T task)
    {
        context_generic* C1 = new context_generic;
        {
            std::lock_guard<std::mutex> lk(slock);
            if(e == nullptr)
                e = new executor_generic;
        }
        C1->e = e;
        C1->send(task);
        C1->e->addContext(C1);
        return C1;
    }
    
//    context_generic()
//    {
//        e = new executor_generic;
//    }
    
    void send_signal(int signal); // process
//    {
//        std::lock_guard<std::mutex> lk(slock);
//        signals = signal; // bitwise should be applied.
//    }
    
    int getpending_signal(); // process
//    {
//        std::lock_guard<std::mutex> lk(slock);
//        return signals;
//    }
    
    bool shouldHalt();
//    {
//        if(getpending_signal() == 1)
//            return true;
//        return false;
//    }
    
    bool contextSwitch();
//    {
//        if(getpending_signal() == 2)
//            return true;
//        return false;
//    }
    
    template <typename T>
    void send(T task) // process
    {
        auto t = (std::unique_ptr<uniformBase>) new uniform<T>(task);

        std::lock_guard<std::mutex> lk(qlock);
        mail_box.push(std::move(t));
    }
    
    //template <typename T>
    std::unique_ptr<uniformBase> receive();
//    {
//        std::unique_lock<std::mutex> lk(qlock);
//
//        auto result = std::move(mail_box.front());
//        mail_box.pop();
//        return result;
//    }
    
    bool hasTask();
//    {
//        std::unique_lock<std::mutex> lk(qlock);
//        return !mail_box.empty();
//    }
    
private:

    std::queue<std::unique_ptr<uniformBase>> mail_box;
    std::mutex qlock; // queue lock
    
    std::mutex slock; //signal lock
    int signals;
    
    executor_generic *e = nullptr;
};

#endif /* context_generic_h */
