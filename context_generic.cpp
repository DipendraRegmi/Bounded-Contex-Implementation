//
//  context_generic.cpp
//  product prototype
//
//  Created by Dipendra Regmi on 05/02/20.
//  Copyright © 2020 Dipendra Regmi. All rights reserved.
//

#include "context_generic.h"
#include "generic_executor.h"

//executor_generic context_generic::*e = nullptr;

//context_generic* context_generic::moveToContext()
//{
//    context_generic* C1 = new context_generic;
//}

//
//template <typename T>
//void context_generic::createNewContextWith(T task)
//{
//    context_generic* C1 = new context_generic;
//    C1->send(task);
//    e->addContext(C1);
//}

void context_generic::send_signal(int signal) // process
{
    std::lock_guard<std::mutex> lk(slock);
    signals = signal; // bitwise should be applied.
}

int context_generic::getpending_signal() // process
{
    std::lock_guard<std::mutex> lk(slock);
    return signals;
}

bool context_generic::shouldHalt()
{
    if(getpending_signal() == 1)
        return true;
    return false;
}

bool context_generic::contextSwitch()
{
    if(getpending_signal() == 2)
        return true;
    return false;
}
//
//template <typename T>
//void context_generic::send(T task) // process
//{
//    auto t = (std::unique_ptr<context_generic::uniformBase>) new context_generic::uniform<T>(task);
//    
//    std::lock_guard<std::mutex> lk(qlock);
//    mail_box.push(std::move(t));
//}

//template <typename T>
std::unique_ptr<context_generic::uniformBase> context_generic::receive()
{
    std::unique_lock<std::mutex> lk(qlock);
    
    auto result = std::move(mail_box.front());
    mail_box.pop();
    return result;
}

bool context_generic::hasTask()
{
    std::unique_lock<std::mutex> lk(qlock);
    return !mail_box.empty();
}
