
/**
 * Loader constructor for initializing and starting a game engine module.
 * Manages the loading of WebAssembly modules and file system operations.
 * 
 * @constructor
 * @property {Object} env - The initialized Godot/game engine module environment
 * 
 * @method init
 * @param {Promise} loadPromise - Promise that resolves when the engine module is loaded
 * @param {string} basePath - Base path for locating engine files
 * @param {Object} [config] - Optional configuration object for the engine
 * @returns {Promise<void>} Promise that resolves when initialization is complete
 * 
 * @method start
 * @param {Array<{path: string, buffer: ArrayBuffer}>} preloadedFiles - Files to copy to the virtual file system before starting
 * @param {Array<string>} args - Command line arguments to pass to the engine's main function
 * @returns {Promise<void>} Promise that resolves when the engine has started
 */
var Loader = /** @constructor */ function() {

	this.env = null;

	this.init = function(loadPromise, basePath, config) {
		var me = this;
		return new Promise(function(resolve, reject) {
			var cfg = config || {};
			cfg['locateFile'] = Utils.createLocateRewrite(basePath);
			cfg['instantiateWasm'] = Utils.createInstantiatePromise(loadPromise);
			loadPromise = null;
			Godot(cfg).then(function(module) {
				me.env = module;
				resolve();
			});
		});
	}

	this.start = function(preloadedFiles, args) {
		var me = this;
		return new Promise(function(resolve, reject) {
			if (!me.env) {
				reject(new Error('The engine must be initialized before it can be started'));
			}
			preloadedFiles.forEach(function(file) {
				Utils.copyToFS(me.env['FS'], file.path, file.buffer);
			});
			preloadedFiles.length = 0; // Clear memory
			me.env['callMain'](args);
			resolve();
		});
	}
};
