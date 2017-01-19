#pragma once

#include <stdint.h>


class Zone;

class Sensor
{
public:
	Sensor(uint32_t sensor_id, const char *sensor_name);
	virtual ~Sensor();

	void set_zone(Zone *zone);

	uint32_t get_id() const;
	const char *get_name() const;
	float get_temperature() const;
	float get_humidity() const;

	// Sensor update web server interface
	void update_temperature(float temperature);
	void update_humidity(float humidity);

private:
	uint32_t _id;
	char *_name;
	float _temperature;
	float _humidity;
	Zone *_zone;
};
