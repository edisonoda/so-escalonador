#include "tcb.hpp"
#include "system.hpp"

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
  priority(priority),
  priority_d(priority)
{
  this->remaining = duration;
  this->state = TCBState::NEW;

  current_event = nullptr;
  mutex = nullptr;

  for (string event : events)
    createEvent(event);
}

TCB::~TCB() {
  current_event = nullptr;
  mutex = nullptr;

  for (Event* ev : events)
    delete ev;

  events.clear();
}

void TCB::createEvent(string ev) {
  size_t sep = ev.find(':');
  int type_index = 2;

  if (sep == string::npos)
    return;

  auto it = events_map.find(ev.substr(0, type_index));

  if (it == events_map.end())
    return;

  EventType type = it->second;
  string info = ev.substr(type_index + 1, ev.length() - 1);
  Event* event = new Event{type, 0, 0, 0};

  switch (type) {
    case EventType::IO:
      sep = info.find("-");
      event->start = stoi(info.substr(0, sep));
      event->duration = stoi(info.substr(sep + 1, info.length() - 1));
      break;
    case EventType::MU:
      sep = info.find(":");
      event->id = stoi(info.substr(0, sep));
      event->start = stoi(info.substr(sep + 1, info.length() - 1));
      break;
    case EventType::ML:
      sep = info.find(":");
      event->id = stoi(info.substr(0, sep));
      event->start = stoi(info.substr(sep + 1, info.length() - 1));
      break;
    default:
      event->start = stoi(info);
      break;
  }

  events.push_back(event);
}

string TCB::getId() const { return id; }

string TCB::getColorHex() const { return color_hex; }

int TCB::getColor() const { return color; }

int TCB::getStart() const { return start; }

int TCB::getDuration() const { return duration; }

int TCB::getPriority() const { return priority; }

int TCB::getPriorityD() const { return priority_d; }

IOEvent* TCB::getCurrentEvent() const { return current_event; }

Mutex* TCB::getMutex() const { return mutex; }

void TCB::setId(const string _id) { id = _id; }

void TCB::setColorHex(const string _color) { color_hex = _color; }

void TCB::setColor(const int _color) { color = _color; }

void TCB::setStart(const int _start) { start = _start; }

void TCB::setDuration(const int _duration) { duration = _duration; }

void TCB::setPriority(const int _priority) { priority = _priority; setPriorityD(_priority); }

void TCB::setPriorityD(const int _priority_d) { priority_d = _priority_d; }

void TCB::setCompletionTime(int time) { this->completion_time = time; }

void TCB::setCurrentEvent(IOEvent* event) { current_event = event; }

void TCB::setMutex(Mutex* m) { mutex = m; }

int TCB::getCompletionTime() const { return this->completion_time; }

int TCB::getRemaining() const { return remaining; }

TCBState TCB::getState() const { return state; }

list<Event*>* TCB::getEvents() { return &events; }

void TCB::setState(TCBState state) { this->state = state; }

void TCB::decrementRemaining(int amount) { this->remaining -= amount; }