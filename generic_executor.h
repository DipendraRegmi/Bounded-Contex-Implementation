//
//  generic_executor.h
//  product prototype
//
//  Created by Dipendra Regmi on 05/02/20.
//  Copyright © 2020 Dipendra Regmi. All rights reserved.
//

#ifndef generic_executor_h
#define generic_executor_h

//#include "context_generic.h"
#include <queue>
#include <thread>
#include <vector>

struct context_generic;

struct executor_generic
{
    executor_generic();
//    {
//        actor = new std::thread(&executor_generic::execute, this);
//        currentContext = NULL;
//        newContext = currentContext;
//    }
    ~executor_generic();
//    {
//        actor->join();
//    }
    void selectContext(context_generic* ctx); // for now we are getting index of context
//    {
//        {
//            std::unique_lock<std::mutex> lk(qlock);
//            // need to push this context to front of the queue so that it gets picked up.
//            newContext = ctx; //a lot do be done, use rotate here.
//        }
//
//        currentContext->send_signal(2); //need to switch the context;
//    }
    
    void addContext(context_generic* C);
//    {
//        std::unique_lock<std::mutex> lk(qlock);
//        activelists.push_back(C);
//        cv.notify_one();
//    }
private:
    
    context_generic* getNextContext();
//    {
//        std::unique_lock<std::mutex> lk(qlock);
//        if(newContext)
//            return newContext; // we don't need a lock.
//        static int i = 0;
//        int ran = (i)%activelists.size();
//        auto next = activelists[ran];
//        i++;
//
//        // if activelist is empty we need to scan pending list and put all the context with
//        // task in pending list to activelist.
//        return next;
//
//    }
    
    void switchContext();
//    {
//        currentContext->send_signal(0); // reset the context switch flag.
//
//        //lists.push_back(currentContext); // use rotate
//        currentContext = getNextContext();
//    }
    void execute();
//    {
//        while(true)
//        {
//            {
//                if(currentContext == NULL) //current will be null if active list is empty
//                {
//                    std::unique_lock<std::mutex> lk(qlock);
//                    if(activelists.empty())
//                    {
//                        // scan  pending list and copy context(with task), coding pending
//                        cv.wait(lk, [this]{return !activelists.empty();});
//                    }
//                    currentContext = activelists[0];
//                }
//            }
//
//            if(currentContext->shouldHalt())
//            {
//
//                // remove current from the list
//            }
//
//            if(currentContext->contextSwitch())
//                switchContext();
//
//            if(!currentContext->hasTask())
//            {
//                //pendinglists.push_back(currentContext);
//                currentContext->send_signal(2);
//                continue; // ugly goto. better change it to goto
//            }
//
//            auto ctx = currentContext->receive();
//            ctx->operator()(); //just invoke
//        }
//    }
    
private:
    context_generic* currentContext;
    context_generic* newContext;
    std::condition_variable cv;
    std::mutex qlock;
    std::thread* actor;
    std::vector<context_generic*> activelists; // make this private
    std::vector<context_generic*> pendinglists; // make this private
};


#endif /* generic_executor_h */
