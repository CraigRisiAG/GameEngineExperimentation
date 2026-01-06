
/**
 * Game Engine initialization and management class.
 * Handles WebAssembly loading, canvas setup, and game execution.
 * 
 * @class Engine
 * @global
 * 
 * @example
 * const engine = new Engine();
 * engine.setCanvas(document.getElementById('canvas'));
 * engine.startGame('game', 'main.pck');
 */

/**
 * Loads the WebAssembly engine file from the specified base path.
 * @param {string} basePath - The base path to load the engine from
 * @returns {Promise<void>} Promise that resolves when loading is complete
 */

/**
 * Unloads the engine resources.
 * @returns {void}
 */

/**
 * Initializes the engine with optional base path.
 * @param {string=} basePath - Optional base path for engine initialization
 * @returns {Promise<void>} Promise that resolves when initialization is complete
 */

/**
 * Preloads a file for use during game execution.
 * @param {string} file - The filename to preload
 * @param {string} path - The file path/URL
 * @returns {Object} Preload object with status
 */

/**
 * Starts the engine with the given arguments.
 * @param {...string} args - Command line arguments to pass to the engine
 * @returns {Promise<void>} Promise that resolves when the engine starts
 */

/**
 * Starts the game with the specified executable and main package.
 * @param {string} execName - The executable name
 * @param {string} mainPack - The main package file path
 * @returns {Promise<void>} Promise that resolves when game starts
 */

/**
 * Sets the WebAssembly filename extension override.
 * @param {string} override - The new extension (e.g., '.wasm')
 * @throws {Error} If override is an empty string
 */

/**
 * Sets whether to unload after initialization.
 * @param {boolean} enabled - True to unload after init, false otherwise
 */

/**
 * Sets the canvas element for rendering.
 * @param {HTMLCanvasElement} canvasElem - The canvas element to use
 */

/**
 * Sets whether the canvas should be resized on start.
 * @param {boolean} enabled - True to resize canvas on start
 */

/**
 * Sets the locale for the engine.
 * @param {string} locale - Locale string (e.g., 'en_US')
 */

/**
 * Sets the executable name.
 * @param {string} newName - The executable name
 */

/**
 * Sets the progress callback function.
 * @param {Function} func - Callback function for progress updates
 */

/**
 * Sets the stdout output function.
 * @param {Function} func - Callback function for stdout messages
 */

/**
 * Sets the stderr output function.
 * @param {Function} func - Callback function for stderr messages
 */
