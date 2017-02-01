#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTestExt/MockSupport.h>

#include "Thermostat.h"
#include "ClimateControlCoordinator.h"
#include "Zone.h"


ClimateControlCoordinator::ClimateControlCoordinator(ClimateControlEquipment *equipment)
{

}

ClimateControlCoordinator::~ClimateControlCoordinator()
{

}

void ClimateControlCoordinator::add_zone(Zone *zone)
{

}

void ClimateControlCoordinator::request_mode(Zone *zone, ClimateControlEquipment::Mode mode)
{

}

void ClimateControlCoordinator::request_humidity(Zone *zone)
{

}

void ClimateControlCoordinator::request_no_humidity(Zone *zone)
{

}



int main(int ac, char** av)
{
	return CommandLineTestRunner::RunAllTests(ac, av);
}