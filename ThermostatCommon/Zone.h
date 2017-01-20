#pragma once

#include <stdint.h>


class Sensor;
class ClimateControlCoordinator;


class Zone
{
public:
	Zone(uint32_t id, const char *name);
	virtual ~Zone();
	void set_climate_control_coordinator(ClimateControlCoordinator *coordinator);
	void add_sensor(Sensor *sensor);

	uint32_t get_id() const;
	const char *get_name() const;

	void notify_sensor_conditions_changed();

	void set_cool_set_point(float temperature);
	void set_heat_set_point(float temperature);
	void set_humidity_set_point(float humidity);

private:
	struct SensorNode
	{
		Sensor *sensor;
		SensorNode *next;
	};

	void climatize_zone();

	uint32_t _id;
	char *_name;
	float _heat_point;
	float _cool_point;
	float _humidity_point;
	SensorNode *_sensor_list;
	ClimateControlCoordinator *_coordinator;
};
