

/**
 * @class PacketPeer
 * @brief Base class for packet-based communication protocol handling.
 * 
 * PacketPeer provides an abstract interface for sending and receiving packets of data.
 * It manages packet buffering, encoding/decoding of variants, and serves as the foundation
 * for packet-based network communication in the engine.
 * 
 * @note This is an abstract class and cannot be instantiated directly.
 * @see StreamPeer, PacketPeerStream
 */

/**
 * @class PacketPeerStream
 * @brief Concrete implementation of PacketPeer that wraps a StreamPeer for packet communication.
 * 
 * PacketPeerStream extends PacketPeer to provide packet-based communication over a StreamPeer.
 * It uses ring buffers and internal buffers to manage input and output data streams,
 * allowing reliable packet transmission with configurable buffer sizes.
 * 
 * The class manages separate input and output buffers to handle asynchronous packet I/O
 * operations efficiently.
 * 
 * @see PacketPeer, StreamPeer, RingBuffer
 */
#ifndef PACKET_PEER_H
#define PACKET_PEER_H

#include "core/io/stream_peer.h"
#include "core/object.h"
#include "core/ring_buffer.h"

class PacketPeer : public Reference {

	GDCLASS(PacketPeer, Reference);

	Variant _bnd_get_var(bool p_allow_objects = false);

	static void _bind_methods();

	Error _put_packet(const Vector<uint8_t> &p_buffer);
	Vector<uint8_t> _get_packet();
	Error _get_packet_error() const;

	mutable Error last_get_error;

	int encode_buffer_max_size;
	Vector<uint8_t> encode_buffer;

public:
	virtual int get_available_packet_count() const = 0;
	virtual Error get_packet(const uint8_t **r_buffer, int &r_buffer_size) = 0; ///< buffer is GONE after next get_packet
	virtual Error put_packet(const uint8_t *p_buffer, int p_buffer_size) = 0;

	virtual int get_max_packet_size() const = 0;

	/* helpers / binders */

	virtual Error get_packet_buffer(Vector<uint8_t> &r_buffer);
	virtual Error put_packet_buffer(const Vector<uint8_t> &p_buffer);

	virtual Error get_var(Variant &r_variant, bool p_allow_objects = false);
	virtual Error put_var(const Variant &p_packet, bool p_full_objects = false);

	void set_encode_buffer_max_size(int p_max_size);
	int get_encode_buffer_max_size() const;

	PacketPeer();
	~PacketPeer() {}
};

class PacketPeerStream : public PacketPeer {

	GDCLASS(PacketPeerStream, PacketPeer);

	//the way the buffers work sucks, will change later

	mutable Ref<StreamPeer> peer;
	mutable RingBuffer<uint8_t> ring_buffer;
	mutable Vector<uint8_t> input_buffer;
	mutable Vector<uint8_t> output_buffer;

	Error _poll_buffer() const;

protected:
	void _set_stream_peer(REF p_peer);
	static void _bind_methods();

public:
	virtual int get_available_packet_count() const;
	virtual Error get_packet(const uint8_t **r_buffer, int &r_buffer_size);
	virtual Error put_packet(const uint8_t *p_buffer, int p_buffer_size);

	virtual int get_max_packet_size() const;

	void set_stream_peer(const Ref<StreamPeer> &p_peer);
	Ref<StreamPeer> get_stream_peer() const;
	void set_input_buffer_max_size(int p_max_size);
	int get_input_buffer_max_size() const;
	void set_output_buffer_max_size(int p_max_size);
	int get_output_buffer_max_size() const;
	PacketPeerStream();
};

#endif // PACKET_STREAM_H
