
/**
 * @file javascript_main.cpp
 * @brief Main entry point for the Godot Engine running on Emscripten/WebAssembly.
 * 
 * This file handles the initialization of the JavaScript/Emscripten platform.
 * It manages IndexedDB filesystem synchronization and delegates to the platform-specific
 * OS_JavaScript class for further setup and execution.
 * 
 * @details
 * The initialization flow:
 * 1. main() sets up the IndexedDB filesystem mount point at '/userfs'
 * 2. Synchronizes persistent IndexedDB data into the Emscripten FS
 * 3. Calls main_after_fs_sync() once synchronization completes
 * 4. main_after_fs_sync() configures resource loading and starts the main engine loop asynchronously
 */

/**
 * @fn void main_after_fs_sync(char *p_idbfs_err)
 * @brief Callback function invoked after IndexedDB filesystem synchronization completes.
 * 
 * @param p_idbfs_err C-string containing error message from IndexedDB sync, or empty string on success
 * 
 * @details
 * Configures the OS singleton with IndexedDB availability status, sets resource loader
 * to not abort on missing resources for better web compatibility, and initiates the
 * main engine setup and async runtime loop.
 */

/**
 * @fn int main(int argc, char *argv[])
 * @brief Entry point for the Emscripten/JavaScript build of the Godot Engine.
 * 
 * @param argc Argument count
 * @param argv Argument vector
 * @return int Exit status (execution continues asynchronously via callback)
 * 
 * @details
 * Initializes the IndexedDB filesystem, mounts it at '/userfs', and synchronizes
 * persisted data before delegating to main_after_fs_sync() via callback.
 * The actual engine startup is deferred until filesystem sync completes.
 */
#include "core/io/resource_loader.h"
#include "main/main.h"
#include "os_javascript.h"

#include <emscripten/emscripten.h>

extern "C" EMSCRIPTEN_KEEPALIVE void main_after_fs_sync(char *p_idbfs_err) {

	String idbfs_err = String::utf8(p_idbfs_err);
	if (!idbfs_err.empty()) {
		print_line("IndexedDB not available: " + idbfs_err);
	}
	OS_JavaScript *os = OS_JavaScript::get_singleton();
	os->set_idb_available(idbfs_err.empty());
	// Ease up compatibility.
	ResourceLoader::set_abort_on_missing_resources(false);
	Main::start();
	os->run_async();
}

int main(int argc, char *argv[]) {

	// Sync from persistent state into memory and then
	// run the 'main_after_fs_sync' function.
	/* clang-format off */
	EM_ASM(
		FS.mkdir('/userfs');
		FS.mount(IDBFS, {}, '/userfs');
		FS.syncfs(true, function(err) {
			ccall('main_after_fs_sync', null, ['string'], [err ? err.message : ""])
		});
	);
	/* clang-format on */

	new OS_JavaScript(argc, argv);
	// TODO: Check error return value.
	Main::setup(argv[0], argc - 1, &argv[1]);

	return 0;
	// Continued async in main_after_fs_sync() from the syncfs() callback.
}
