
/// \brief Entry point for the Haiku operating system platform.
///
/// Initializes the Godot Engine for the Haiku platform, sets up the main OS instance,
/// and handles the engine lifecycle including setup, execution, and cleanup.
///
/// \param argc Number of command-line arguments passed to the program.
/// \param argv Array of command-line argument strings.
///
/// \return Exit code indicating program termination status:
///         - 0 if the program completed successfully
///         - 255 if engine setup failed
///         - Other values based on OS_Haiku::get_exit_code()
///
/// \note This function manages the following stages:
///       1. OS_Haiku initialization
///       2. Engine setup via Main::setup()
///       3. Engine start and main loop execution via os.run()
///       4. Engine cleanup via Main::cleanup()
#include "main/main.h"
#include "os_haiku.h"

int main(int argc, char *argv[]) {
	OS_Haiku os;

	Error error = Main::setup(argv[0], argc - 1, &argv[1]);
	if (error != OK) {
		return 255;
	}

	if (Main::start()) {
		os.run();
	}

	Main::cleanup();

	return os.get_exit_code();
}
