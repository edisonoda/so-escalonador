#include "refresh_subject.hpp"

using namespace UI;

RefreshSubject::RefreshSubject()
{
}

RefreshSubject::~RefreshSubject()
{
    observers.clear();
}

void RefreshSubject::attach(RefreshObserver* obs)
{
    observers.insert(obs);
}

void RefreshSubject::detach(RefreshObserver* obs)
{
    observers.erase(obs);
}

void RefreshSubject::update()
{
    for (RefreshObserver* obs : observers)
        obs->update();
}