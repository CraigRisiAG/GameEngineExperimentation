
#ifndef NAVIGATION_OBSTACLE_2D_H
#define NAVIGATION_OBSTACLE_2D_H

#include "scene/main/node.h"

class Navigation2D;

class NavigationObstacle2D : public Node {
  GDCLASS(NavigationObstacle2D, Node);

  Navigation2D *navigation;

  RID agent;

protected:
  static void _bind_methods();
  void _notification(int p_what);

public:
  NavigationObstacle2D();
  virtual ~NavigationObstacle2D();

  void set_navigation(Navigation2D *p_nav);
  const Navigation2D *get_navigation() const { return navigation; }

  void set_navigation_node(Node *p_nav);
  Node *get_navigation_node() const;

  RID get_rid() const { return agent; }

  virtual String get_configuration_warning() const;

private:
  void update_agent_shape();
};

#endif
