#pragma once

#include <string>
#include <list>
#include <map>

using namespace std;

namespace Core {
  class IOEvent;
  class Mutex;

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
    int id;
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
      int priority_d;
      int remaining;
      int completion_time;
      TCBState state;
      IOEvent* current_event;
      list<Mutex*> mutex_list;
      list<Event*> events;
      list<string> string_events;

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
      int getPriorityD() const;
      int getRemaining() const;
      int getCompletionTime() const;
      TCBState getState() const;
      IOEvent* getCurrentEvent() const;
      list<Mutex*>* getMutexList();
      list<Event*>* getEvents();
      list<string>* getStringEvents();
      bool isAvailable();

      void setId(const string _id);
      void setColorHex(const string _color);
      void setColor(const int _color);
      void setStart(const int _start);
      void setDuration(const int _duration);
      void setPriority(const int _priority);
      void setPriorityD(const int _priority_d);
      void setCompletionTime(int time);
      void setCurrentEvent(IOEvent* event);
      void addMutex(Mutex* mutex);
      void removeMutex(Mutex* mutex);

      void setState(TCBState state);
      void decrementRemaining(int amount = 1);
  };
} // namespace Core