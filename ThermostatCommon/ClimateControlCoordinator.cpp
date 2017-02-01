#include "ClimateControlCoordinator.h"

#include "Zone.h"

#include <assert.h>


ClimateControlCoordinator::ClimateControlCoordinator(ClimateControlEquipment *equipment) :
	_equipment(equipment),
	_zone_list(NULL)
{

}

ClimateControlCoordinator::~ClimateControlCoordinator()
{
	ZoneNode *node = _zone_list;
	_zone_list = NULL;

	while (node)
	{
		ZoneNode *t = node;
		node = node->next;
		t->zone->set_climate_control_coordinator(NULL);
		delete t;
	}
}

void ClimateControlCoordinator::add_zone(Zone *zone)
{
	ZoneNode *node = new ZoneNode;

	node->next = NULL;
	node->zone = zone;

	if (_zone_list == NULL)
	{
		_zone_list = node;
	}
	else
	{
		ZoneNode *n = _zone_list;
		while (n->next)
		{
			n = n->next;
		}

		n->next = node;
	}

	zone->set_climate_control_coordinator(this);
}

ClimateControlCoordinator::ZoneNode *ClimateControlCoordinator::get_node_for_zone(Zone *zone)
{
	ZoneNode *node = _zone_list;
	while (node)
	{
		if (node->zone == zone)
		{
			return node;
		}

		node = node->next;
	}

	return NULL;
}

void ClimateControlCoordinator::request_mode(Zone *zone, ClimateControlEquipment::Mode mode)
{

	ZoneNode *node = get_node_for_zone(zone);
	assert(node != NULL);

	node->requested_mode = mode;

	climatize_zones();
}

void ClimateControlCoordinator::request_humidity(Zone *zone)
{
	ZoneNode *node = get_node_for_zone(zone);
	assert(node != NULL);

	node->requesting_humidity = true;

	climatize_zones();
}

void ClimateControlCoordinator::request_no_humidity(Zone *zone)
{
	ZoneNode *node = get_node_for_zone(zone);
	assert(node != NULL);

	node->requesting_humidity = false;

	climatize_zones();
}

void ClimateControlCoordinator::climatize_zones()
{
	unsigned zones_requesting_heat = 0;
	unsigned zones_requesting_cool = 0;
	unsigned zones_requesting_humidity = 0;
	unsigned total_zones = 0;

	ZoneNode *node = _zone_list;
	while (node)
	{
		total_zones++;

		if (ClimateControlEquipment::heat == node->requested_mode)
		{
			zones_requesting_heat++;
		}
		else if (ClimateControlEquipment::cool == node->requested_mode)
		{
			zones_requesting_cool++;
		}

		if (node->requesting_humidity)
		{
			zones_requesting_humidity++;
		}
		node = node->next;
	}

	if (zones_requesting_heat > zones_requesting_cool)
	{
		_equipment->set_mode(ClimateControlEquipment::heat);
	}
	else if (zones_requesting_cool > zones_requesting_cool)
	{
		_equipment->set_mode(ClimateControlEquipment::cool);
	}
	else if (zones_requesting_cool > 0)
	{
		assert(zones_requesting_heat > 0);

		_equipment->set_mode(ClimateControlEquipment::fan_only);
	}
	else
	{
		// This is an optimization so we don't turn off the
		// humidifier just to turn it on below.
		if (0 == zones_requesting_humidity)
		{
			_equipment->set_mode(ClimateControlEquipment::off);
		}
	}

	if (zones_requesting_humidity > total_zones / 2)
	{
		_equipment->turn_on_humidifier();
	}
	else 
	{
		// Less than half (or so) of the zones are not reuqesting
		// humidity so turn off the humidifier.
		_equipment->turn_off_humidifier();

		if (zones_requesting_humidity > 0)
		{
			// At least one zone is requesting humidity.
			// Don't turn on the humidifer, just circulate.
			if (ClimateControlEquipment::off == _equipment->get_mode())
			{
				_equipment->set_mode(ClimateControlEquipment::fan_only);
			}
		}
	}
}
