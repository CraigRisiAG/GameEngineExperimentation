
#include "rasterizer.h"

#include "core/os/os.h"
#include "core/print_string.h"

Rasterizer *(*Rasterizer::_create_func)() = NULL;

void RasterizerScene::InstanceDependency::instance_notify_changed(
    bool p_aabb, bool p_dependencies) {
  for (Map<InstanceBase *, uint32_t>::Element *E = instances.front(); E;
       E = E->next()) {
    E->key()->dependency_changed(p_aabb, p_dependencies);
  }
}
void RasterizerScene::InstanceDependency::instance_notify_deleted(
    RID p_deleted) {
  for (Map<InstanceBase *, uint32_t>::Element *E = instances.front(); E;
       E = E->next()) {
    E->key()->dependency_deleted(p_deleted);
  }
  for (Map<InstanceBase *, uint32_t>::Element *E = instances.front(); E;
       E = E->next()) {
    E->key()->dependencies.erase(this);
  }

  instances.clear();
}

RasterizerScene::InstanceDependency::~InstanceDependency() {
#ifdef DEBUG_ENABLED
  if (instances.size()) {
    WARN_PRINT("Leaked instance dependency: Bug - did not call "
               "instance_notify_deleted when freeing.");
    for (Map<InstanceBase *, uint32_t>::Element *E = instances.front(); E;
         E = E->next()) {
      E->key()->dependencies.erase(this);
    }
  }
#endif
}

Rasterizer *Rasterizer::create() { return _create_func(); }

RasterizerCanvas *RasterizerCanvas::singleton = NULL;

RasterizerStorage *RasterizerStorage::base_singleton = NULL;

RasterizerStorage::RasterizerStorage() { base_singleton = this; }
