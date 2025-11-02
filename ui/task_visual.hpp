#pragma once

#include <vector>

#include "screen.hpp"
#include "../core/tcb.hpp"

#define INFO_SPACE 6
#define STATS_SPACE 3

#define UNIT_WIDTH 3

namespace UI {
  struct GanttEntry {
    int tick;
    int task_index;
    string color;
  };

  class TaskVisual : public Window {
    protected:
      vector<Core::TCB*> *ord_tasks;
      int visual_edge_x;
      int visual_edge_y;
      int x_offset;
      int y_offset;

      void checkEdges(int x, int y);
      
      public:
      TaskVisual();
      virtual ~TaskVisual();

      virtual void setTasks(vector<Core::TCB*> *tasks, int y_offset = 0);
      
      virtual void print(int x, int y, string str);
      virtual void print(int x, int y, int ch);
      virtual void drawTick(int tick) = 0;
  };

  class TaskInfo : public TaskVisual {
    private:
      map<string, string> MONITOR_LABELS;
      map<string, string> MONITOR_LABELS_STATUS;

    public:
      TaskInfo();
      ~TaskInfo();
      
      void drawStaticInfo(int index, int offset = 0);
      void calcFinalStatistics(double *avg_turnaround, double *avg_wait);
      void displayFinalStatistics();

      virtual void setTasks(vector<Core::TCB*> *tasks, int y_offset = 0);
      virtual void drawTick(int tick);
  };

  class GanttExporter {
    private:
      const int TICK_WIDTH = 30;  // Largura de cada tick
      const int TASK_HEIGHT = 30; // Altura de cada tarefa
      vector<GanttEntry> chart_history;
      vector<Core::TCB*> *tasks;
      
    public:
      GanttExporter(vector<Core::TCB*> *task_list);
      ~GanttExporter();
      void registerEntry(int tick, int task_index, int color);
      void generate(const string& filename, int total_time, int task_count);
      string convertColor(int color);
  };
  
  class GanttChart : public TaskVisual {
    private:
      GanttExporter *gantt_exporter;

    public:
      GanttChart(GanttExporter *chart_gen);
      ~GanttChart();

      virtual void setTasks(vector<Core::TCB*> *tasks, int y_offset = 0);
      virtual void drawTick(int tick);
  };
} // namespace UI