
/**
 * IDHandler - A utility for managing object identifiers
 * 
 * @constructor
 * @description Provides methods to store, retrieve, and manage objects with randomly generated IDs
 * 
 * @method has
 * @param {number} id - The ID to check
 * @returns {boolean} True if the ID exists in the handler, false otherwise
 * 
 * @method add
 * @param {*} obj - The object to add to the handler
 * @returns {number} A randomly generated 32-bit integer ID for the object
 * 
 * @method get
 * @param {number} id - The ID of the object to retrieve
 * @returns {*} The object associated with the ID, or undefined if not found
 * 
 * @method remove
 * @param {number} id - The ID of the object to remove
 * @returns {void}
 * 
 * @method size
 * @returns {number} The current count of objects in the handler
 * 
 * @property {Object} ids - Direct access to the internal IDs object containing all stored objects
 */
var IDHandler = /** @constructor */ function() {

	var ids = {};
	var size = 0;

	this.has = function(id) {
		return ids.hasOwnProperty(id);
	}

	this.add = function(obj) {
		size += 1;
		var id = crypto.getRandomValues(new Int32Array(32))[0];
		ids[id] = obj;
		return id;
	}

	this.get = function(id) {
		return ids[id];
	}

	this.remove = function(id) {
		size -= 1;
		delete ids[id];
	}

	this.size = function() {
		return size;
	}

	this.ids = ids;
};

Module.IDHandler = new IDHandler;
