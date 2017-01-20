#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTestExt/MockSupport.h>

#include "zone.h"
#include "Sensor.h"
#include "ClimateControlCoordinator.h"


Sensor::Sensor(uint32_t sensor_id, const char *sensor_name)
{

}

Sensor::~Sensor()
{

}

void Sensor::set_zone(Zone *zone)
{
	mock().actualCall("Sensor::set_zone").
		withPointerParameter("zone", zone);
}

float Sensor::get_temperature() const
{
	return (float)mock().actualCall("Sensor::get_temperature").
		returnDoubleValue();
}

float Sensor::get_humidity() const
{
	return (float)mock().actualCall("Sensor::get_humidity").
		returnDoubleValue();
}



ClimateControlCoordinator::ClimateControlCoordinator(ClimateControlEquipment *equipment)
{

}

ClimateControlCoordinator::~ClimateControlCoordinator()
{

}

void ClimateControlCoordinator::request_mode(Zone *zone, ClimateControlEquipment::Mode mode)
{
	mock().actualCall("ClimateControlCoordinator::request_mode").
		withPointerParameter("zone", zone).
		withUnsignedIntParameter("mode", (unsigned)mode);
}

void ClimateControlCoordinator::request_humidity(Zone *zone)
{
	mock().actualCall("ClimateControlCoordinator::request_humidity").
		withPointerParameter("zone", zone);
}


