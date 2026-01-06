
/**
 * Preloader constructor for managing file downloads and preloading.
 * Handles XMLHttpRequest-based file loading with retry logic and progress tracking.
 * 
 * @constructor
 * @property {Array<{path: string, buffer: ArrayBuffer|Uint8Array}>} preloadedFiles - Array of preloaded files
 * 
 * @method loadPromise(file) - Returns a Promise that resolves when a file is loaded
 * @param {string} file - The file path to load
 * @returns {Promise<XMLHttpRequest>} Promise that resolves with the XMLHttpRequest object
 * 
 * @method preload(pathOrBuffer, destPath) - Preloads a file or buffer to memory
 * @param {string|ArrayBuffer|Uint8Array} pathOrBuffer - File path or buffer data to preload
 * @param {string} [destPath] - Destination path for the preloaded file (optional, defaults to source path)
 * @returns {Promise<void>} Promise that resolves when preloading is complete
 * 
 * @method setProgressFunc(callback) - Sets a callback function to track loading progress
 * @param {Function} callback - Callback function with signature (loaded: number, total: number)
 * 
 * @method animateProgress() - Animates and tracks progress of all loading files
 * Uses requestAnimationFrame to continuously update progress until all files are loaded
 * 
 * @throws {Promise} Rejects with error if preload() is called with invalid object type
 */
var Preloader = /** @constructor */ function() {

	var DOWNLOAD_ATTEMPTS_MAX = 4;
	var progressFunc = null;
	var lastProgress = { loaded: 0, total: 0 };

	var loadingFiles = {};
	this.preloadedFiles = [];

	function loadXHR(resolve, reject, file, tracker) {
		var xhr = new XMLHttpRequest;
		xhr.open('GET', file);
		if (!file.endsWith('.js')) {
			xhr.responseType = 'arraybuffer';
		}
		['loadstart', 'progress', 'load', 'error', 'abort'].forEach(function(ev) {
			xhr.addEventListener(ev, onXHREvent.bind(xhr, resolve, reject, file, tracker));
		});
		xhr.send();
	}

	function onXHREvent(resolve, reject, file, tracker, ev) {

		if (this.status >= 400) {

			if (this.status < 500 || ++tracker[file].attempts >= DOWNLOAD_ATTEMPTS_MAX) {
				reject(new Error("Failed loading file '" + file + "': " + this.statusText));
				this.abort();
				return;
			} else {
				setTimeout(loadXHR.bind(null, resolve, reject, file, tracker), 1000);
			}
		}

		switch (ev.type) {
			case 'loadstart':
				if (tracker[file] === undefined) {
					tracker[file] = {
						total: ev.total,
						loaded: ev.loaded,
						attempts: 0,
						final: false,
					};
				}
				break;

			case 'progress':
				tracker[file].loaded = ev.loaded;
				tracker[file].total = ev.total;
				break;

			case 'load':
				tracker[file].final = true;
				resolve(this);
				break;

			case 'error':
				if (++tracker[file].attempts >= DOWNLOAD_ATTEMPTS_MAX) {
					tracker[file].final = true;
					reject(new Error("Failed loading file '" + file + "'"));
				} else {
					setTimeout(loadXHR.bind(null, resolve, reject, file, tracker), 1000);
				}
				break;

			case 'abort':
				tracker[file].final = true;
				reject(new Error("Loading file '" + file + "' was aborted."));
				break;
		}
	}

	this.loadPromise = function(file) {
		return new Promise(function(resolve, reject) {
			loadXHR(resolve, reject, file, loadingFiles);
		});
	}

	this.preload = function(pathOrBuffer, destPath) {
		if (pathOrBuffer instanceof ArrayBuffer) {
			pathOrBuffer = new Uint8Array(pathOrBuffer);
		} else if (ArrayBuffer.isView(pathOrBuffer)) {
			pathOrBuffer = new Uint8Array(pathOrBuffer.buffer);
		}
		if (pathOrBuffer instanceof Uint8Array) {
			this.preloadedFiles.push({
				path: destPath,
				buffer: pathOrBuffer
			});
			return Promise.resolve();
		} else if (typeof pathOrBuffer === 'string') {
			var me = this;
			return this.loadPromise(pathOrBuffer).then(function(xhr) {
				me.preloadedFiles.push({
					path: destPath || pathOrBuffer,
					buffer: xhr.response
				});
				return Promise.resolve();
			});
		} else {
			throw Promise.reject("Invalid object for preloading");
		}
	};

	var animateProgress = function() {

		var loaded = 0;
		var total = 0;
		var totalIsValid = true;
		var progressIsFinal = true;

		Object.keys(loadingFiles).forEach(function(file) {
			const stat = loadingFiles[file];
			if (!stat.final) {
				progressIsFinal = false;
			}
			if (!totalIsValid || stat.total === 0) {
				totalIsValid = false;
				total = 0;
			} else {
				total += stat.total;
			}
			loaded += stat.loaded;
		});
		if (loaded !== lastProgress.loaded || total !== lastProgress.total) {
			lastProgress.loaded = loaded;
			lastProgress.total = total;
			if (typeof progressFunc === 'function')
				progressFunc(loaded, total);
		}
		if (!progressIsFinal)
			requestAnimationFrame(animateProgress);
	}
	this.animateProgress = animateProgress; // Also exposed to start it.

	this.setProgressFunc = function(callback) {
		progressFunc = callback;
	}
};
