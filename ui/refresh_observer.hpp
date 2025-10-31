#pragma once

namespace UI
{
    class RefreshObserver
    {
        public:
            RefreshObserver() { };
            virtual ~RefreshObserver() { };

            virtual void update() = 0;
    };
} // namespace UI