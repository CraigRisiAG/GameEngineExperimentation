
/// @class HaikuApplication
/// @brief Haiku platform-specific application class that extends BApplication.
///
/// Initializes a Haiku application with the vendor-specific MIME type identifier.
/// This class serves as the entry point for Haiku OS integration within the game engine.
///
/// @details
/// The constructor registers the application with Haiku's application server using
/// the MIME type "application/x-vnd.godot" for proper system integration.
#include "haiku_application.h"

HaikuApplication::HaikuApplication() :
		BApplication("application/x-vnd.godot") {
}
