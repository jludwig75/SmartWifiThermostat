#pragma once


class ClimateControlEquipment
{
public:
	ClimateControlEquipment();

	enum Mode { off, fan_only, heat, cool };

	void set_mode(Mode mode);
	void turn_on_humidifier();
	void turn_off_humidifier();

	Mode get_mode() const;
	bool is_humidifier_on() const;

private:
	Mode _mode;
	bool _humidifier_on;
};
