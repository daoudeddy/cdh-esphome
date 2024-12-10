import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    UNIT_CELSIUS,
    UNIT_VOLT,
    UNIT_AMPERE,
    UNIT_HERTZ,
    UNIT_EMPTY,
    UNIT_REVOLUTIONS_PER_MINUTE,
    ICON_THERMOMETER,
    ICON_CURRENT_AC,
    ICON_FLASH,
    ICON_FAN,
    ICON_EMPTY,
)

from . import heater_uart_ns, HeaterUart

DEPENDENCIES = ["heater_uart"]

# Define all sensors with their unit, icon, and device class
SENSORS = {
    "current_temperature": ("Current Temperature", UNIT_CELSIUS, ICON_THERMOMETER),
    "fan_speed": ("Fan Speed", UNIT_REVOLUTIONS_PER_MINUTE, ICON_FAN),
    "supply_voltage": ("Supply Voltage", UNIT_VOLT, ICON_FLASH),
    "heat_exchanger_temp": ("Heat Exchanger Temperature", UNIT_CELSIUS, ICON_THERMOMETER),
    "glow_plug_voltage": ("Glow Plug Voltage", UNIT_VOLT, ICON_FLASH),
    "glow_plug_current": ("Glow Plug Current", UNIT_AMPERE, ICON_CURRENT_AC),
    "pump_frequency": ("Pump Frequency", UNIT_HERTZ, ICON_EMPTY),
    "fan_voltage": ("Fan Voltage", UNIT_VOLT, ICON_FLASH),
    "desired_temperature": ("Desired Temperature", UNIT_CELSIUS, ICON_THERMOMETER),
}

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.use_id(HeaterUart),
        **{
            cv.Optional(sensor_key): sensor.sensor_schema(
                unit_of_measurement=unit,
                icon=icon,
                accuracy_decimals=1,
            )
            for sensor_key, (_, unit, icon) in SENSORS.items()
        },
    }
)

async def to_code(config):
    # Resolve the HeaterUart parent component
    parent = await cg.get_variable(config[cv.GenerateID()])

    # Loop through all sensors in the schema and register them
    for sensor_key, (name, _, _) in SENSORS.items():
        if sensor_key in config:
            sens = await sensor.new_sensor(config[sensor_key])
            cg.add(parent.set_sensor(sensor_key, sens))
