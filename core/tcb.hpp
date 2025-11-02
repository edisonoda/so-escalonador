#pragma once

#include <string>
#include <list>
#include <map>

namespace Core
{
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
      const static std::map<std::string, EventType> events_map;

      std::string id;
      int color;
      int start;
      int duration;
      int priority;
      int remaining;
      int completion_time;
      TCBState state;
      std::list<Event> events;

      void createEvent(std::string ev);

    public:
      TCB(std::string id, int color, int start, int duration, int priority, std::list<std::string> events = {});
      ~TCB();

      std::string getId() const;
      int getColor() const;
      int getStart() const;
      int getDuration() const;
      int getPriority() const;

      void setId(const std::string _id);
      void setColor(const int _color);
      void setStart(const int _start);
      void setDuration(const int _duration);
      void setPriority(const int _priority);
      void setCompletionTime(int time);

      int getRemaining() const;
      TCBState getState() const;
      int getCompletionTime() const;

      void setState(TCBState state);
      void decrementRemaining(int amount = 1);
  };
} // namespace Core