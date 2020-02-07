//
//  Context.h
//  product prototype
//
//  Created by Dipendra Regmi on 20/01/20.
//  Copyright © 2020 Dipendra Regmi. All rights reserved.
//

#ifndef Context_h
#define Context_h


#include <queue>
#include <thread>
#include <vector>

template <typename T>
struct context
{
public:
    
    void send_signal(int signal) // process
    {
        std::lock_guard<std::mutex> lk(slock);
        signals = signal; // bitwise should be applied.
    }
    
    int getpending_signal() // process
    {
        std::lock_guard<std::mutex> lk(slock);
        return signals;
    }
    
    bool shouldHalt()
    {
        if(getpending_signal() == 1)
            return true;
        return false;
    }
    
    bool contextSwitch()
    {
        if(getpending_signal() == 2)
            return true;
        return false;
    }
 
    void send(T task) // process
    {
        std::lock_guard<std::mutex> lk(qlock);
        mail_box.push(task);
    }
    
    T receive()
    {
        std::unique_lock<std::mutex> lk(qlock);
        
        auto result = mail_box.front();
        mail_box.pop();
        return result;
    }
    
    bool hasTask()
    {
        std::unique_lock<std::mutex> lk(qlock);
        return !mail_box.empty();
    }
    
private:
    std::queue<T> mail_box;
    std::mutex qlock; // queue lock
    
    std::mutex slock; //signal lock
    int signals;
};



template <typename T>
struct executor
{
    executor()
    {
        actor = new std::thread(&executor::execute, this);
        currentContext = NULL;
        newContext = currentContext;
    }
    ~executor()
    {
        actor->join();
    }
    void selectContext(context<T>* ctx) // for now we are getting index of context
    {
        {
            std::unique_lock<std::mutex> lk(qlock);
            // need to push this context to front of the queue so that it gets picked up.
            newContext = ctx; //a lot do be done, use rotate here.
            
        }
        
        currentContext->send_signal(2); //need to switch the context;
    }
    
    void addContext(context<T>* C)
    {
        std::unique_lock<std::mutex> lk(qlock);
        activelists.push_back(C);
        cv.notify_one();
    }
private:
    
    context<T>* getNextContext()
    {
        std::unique_lock<std::mutex> lk(qlock);
        if(newContext)
            return newContext; // we don't need a lock.
        static int i = 0;
        int ran = (i)%activelists.size();
        auto next = activelists[ran];
        i++;

        // if activelist is empty we need to scan pending list and put all the context with
        // task in pending list to activelist.
        return next;
        
    }
    
    
    void switchContext()
    {
        currentContext->send_signal(0); // reset the context switch flag.
       
        //lists.push_back(currentContext); // use rotate
        currentContext = getNextContext();
    }
    void execute()
    {
        while(true)
        {
            {
                if(currentContext == NULL) //current will be null if active list is empty
                {
                    std::unique_lock<std::mutex> lk(qlock);
                    if(activelists.empty())
                    {
                        // scan  pending list and copy context(with task), coding pending
                        cv.wait(lk, [this]{return !activelists.empty();});
                    }
                     currentContext = activelists[0];
                }
            }
            
            if(currentContext->shouldHalt())
            {
                
                // remove current from the list
            }
            
            if(currentContext->contextSwitch())
                switchContext();
            
            if(!currentContext->hasTask())
            {
                //pendinglists.push_back(currentContext);
                currentContext->send_signal(2);
                continue; // ugly goto. better change it to goto
            }
            
            auto ctx = currentContext->receive();
            ctx(); //just invoke
        }
    }
   
private:
    context<T>* currentContext;
    context<T>* newContext;
    std::condition_variable cv;
    std::mutex qlock;
    std::thread* actor;
    std::vector<context<T>*> activelists; // make this private
    std::vector<context<T>*> pendinglists; // make this private
};






#endif /* Context_h */
