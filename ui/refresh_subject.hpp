#pragma once

#include "../utils/std_libraries.hpp"
#include "refresh_observer.hpp"

namespace UI
{
    class RefreshSubject
    {
        private:
            set<RefreshObserver*> observers;

        public:
            RefreshSubject();
            ~RefreshSubject();

            void attach(RefreshObserver* obs);
            void detach(RefreshObserver* obs);
            void update();
    };
} // namespace UI