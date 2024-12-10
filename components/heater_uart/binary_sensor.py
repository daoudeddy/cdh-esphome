import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import DEVICE_CLASS_POWER

from . import heater_uart_ns, HeaterUart

DEPENDENCIES = ["heater_uart"]

BINARY_SENSORS = {
    "on_off_state": ("On/Off State", DEVICE_CLASS_POWER),
}

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.use_id(HeaterUart),
        **{
            cv.Optional(binary_sensor_key): binary_sensor.binary_sensor_schema(
                device_class=device_class
            )
            for binary_sensor_key, (name, device_class) in BINARY_SENSORS.items()
        },
    }
)

async def to_code(config):
    parent = await cg.get_variable(config[cv.GenerateID()])
    for binary_sensor_key, (name, _) in BINARY_SENSORS.items():
        if binary_sensor_key in config:
            bin_sens = await binary_sensor.new_binary_sensor(config[binary_sensor_key])
            cg.add(parent.set_binary_sensor(binary_sensor_key, bin_sens))
