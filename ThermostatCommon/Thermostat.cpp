#include "Thermostat.h"

#include "Zone.h"
#include "ClimateControlCoordinator.h"


Thermostat::Thermostat(ClimateControlEquipment * equipment) :
	_equipment(equipment),
	_coordinator(new ClimateControlCoordinator(_equipment)),
	_zone_list(NULL)
{

}

Thermostat::~Thermostat()
{
	ZoneNode *node = _zone_list;
	_zone_list = NULL;

	while (node)
	{
		ZoneNode *t = node;
		node = node->next;
		delete t;
	}

	delete _coordinator;
}

void Thermostat::add_zone(Zone *zone)
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

	_coordinator->add_zone(zone);
}

void Thermostat::climatize()
{
	// TODO: update the zone set points based on the schedule.
	ZoneNode *node = _zone_list;
	while (node)
	{
		// TODO: update the zone set points based on the schedule here.
		node = node->next;
	}
}
