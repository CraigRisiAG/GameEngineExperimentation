
#ifndef NAVIGATION_OBSTACLE_H
#define NAVIGATION_OBSTACLE_H

#include "scene/main/node.h"

class Navigation;

class NavigationObstacle : public Node {
  GDCLASS(NavigationObstacle, Node);

  Navigation *navigation;

  RID agent;

protected:
  static void _bind_methods();
  void _notification(int p_what);

public:
  NavigationObstacle();
  virtual ~NavigationObstacle();

  void set_navigation(Navigation *p_nav);
  const Navigation *get_navigation() const { return navigation; }

  void set_navigation_node(Node *p_nav);
  Node *get_navigation_node() const;

  RID get_rid() const { return agent; }

  virtual String get_configuration_warning() const;

private:
  void update_agent_shape();
};

#endif
