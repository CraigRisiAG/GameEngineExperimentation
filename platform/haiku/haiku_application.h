
/**
 * @class HaikuApplication
 * @brief Application class for Haiku OS platform integration.
 * 
 * HaikuApplication extends BApplication to provide platform-specific
 * application functionality for the Haiku operating system. It serves as
 * the main application object that manages the lifecycle and event handling
 * for Haiku-based applications.
 * 
 * @note Requires kernel/image.h for image_id support and the Haiku API.
 */
#ifndef HAIKU_APPLICATION_H
#define HAIKU_APPLICATION_H

#include <kernel/image.h> // needed for image_id

#include <Application.h>

class HaikuApplication : public BApplication {
public:
	HaikuApplication();
};

#endif
