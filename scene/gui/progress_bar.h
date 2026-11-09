// This file contains the declarations of the ProgressBar class, which is
// responsible for managing a progress bar within the GUI. It provides
// functionality for handling progress display, percentage visibility, and
// drawing the progress bar.
#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include "scene/gui/range.h"

class ProgressBar : public Range {

  GDCLASS(ProgressBar, Range);

  bool percent_visible;

protected:
  void _notification(int p_what);
  static void _bind_methods();

public:
  void set_percent_visible(bool p_visible);
  bool is_percent_visible() const;

  Size2 get_minimum_size() const;
  ProgressBar();
};

#endif // PROGRESS_BAR_H
