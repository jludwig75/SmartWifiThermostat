#include "Zone.h"

#include "Sensor.h"
#include "ClimateControlCoordinator.h"

#include <string.h>
#include <malloc.h>
#include <assert.h>


Zone::Zone(uint32_t id, const char *name) :
	_id(id),
	_name(_strdup(name)),
	_heat_point(60),
	_cool_point(80),
	_humidity_point(0),
	_sensor_list(NULL),
	_coordinator(NULL)
{

}

Zone::~Zone()
{
	SensorNode *node = _sensor_list;
	_sensor_list = NULL;

	while (node)
	{
		SensorNode *t = node;
		node = node->next;
		t->sensor->set_zone(NULL);
		delete t;
	}

	free(_name);
}

void Zone::set_climate_control_coordinator(ClimateControlCoordinator *coordinator)
{
	_coordinator = coordinator;
}


void Zone::add_sensor(Sensor *sensor)
{
	SensorNode *node = new SensorNode;

	node->next = NULL;
	node->sensor = sensor;

	if (_sensor_list == NULL)
	{
		_sensor_list = node;
	}
	else
	{
		SensorNode *n = _sensor_list;
		while (n->next)
		{
			n = n->next;
		}

		n->next = node;
	}

	sensor->set_zone(this);
}

uint32_t Zone::get_id() const
{
	return _id;
}

const char *Zone::get_name() const
{
	return _name;
}

void Zone::climatize_zone()
{
	unsigned sensors_requesting_heat = 0;
	unsigned sensors_requesting_cool = 0;
	unsigned sensors_requesting_humidity = 0;
	unsigned total_sensors = 0;

	SensorNode *node = _sensor_list;

	ClimateControlEquipment::Mode request_mode = ClimateControlEquipment::off;

	while (node)
	{
		Sensor *sensor = node->sensor;
		total_sensors++;

		float temperature = sensor->get_temperature();
		float humidity = sensor->get_humidity();

		if (temperature < _heat_point)
		{
			sensors_requesting_heat++;
		}

		if (temperature > _cool_point)
		{
			sensors_requesting_cool++;
		}

		if (humidity < _humidity_point)
		{
			sensors_requesting_humidity++;
		}

		node = node->next;
	}

	if (sensors_requesting_heat > sensors_requesting_cool)
	{
		request_mode = ClimateControlEquipment::heat;
		_coordinator->request_mode(this, request_mode);
	}
	else if (sensors_requesting_cool > sensors_requesting_heat) {
		request_mode = ClimateControlEquipment::cool;
		_coordinator->request_mode(this, request_mode);
	}
	else if (sensors_requesting_cool > 0)
	{
		// This must be > 0 too, because the above cases handle inequality.
		assert(sensors_requesting_heat > 0);

		// It's a tie between the sensors requesting heat and the sensors requestion cool.
		// This means we need to circulate the air.
		request_mode = ClimateControlEquipment::fan_only;
		_coordinator->request_mode(this, request_mode);
	}
	else
	{
		// Optimization so we don't turn around and
		// request the fan again for the humidity below.
		if (0 == sensors_requesting_humidity)
		{
			request_mode = ClimateControlEquipment::off;
			_coordinator->request_mode(this, request_mode);
		}
	}

	if (sensors_requesting_humidity > total_sensors / 2)
	{
		_coordinator->request_humidity(this);
	}
	else
	{
		if (sensors_requesting_humidity > 0)
		{
			// At least one sensor wants humidity, but not all.
			// This means we need to circulate the air.

			if (ClimateControlEquipment::off == request_mode)
			{
				request_mode = ClimateControlEquipment::fan_only;
				_coordinator->request_mode(this, request_mode);
			}
		}
	}
}

void Zone::notify_sensor_conditions_changed()
{
	climatize_zone();
}

void Zone::set_cool_set_point(float temperature)
{
	_cool_point = temperature;
	climatize_zone();
}

void Zone::set_heat_set_point(float temperature)
{
	_heat_point = temperature;
	climatize_zone();
}

void Zone::set_humidity_set_point(float humidity)
{
	_humidity_point = humidity;
	climatize_zone();
}

