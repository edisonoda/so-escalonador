#include "tcb.hpp"
using namespace Core;

TCB::TCB(int id, string color, int start, int duration, int priority)
{
    this->id = id;
    this->color = color;
    this->start = start;
    this->duration = duration;
    this->priority = priority;
    this->remaining = duration;
    this->state = "READY";
}

TCB::~TCB() {}

int TCB::getId() { return id; }

string TCB::getColor() { return color; }

int TCB::getStart() { return start; }

int TCB::getDuration() { return duration; }

int TCB::getPriority() { return priority; }

int TCB::getRemaining() { return remaining; }

string TCB::getState() { return state; }

void TCB::setState(string state) { this->state = state; }

void TCB::decrementRemaining(int amount) { this->remaining -= amount; } 