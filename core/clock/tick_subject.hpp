#pragma once

#include <set>
#include "tick_observer.hpp"

using namespace std;

namespace Core
{
    class TickSubject
    {
    private:
        set<TickObserver*> observers;
    
    protected:
        TickSubject();

    public:
        virtual ~TickSubject();

        void attach(TickObserver* obs);
        void detach(TickObserver* obs);
        void notify();
    };
} // namespace Core