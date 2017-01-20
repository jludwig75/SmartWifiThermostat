#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTestExt/MockSupport.h>


#include "Sensor.h"
#include "Zone.h"


Zone::Zone(uint32_t id, const char *name)
{

}

Zone::~Zone()
{

}

void Zone::notify_sensor_conditions_changed()
{
	mock().actualCall("Zone::notify_sensor_conditions_changed");
}

TEST_GROUP(Sensor)
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

TEST(Sensor, condition_updates_work)
{
	Sensor sensor(7, "Living Room");

	CHECK_EQUAL(7, sensor.get_id());
	CHECK(strcmp("Living Room", sensor.get_name()) == 0);

	sensor.update_temperature((float)99.6);
	CHECK_EQUAL((float)99.6, sensor.get_temperature());

	sensor.update_humidity((float)55.1);
	CHECK_EQUAL((float)55.1, sensor.get_humidity());
}

TEST(Sensor, zone_notified_of_significant_changes)
{
	Sensor sensor(7, "Living Room");

	CHECK_EQUAL(7, sensor.get_id());
	CHECK(strcmp("Living Room", sensor.get_name()) == 0);

	sensor.update_temperature((float)99.6);
	CHECK_EQUAL((float)99.6, sensor.get_temperature());

	sensor.update_humidity((float)55.1);
	CHECK_EQUAL((float)55.1, sensor.get_humidity());

	Zone zone(0, "Upstairs");
	sensor.set_zone(&zone);

	// Make a significant change to the temperature.
	mock().expectOneCall("Zone::notify_sensor_conditions_changed");

	sensor.update_temperature((float)33.7);
	CHECK_EQUAL((float)33.7, sensor.get_temperature());

	// Make a significant change to the humidity.
	mock().expectOneCall("Zone::notify_sensor_conditions_changed");

	sensor.update_humidity((float)91.3);
	CHECK_EQUAL((float)91.3, sensor.get_humidity());
}

TEST(Sensor, zone_not_notified_of_insignificant_changes)
{
	Sensor sensor(7, "Living Room");

	CHECK_EQUAL(7, sensor.get_id());
	CHECK(strcmp("Living Room", sensor.get_name()) == 0);

	sensor.update_temperature((float)99.6);
	CHECK_EQUAL((float)99.6, sensor.get_temperature());

	sensor.update_humidity((float)55.1);
	CHECK_EQUAL((float)55.1, sensor.get_humidity());

	Zone zone(0, "Upstairs");
	sensor.set_zone(&zone);

	// Make a significant change to the temperature.
	mock().expectNCalls(0, "Zone::notify_sensor_conditions_changed");

	sensor.update_temperature((float)99.4);	// Difference of 0.2 degrees
	CHECK_EQUAL((float)99.4, sensor.get_temperature());

	// Make a significant change to the humidity.
	mock().expectNCalls(0, "Zone::notify_sensor_conditions_changed");

	sensor.update_humidity((float)54.9);	// Difference of 0.2 percent
	CHECK_EQUAL((float)54.9, sensor.get_humidity());
}



int main(int ac, char** av)
{
	return CommandLineTestRunner::RunAllTests(ac, av);
}