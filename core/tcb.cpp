#include "tcb.hpp"
using namespace Core;

TCB::TCB(string id, string color, int start, int duration, int priority, list<string> events)
    : id(id)
    , color(color)
    , start(start)
    , duration(duration)
    , priority(priority)
{
    this->remaining = duration;
    this->state = TCBState::LOADED;
    this->events = events;
}

TCB::~TCB() {}

string TCB::getId() const { return id; }

string TCB::getColor() const { return color; }

int TCB::getStart() const { return start; }

int TCB::getDuration() const { return duration; }

int TCB::getPriority() const { return priority; }

int TCB::getRemaining() const { return remaining; }

TCBState TCB::getState() const { return state; }

void TCB::setState(TCBState state) { this->state = state; }

void TCB::decrementRemaining(int amount) { this->remaining -= amount; } 