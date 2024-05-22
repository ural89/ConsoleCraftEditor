#pragma once
enum class EventType
{
	None = 0,
	OnEnemySpawned = 1,
	OnEnemyKilled = 2,
};
class Event
{
public:
	Event(EventType eventType)
	{
		this->eventType = eventType;
	}
	EventType GetEventType() const
	{
		return eventType;
	}

private:
	EventType eventType;
};

