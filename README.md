# Bounded-Contex-Implementation
ProtoType for Bounded Context Concept in C++.
This can be used to implement multi tab feature in any application. All the actions within a tab has no implication to other tabs and a single tab is active at a time.
Implementation has following concepts:
Tasks :: Unit of work to be done.
Context :: Which is a collection of such dependent task, all the task from a doc are part of same context.
Executor:: Which is a collection of contexts and a thread, one doc is active at a time so single threaded application.
           Executor picks up a context and starts executing the tasks in the context.
           
Context can be signaled, we can context switch the context, this behavior can be used to simulate the switch between docs.
Context can create another context, like opening a new doc, from current doc.
