
/**
 * @class GameCenter
 * @brief Manages Game Center integration for iOS platform.
 * 
 * Provides functionality to authenticate users, post scores, manage achievements,
 * and handle Game Center interactions on iOS devices. This class is only available
 * when GAME_CENTER_ENABLED is defined.
 * 
 * @note This is a singleton class. Use GameCenter::get_singleton() to access the instance.
 */

/**
 * @brief Initiates connection and authentication with Game Center.
 * 
 * Attempts to connect and authenticate the current user with Apple's Game Center service.
 * Results are posted as pending events that can be retrieved via pop_pending_event().
 */

/**
 * @brief Checks if the user is currently authenticated with Game Center.
 * 
 * @return true if user is authenticated, false otherwise.
 */

/**
 * @brief Posts a score to a Game Center leaderboard.
 * 
 * @param p_score A Variant containing the score value and leaderboard identifier.
 * @return Error code indicating success or failure.
 */

/**
 * @brief Awards an achievement to the authenticated user.
 * 
 * @param p_params A Variant containing achievement identifier and progress (0.0-100.0).
 * @return Error code indicating success or failure.
 */

/**
 * @brief Resets all achievements for the current user.
 * 
 * This action is permanent and removes all achievement progress.
 */

/**
 * @brief Requests a list of achievements the user has earned.
 * 
 * Results are posted as pending events.
 */

/**
 * @brief Requests descriptions of all available achievements.
 * 
 * Retrieves achievement metadata including titles and descriptions.
 * Results are posted as pending events.
 */

/**
 * @brief Displays the Game Center user interface.
 * 
 * @param p_params A Variant containing display parameters (leaderboard, achievement, etc.).
 * @return Error code indicating success or failure.
 */

/**
 * @brief Requests an identity verification signature from Game Center.
 * 
 * Used for server-side verification of player identity.
 * @return Error code indicating success or failure.
 */

/**
 * @brief Called when the Game Center interface is closed by the user.
 * 
 * Internal callback for handling Game Center UI closure.
 */

/**
 * @brief Gets the number of pending events from Game Center operations.
 * 
 * @return Count of unprocessed events in the pending queue.
 */

/**
 * @brief Retrieves and removes the next pending event from the queue.
 * 
 * @return A Variant containing the event data, or an empty Variant if queue is empty.
 */

/**
 * @brief Returns the singleton instance of GameCenter.
 * 
 * @return Pointer to the GameCenter singleton instance.
 */
#ifdef GAME_CENTER_ENABLED

#ifndef GAME_CENTER_H
#define GAME_CENTER_H

#include "core/object.h"

class GameCenter : public Object {

	GDCLASS(GameCenter, Object);

	static GameCenter *instance;
	static void _bind_methods();

	List<Variant> pending_events;

	bool authenticated;

	void return_connect_error(const char *p_error_description);

public:
	void connect();
	bool is_authenticated();

	Error post_score(Variant p_score);
	Error award_achievement(Variant p_params);
	void reset_achievements();
	void request_achievements();
	void request_achievement_descriptions();
	Error show_game_center(Variant p_params);
	Error request_identity_verification_signature();

	void game_center_closed();

	int get_pending_event_count();
	Variant pop_pending_event();

	static GameCenter *get_singleton();

	GameCenter();
	~GameCenter();
};

#endif

#endif
