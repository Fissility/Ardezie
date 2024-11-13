#pragma once

#include <vector>
#include <functional>
#include <thread>
#include <mutex>

#ifdef __GNUC__
#include <algorithm>
#endif

/*
The Event class handles lists of funtion calls, where the callee or the caller can add functions. When the event is triggered by trigger() or asyncTrigger() all function subscribers get called with the respective paramters
*/
template<typename ... T>
class Event {
private:

	// List of functions that subscribed to the veent
	std::vector<std::function<void(T...)>> listeners;
	// List of events that subscribed to the event
	std::vector<Event<T...>*> chainedEvents;

public:

	Event() {
	}

	Event(const Event<T...>& other) : listeners(other.listeners), chainedEvents(other.chainedEvents) {
	}

	/*
	* @brief Subscribes a function to the event
	*/
	void operator += (const std::function<void(T...)>& f) {
		listeners.push_back(f);
	}

	/*
	* @brief Unsubscribes a function from the event
	*/
	void operator -= (const std::function<void(T...)>& f) {
		auto iterator = std::find(listeners.begin(), listeners.end(), f);
		if (iterator != listeners.end()) {
			listeners.erase(iterator);
		}
	}

	/*
	* @brief Subscribes an event to the event
	*/
	void operator += (Event<T...>& _event) {
		chainedEvents.push_back(&_event);
	}

	/*
	* @brief Synchronous event trigger
	*/
	void trigger(T... args) {
		for (std::function<void(T...)>& f : listeners) {
			f(args...);
		}
		for (Event<T...>* _event : chainedEvents) {
			_event->trigger(args...);
		}
	}

	/*
	* @brief Asynchronous event trigger, doesn't wait for all subscribed functions to load
	*/
	void asyncTrigger(T... args) {
		std::thread([=]() {
			trigger(args...);
			}).detach();
	}

	/*
	* @brief I don't remember why this is here.
	*/
	void asyncTrigger(T... args, const std::function<void(T...)>& onFinish) {
		std::thread([=]() {
			trigger(args...);
			onFinish(args...);
			}).detach();
	}


};