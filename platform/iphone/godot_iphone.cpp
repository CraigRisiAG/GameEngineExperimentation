
/**
 * @brief Initializes the iPhone OS and Godot engine.
 * 
 * Sets up the working directory based on the executable path, creates an OSIPhone instance,
 * and initializes the Godot engine with the provided arguments and configuration.
 * 
 * @param width The width of the display in pixels.
 * @param height The height of the display in pixels.
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @param data_dir The directory path where engine data is located.
 * 
 * @return 0 on successful initialization, 255 if engine setup fails.
 * 
 * @note This function must be called once during application startup.
 * @see iphone_finish()
 */

/**
 * @brief Cleans up and shuts down the Godot engine on iPhone.
 * 
 * Performs cleanup operations, tears down the engine, and releases the OSIPhone instance.
 * Should be called during application termination.
 * 
 * @return void
 * 
 * @note Must be called after iphone_main() to properly release resources.
 * @see iphone_main()
 */
#include "core/ustring.h"
#include "main/main.h"
#include "os_iphone.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

static OSIPhone *os = NULL;

extern "C" {
int add_path(int p_argc, char **p_args);
int add_cmdline(int p_argc, char **p_args);
};

int iphone_main(int, int, int, char **, String);

int iphone_main(int width, int height, int argc, char **argv, String data_dir) {

	size_t len = strlen(argv[0]);

	while (len--) {
		if (argv[0][len] == '/') break;
	}

	if (len >= 0) {
		char path[512];
		memcpy(path, argv[0], len > sizeof(path) ? sizeof(path) : len);
		path[len] = 0;
		printf("Path: %s\n", path);
		chdir(path);
	}

	printf("godot_iphone %s\n", argv[0]);
	char cwd[512];
	getcwd(cwd, sizeof(cwd));
	printf("cwd %s\n", cwd);
	os = new OSIPhone(width, height, data_dir);

	char *fargv[64];
	for (int i = 0; i < argc; i++) {
		fargv[i] = argv[i];
	};
	fargv[argc] = NULL;
	argc = add_path(argc, fargv);
	argc = add_cmdline(argc, fargv);

	printf("os created\n");
	Error err = Main::setup(fargv[0], argc - 1, &fargv[1], false);
	printf("setup %i\n", err);
	if (err != OK)
		return 255;

	return 0;
};

void iphone_finish() {

	printf("iphone_finish\n");
	Main::cleanup();
	delete os;
};
