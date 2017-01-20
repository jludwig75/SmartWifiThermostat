#include "Sensor.h"

#include "Zone.h"

#include <string.h>
#include <malloc.h>


static float fabs(float val)
{
	return val >= 0 ? val : -val;
}

// TODO: These could be project defines:
#define TEMPERATURE_CHANGE_THRESHOLD	0.25
#define HUMIDITY_CHANGE_THRESHOLD		1

static bool temperature_changed(float old_temperature, float new_temperature)
{
	return fabs(new_temperature - old_temperature) >= TEMPERATURE_CHANGE_THRESHOLD;
}

static bool humidity_changed(float old_humidity, float new_humidity)
{
	return fabs(new_humidity - old_humidity) >= HUMIDITY_CHANGE_THRESHOLD;
}


Sensor::Sensor(uint32_t sensor_id, const char *sensor_name) :
	_id(sensor_id),
	_name(_strdup(sensor_name)),
	_temperature(71),
	_humidity(60),
	_zone(NULL)
{

}

Sensor::~Sensor()
{
	free(_name);
}

void Sensor::set_zone(Zone *zone)
{
	_zone = zone;
}

uint32_t Sensor::get_id() const
{
	return _id;
}

const char *Sensor::get_name() const
{
	return _name;
}

float Sensor::get_temperature() const
{
	return _temperature;
}

float Sensor::get_humidity() const
{
	return _humidity;
}


// Sensor update web server interface
void Sensor::update_temperature(float temperature)
{
	bool notify = temperature_changed(_temperature, temperature);

	_temperature = temperature;

	if (notify && _zone != NULL)
	{
		_zone->notify_sensor_conditions_changed();
	}
}

void Sensor::update_humidity(float humidity)
{
	bool notify = humidity_changed(_humidity, humidity);

	_humidity = humidity;

	if (notify && _zone != NULL)
	{
		_zone->notify_sensor_conditions_changed();
	}
}

