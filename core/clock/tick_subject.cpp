#include "tick_subject.hpp"

using namespace Core;

TickSubject::TickSubject() { }

TickSubject::~TickSubject()
{
    observers.clear();
}

void TickSubject::attach(TickObserver* obs)
{
    observers.insert(obs);
}

void TickSubject::detach(TickObserver* obs)
{
    observers.erase(obs);
}

void TickSubject::notify()
{
    for (TickObserver* obs : observers)
        obs->tick();
}
