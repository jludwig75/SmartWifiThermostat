#include <CppUTest/CommandLineTestRunner.h>


#include "Thermostat.h"
#include "Zone.h"
#include "Sensor.h"
#include "ClimateControlEquipment.h"

ClimateControlEquipment::ClimateControlEquipment() : _mode(off), _humidifier_on(false)
{

}

void ClimateControlEquipment::set_mode(Mode mode)
{
	_mode = mode;
}

void ClimateControlEquipment::turn_on_humidifier()
{
	_humidifier_on = true;
}

void ClimateControlEquipment::turn_off_humidifier()
{
	_humidifier_on = false;
}

ClimateControlEquipment::Mode ClimateControlEquipment::get_mode() const
{
	return _mode;
}

bool ClimateControlEquipment::is_humidifier_on() const
{
	return _humidifier_on;
}


TEST_GROUP(Thermostat)
{
	void setup()
	{
		_sensor0 = new Sensor(0, "Hallway");
		_sensor1 = new Sensor(1, "Master Bedroom");
		_sensor2 = new Sensor(2, "West Basement Bedroom");
		_sensor3 = new Sensor(3, "East Basement Bedroom");

		_zone = new Zone(0, "Whole House");
		_zone->add_sensor(_sensor0);
		_zone->add_sensor(_sensor1);
		_zone->add_sensor(_sensor2);
		_zone->add_sensor(_sensor3);

		_thermostat = new Thermostat(&_equipment);
		_thermostat->add_zone(_zone);
	}

	void teardown()
	{
		delete _thermostat;
		delete _zone;
		delete _sensor0;
		delete _sensor1;
		delete _sensor2;
		delete _sensor3;
	}

	Sensor *_sensor0;
	Sensor *_sensor1;
	Sensor *_sensor2;
	Sensor *_sensor3;

	Zone *_zone;

	Thermostat *_thermostat;
	ClimateControlEquipment _equipment;
};

TEST(Thermostat, zone_too_cold_at_one_sensor)
{
	CHECK_EQUAL(ClimateControlEquipment::off, _equipment.get_mode());

	_zone->set_heat_set_point(70);

	_sensor0->update_temperature(68);
	CHECK_EQUAL(ClimateControlEquipment::heat, _equipment.get_mode());
}

TEST(Thermostat, zone_too_cold_at_all_sensors)
{
	CHECK_EQUAL(ClimateControlEquipment::off, _equipment.get_mode());

	_zone->set_heat_set_point(70);

	_sensor0->update_temperature(68);
	_sensor1->update_temperature(69);
	_sensor2->update_temperature(67);
	_sensor3->update_temperature(68);
	CHECK_EQUAL(ClimateControlEquipment::heat, _equipment.get_mode());
}

TEST(Thermostat, zone_too_cold_at_one_sensor_after_set_point_change)
{
	_sensor0->update_temperature(70);
	_sensor1->update_temperature(71);
	_sensor2->update_temperature(71);
	_sensor3->update_temperature(71);

	_zone->set_heat_set_point(68);
	CHECK_EQUAL(ClimateControlEquipment::off, _equipment.get_mode());

	_zone->set_heat_set_point(71);

	CHECK_EQUAL(ClimateControlEquipment::heat, _equipment.get_mode());
}

TEST(Thermostat, zone_too_cold_at_all_sensors_after_set_point_change)
{
	_sensor0->update_temperature(70);
	_sensor1->update_temperature(70);
	_sensor2->update_temperature(70);
	_sensor3->update_temperature(70);

	_zone->set_heat_set_point(68);
	CHECK_EQUAL(ClimateControlEquipment::off, _equipment.get_mode());

	_zone->set_heat_set_point(71);

	CHECK_EQUAL(ClimateControlEquipment::heat, _equipment.get_mode());
}

int main(int ac, char** av)
{
	return CommandLineTestRunner::RunAllTests(ac, av);
}