#include "tcb.hpp"

using namespace Core;

// Faz o mapeamento dos tipos de evento
const map<string, EventType> TCB::events_map({
  {"IO", EventType::IO},
  {"ML", EventType::ML},
  {"MU", EventType::MU}
});

TCB::TCB(string id, string color_hex, int color, int start, int duration, int priority, list<string> events) :
  id(id),
  color_hex(color_hex),
  color(color),
  start(start),
  duration(duration),
  priority(priority)
{
  this->remaining = duration;
  this->state = TCBState::NEW;

  for (string event : events)
    createEvent(event);
}

TCB::~TCB() {}

void TCB::createEvent(string ev) {
  size_t sep = ev.find(':');

  if (sep == string::npos)
    return;

  auto it = events_map.find(ev.substr(0, sep));

  if (it == events_map.end())
    return;

  EventType type = it->second;
  string duration_info = ev.substr(sep + 1, ev.length() - 1);
  Event event = {type};

  switch (type) {
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

string TCB::getColorHex() const { return color_hex; }

int TCB::getColor() const { return color; }

int TCB::getStart() const { return start; }

int TCB::getDuration() const { return duration; }

int TCB::getPriority() const { return priority; }

void TCB::setId(const string _id) { id = _id; }

void TCB::setColorHex(const string _color) { color_hex = _color; }

void TCB::setColor(const int _color) { color = _color; }

void TCB::setStart(const int _start) { start = _start; }

void TCB::setDuration(const int _duration) { duration = _duration; }

void TCB::setPriority(const int _priority) { priority = _priority; }

void TCB::setCompletionTime(int time) { this->completion_time = time; }

int TCB::getCompletionTime() const { return this->completion_time; }

int TCB::getRemaining() const { return remaining; }

TCBState TCB::getState() const { return state; }

void TCB::setState(TCBState state) { this->state = state; }

void TCB::decrementRemaining(int amount) { this->remaining -= amount; }