Function('return this')()['Engine'] = (function() {

	var unloadAfterInit = true;
	var canvas = null;
	var resizeCanvasOnStart = false;
	var customLocale = 'en_US';
	var wasmExt = '.wasm';

	var preloader = new Preloader();
	var loader = new Loader();
	var rtenv = null;

	var executableName = '';
	var loadPath = '';
	var loadPromise = null;
	var initPromise = null;
	var stderr = null;
	var stdout = null;
	var progressFunc = null;

	function load(basePath) {
		if (loadPromise == null) {
			loadPath = basePath;
			loadPromise = preloader.loadPromise(basePath + wasmExt);
			preloader.setProgressFunc(progressFunc);
			requestAnimationFrame(preloader.animateProgress);
		}
		return loadPromise;
	};

	function unload() {
		loadPromise = null;
	};

	/** @constructor */
	function Engine() {};

	Engine.prototype.init = /** @param {string=} basePath */ function(basePath) {
		if (initPromise) {
			return initPromise;
		}
		if (!loadPromise) {
			if (!basePath) {
				initPromise = Promise.reject(new Error("A base path must be provided when calling `init` and the engine is not loaded."));
				return initPromise;
			}
			load(basePath);
		}
		var config = {}
		if (typeof stdout === 'function')
			config.print = stdout;
		if (typeof stderr === 'function')
			config.printErr = stderr;
		initPromise = loader.init(loadPromise, loadPath, config).then(function() {
			return new Promise(function(resolve, reject) {
				rtenv = loader.env;
				if (unloadAfterInit) {
					loadPromise = null;
				}
				resolve();
			});
		});
		return initPromise;
	};

	/** @type {function(string, string):Object} */
	Engine.prototype.preloadFile = function(file, path) {
		return preloader.preload(file, path);
	};

	/** @type {function(...string):Object} */
	Engine.prototype.start = function() {
		// Start from arguments.
		var args = [];
		for (var i = 0; i < arguments.length; i++) {
			args.push(arguments[i]);
		}
		var me = this;
		return new Promise(function(resolve, reject) {
			return me.init().then(function() {
				if (!(canvas instanceof HTMLCanvasElement)) {
					canvas = Utils.findCanvas();
				}
				rtenv['locale'] = customLocale;
				rtenv['canvas'] = canvas;
				rtenv['thisProgram'] = executableName;
				rtenv['resizeCanvasOnStart'] = resizeCanvasOnStart;
				loader.start(preloader.preloadedFiles, args).then(function() {
					loader = null;
					initPromise = null;
					resolve();
				});
			});
		});
	};

	Engine.prototype.startGame = function(execName, mainPack) {
		// Start and init with execName as loadPath if not inited.
		executableName = execName;
		var me = this;
		return Promise.all([
			this.init(execName),
			this.preloadFile(mainPack, mainPack)
		]).then(function() {
			return me.start('--main-pack', mainPack);
		});
	};

	Engine.prototype.setWebAssemblyFilenameExtension = function(override) {
		if (String(override).length === 0) {
			throw new Error('Invalid WebAssembly filename extension override');
		}
		wasmExt = String(override);
	};

	Engine.prototype.setUnloadAfterInit = function(enabled) {
		unloadAfterInit = enabled;
	};

	Engine.prototype.setCanvas = function(canvasElem) {
		canvas = canvasElem;
	};

	Engine.prototype.setCanvasResizedOnStart = function(enabled) {
		resizeCanvasOnStart = enabled;
	};

	Engine.prototype.setLocale = function(locale) {
		customLocale = locale;
	};

	Engine.prototype.setExecutableName = function(newName) {
		executableName = newName;
	};

	Engine.prototype.setProgressFunc = function(func) {
		progressFunc = func;
	}

	Engine.prototype.setStdoutFunc = function(func) {

		var print = function(text) {
			if (arguments.length > 1) {
				text = Array.prototype.slice.call(arguments).join(" ");
			}
			func(text);
		};
		if (rtenv)
			rtenv.print = print;
		stdout = print;
	};

	Engine.prototype.setStderrFunc = function(func) {

		var printErr = function(text) {
			if (arguments.length > 1)
				text = Array.prototype.slice.call(arguments).join(" ");
			func(text);
		};
		if (rtenv)
			rtenv.printErr = printErr;
		stderr = printErr;
	};

	// Closure compiler exported engine methods.
	/** @export */
	Engine['isWebGLAvailable'] = Utils.isWebGLAvailable;
	Engine['load'] = load;
	Engine['unload'] = unload;
	Engine.prototype['init'] = Engine.prototype.init
	Engine.prototype['preloadFile'] = Engine.prototype.preloadFile
	Engine.prototype['start'] = Engine.prototype.start
	Engine.prototype['startGame'] = Engine.prototype.startGame
	Engine.prototype['setWebAssemblyFilenameExtension'] = Engine.prototype.setWebAssemblyFilenameExtension
	Engine.prototype['setUnloadAfterInit'] = Engine.prototype.setUnloadAfterInit
	Engine.prototype['setCanvas'] = Engine.prototype.setCanvas
	Engine.prototype['setCanvasResizedOnStart'] = Engine.prototype.setCanvasResizedOnStart
	Engine.prototype['setLocale'] = Engine.prototype.setLocale
	Engine.prototype['setExecutableName'] = Engine.prototype.setExecutableName
	Engine.prototype['setProgressFunc'] = Engine.prototype.setProgressFunc
	Engine.prototype['setStdoutFunc'] = Engine.prototype.setStdoutFunc
	Engine.prototype['setStderrFunc'] = Engine.prototype.setStderrFunc
	return Engine;
})();
