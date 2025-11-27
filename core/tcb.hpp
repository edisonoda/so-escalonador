#pragma once

#include <string>
#include <list>
#include <map>

using namespace std;

namespace Core {
  class IOEvent;

  enum class TCBState {
    NEW,
    READY,
    RUNNING,
    SUSPENDED,
    TERMINATED
  };

  enum class EventType {
    IO,
    ML,
    MU
  };

  struct Event {
    EventType type;
    int start;
    int duration;
  };

  class TCB {
    private:
      const static map<string, EventType> events_map;

      string id;
      string color_hex;
      int color;
      int start;
      int duration;
      int priority;
      int remaining;
      int completion_time;
      TCBState state;
      IOEvent* current_event;
      list<Event*> events;

      void createEvent(string ev);

    public:
      TCB(string id, string color_hex, int color, int start, int duration, int priority, list<string> events = {});
      ~TCB();

      string getId() const;
      string getColorHex() const;
      int getColor() const;
      int getStart() const;
      int getDuration() const;
      int getPriority() const;
      int getRemaining() const;
      int getCompletionTime() const;
      TCBState getState() const;
      IOEvent* getCurrentEvent() const;
      list<Event*>* getEvents();

      void setId(const string _id);
      void setColorHex(const string _color);
      void setColor(const int _color);
      void setStart(const int _start);
      void setDuration(const int _duration);
      void setPriority(const int _priority);
      void setCompletionTime(int time);
      void setCurrentEvent(IOEvent* event);

      void setState(TCBState state);
      void decrementRemaining(int amount = 1);
  };
} // namespace Core