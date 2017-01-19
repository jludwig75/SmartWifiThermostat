#pragma once

#include <stdint.h>


class ClimateControlEquipment;
class Zone;
class ClimateControlCoordinator;

class Thermostat
{
public:
	Thermostat(ClimateControlEquipment *equipment);
	virtual ~Thermostat();

	void add_zone(Zone *zone);

	void climatize();
private:
	struct ZoneNode
	{
		Zone *zone;
		ZoneNode *next;
	};

	ClimateControlEquipment * _equipment;
	ClimateControlCoordinator *_coordinator;
	ZoneNode *_zone_list;
};