TEST_GROUP(Zone)
{
	void setup()
	{

	}

	void teardown()
	{
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(Zone, add_sensors)
{
	Zone *zone = new Zone(2, "Basement");
	ClimateControlCoordinator coordinator(NULL);

	zone->set_climate_control_coordinator(&coordinator);

	Sensor sensor0(3, "Family Room");
	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", zone);
	zone->add_sensor(&sensor0);

	Sensor sensor1(4, "Bedroom");
	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", zone);
	zone->add_sensor(&sensor1);

	Sensor sensor2(5, "Bedroom");
	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", zone);
	zone->add_sensor(&sensor2);

	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", NULL);
	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", NULL);
	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", NULL);
	delete zone;
}

TEST(Zone, zone_requests_heat_when_one_sensor_below_heat_set_point)
{
	Zone *zone = new Zone(2, "Basement");
	ClimateControlCoordinator coordinator(NULL);

	zone->set_climate_control_coordinator(&coordinator);

	Sensor sensor0(3, "Family Room");
	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", zone);
	zone->add_sensor(&sensor0);

	Sensor sensor1(4, "Bedroom");
	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", zone);
	zone->add_sensor(&sensor1);

	Sensor sensor2(5, "Bedroom");
	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", zone);
	zone->add_sensor(&sensor2);

	mock().expectOneCall("Sensor::get_temperature").
		andReturnValue((double)68);
	mock().expectOneCall("Sensor::get_humidity").
		andReturnValue((double)45);

	mock().expectOneCall("Sensor::get_temperature").
		andReturnValue((double)71);
	mock().expectOneCall("Sensor::get_humidity").
		andReturnValue((double)45);

	mock().expectOneCall("Sensor::get_temperature").
		andReturnValue((double)71);
	mock().expectOneCall("Sensor::get_humidity").
		andReturnValue((double)45);

	mock().expectOneCall("ClimateControlCoordinator::request_mode").
		withPointerParameter("zone", zone).
		withUnsignedIntParameter("mode", ClimateControlEquipment::heat);

	zone->set_heat_set_point(70);

	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", NULL);
	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", NULL);
	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", NULL);
	delete zone;
}

TEST(Zone, zone_requests_heat_when_two_sensors_below_heat_set_point_and_one_above_cool_point)
{
	Zone *zone = new Zone(2, "Basement");
	ClimateControlCoordinator coordinator(NULL);

	zone->set_climate_control_coordinator(&coordinator);

	Sensor sensor0(3, "Family Room");
	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", zone);
	zone->add_sensor(&sensor0);

	Sensor sensor1(4, "Bedroom");
	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", zone);
	zone->add_sensor(&sensor1);

	Sensor sensor2(5, "Bedroom");
	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", zone);
	zone->add_sensor(&sensor2);

	mock().expectOneCall("Sensor::get_temperature").
		andReturnValue((double)68);
	mock().expectOneCall("Sensor::get_humidity").
		andReturnValue((double)45);

	mock().expectOneCall("Sensor::get_temperature").
		andReturnValue((double)67);
	mock().expectOneCall("Sensor::get_humidity").
		andReturnValue((double)45);

	mock().expectOneCall("Sensor::get_temperature").
		andReturnValue((double)85);
	mock().expectOneCall("Sensor::get_humidity").
		andReturnValue((double)45);

	mock().expectOneCall("ClimateControlCoordinator::request_mode").
		withPointerParameter("zone", zone).
		withUnsignedIntParameter("mode", ClimateControlEquipment::heat);

	zone->set_heat_set_point(70);

	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", NULL);
	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", NULL);
	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", NULL);
	delete zone;
}

TEST(Zone, zone_requests_fan_only_when_one_sensor_below_heat_set_point_and_one_above_cool_point)
{
	Zone *zone = new Zone(2, "Basement");
	ClimateControlCoordinator coordinator(NULL);

	zone->set_climate_control_coordinator(&coordinator);

	Sensor sensor0(3, "Family Room");
	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", zone);
	zone->add_sensor(&sensor0);

	Sensor sensor1(4, "Bedroom");
	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", zone);
	zone->add_sensor(&sensor1);

	mock().expectOneCall("Sensor::get_temperature").
		andReturnValue((double)68);
	mock().expectOneCall("Sensor::get_humidity").
		andReturnValue((double)45);

	mock().expectOneCall("Sensor::get_temperature").
		andReturnValue((double)85);
	mock().expectOneCall("Sensor::get_humidity").
		andReturnValue((double)45);

	mock().expectOneCall("ClimateControlCoordinator::request_mode").
		withPointerParameter("zone", zone).
		withUnsignedIntParameter("mode", ClimateControlEquipment::fan_only);

	zone->set_heat_set_point(70);

	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", NULL);
	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", NULL);
	delete zone;
}

TEST(Zone, zone_requests_off_when_all_sensors_within_set_points)
{
	Zone *zone = new Zone(2, "Basement");
	ClimateControlCoordinator coordinator(NULL);

	zone->set_climate_control_coordinator(&coordinator);

	Sensor sensor0(3, "Family Room");
	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", zone);
	zone->add_sensor(&sensor0);

	Sensor sensor1(4, "Bedroom");
	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", zone);
	zone->add_sensor(&sensor1);

	Sensor sensor2(5, "Bedroom");
	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", zone);
	zone->add_sensor(&sensor2);

	mock().expectOneCall("Sensor::get_temperature").
		andReturnValue((double)71);
	mock().expectOneCall("Sensor::get_humidity").
		andReturnValue((double)45);

	mock().expectOneCall("Sensor::get_temperature").
		andReturnValue((double)71);
	mock().expectOneCall("Sensor::get_humidity").
		andReturnValue((double)45);

	mock().expectOneCall("Sensor::get_temperature").
		andReturnValue((double)71);
	mock().expectOneCall("Sensor::get_humidity").
		andReturnValue((double)45);

	mock().expectOneCall("ClimateControlCoordinator::request_mode").
		withPointerParameter("zone", zone).
		withUnsignedIntParameter("mode", ClimateControlEquipment::off);

	zone->set_heat_set_point(70);

	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", NULL);
	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", NULL);
	mock().expectOneCall("Sensor::set_zone").
		withPointerParameter("zone", NULL);
	delete zone;
}


int main(int ac, char** av)
{
	return CommandLineTestRunner::RunAllTests(ac, av);
}