#pragma once

namespace Core
{
    class TickObserver
    {
    protected:
        TickObserver() { }

    public:
        virtual ~TickObserver() { }
        virtual void tick() = 0;
    };
} // namespace Core