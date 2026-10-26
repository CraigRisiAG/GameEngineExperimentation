
#include "register_types.h"
#include "core/error_macros.h"
#include "networked_multiplayer_enet.h"

static bool enet_ok = false;

void register_enet_types() {

  if (enet_initialize() != 0) {
    ERR_PRINT("ENet initialization failure");
  } else {
    enet_ok = true;
  }

  ClassDB::register_class<NetworkedMultiplayerENet>();
}

void unregister_enet_types() {

  if (enet_ok)
    enet_deinitialize();
}
