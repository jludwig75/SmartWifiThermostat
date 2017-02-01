#pragma once


#include "ClimateControlEquipment.h"

#include <stdint.h>	// for NULL


class Zone;

class ClimateControlCoordinator
{
public:
	ClimateControlCoordinator(ClimateControlEquipment *equipment);
	virtual ~ClimateControlCoordinator();

	void add_zone(Zone *zone);

	void request_mode(Zone *zone, ClimateControlEquipment::Mode mode);

	void request_humidity(Zone *zone);
	void request_no_humidity(Zone *zone);

private:
	struct ZoneNode
	{
		ZoneNode() :
			zone(NULL),
			requested_mode(ClimateControlEquipment::off),
			requesting_humidity(false),
			next(NULL)
		{

		}
		Zone *zone;
		ClimateControlEquipment::Mode requested_mode;
		bool requesting_humidity;
		ZoneNode *next;
	};

	ZoneNode *get_node_for_zone(Zone *zone);

	void climatize_zones();

	ClimateControlEquipment *_equipment;
	ZoneNode *_zone_list;
};
