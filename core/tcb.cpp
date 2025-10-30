#include "tcb.hpp"

using namespace Core;

TCB::TCB(string id, int color, int start, int duration, int priority, list<string> events)
    : id(id), color(color), start(start), duration(duration), priority(priority)
{
    this->remaining = duration;
    this->state = TCBState::NEW;
    this->events = events;
}

TCB::~TCB() {}

string TCB::getId() const { return id; }

int TCB::getColor() const { return color; }

int TCB::getStart() const { return start; }

int TCB::getDuration() const { return duration; }

int TCB::getPriority() const { return priority; }

void TCB::setId(const string _id) { id = _id; }

void TCB::setColor(const int _color) { color = _color; }

void TCB::setStart(const int _start) { start = _start; }

void TCB::setDuration(const int _duration) { duration = _duration; }

void TCB::setPriority(const int _priority) { priority = _priority; }

int TCB::getRemaining() const { return remaining; }

TCBState TCB::getState() const { return state; }

void TCB::setState(TCBState state) { this->state = state; }

void TCB::decrementRemaining(int amount) { this->remaining -= amount; }