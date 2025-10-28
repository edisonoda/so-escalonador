#include "tcb.hpp"
using namespace Core;

const map<string, EventType> TCB::events_map({
    {"IO", EventType::IO},
    {"ML", EventType::ML},
    {"MU", EventType::MU}
});

TCB::TCB(string id, int color, int start, int duration, int priority, list<string> events)
    : id(id)
    , color(color)
    , start(start)
    , duration(duration)
    , priority(priority)
{
    this->remaining = duration;
    this->state = TCBState::NEW;

    for (string event : events)
        createEvent(event);
}

TCB::~TCB() {}

void TCB::createEvent(string ev)
{
    size_t sep = ev.find(':');

    if (sep == string::npos)
        return;

    auto it = events_map.find(ev.substr(0, sep));

    if (it == events_map.end())
        return;

    EventType type = it->second;
    string duration_info = ev.substr(sep + 1, ev.length() - 1);
    Event event = { type };

    switch (type)
    {
        case EventType::IO:
            sep = duration_info.find("-");
            event.start = stoi(duration_info.substr(0, sep));
            event.duration = stoi(duration_info.substr(sep + 1, duration_info.length() - 1));
            break;
        default:
            event.start = stoi(duration_info);
            break;
    }

    this->events.push_back(event);
}

string TCB::getId() const { return id; }

int TCB::getColor() const { return color; }

int TCB::getStart() const { return start; }

int TCB::getDuration() const { return duration; }

int TCB::getPriority() const { return priority; }

int TCB::getRemaining() const { return remaining; }

TCBState TCB::getState() const { return state; }

void TCB::setState(TCBState state) { this->state = state; }

void TCB::decrementRemaining(int amount) { this->remaining -= amount; } 