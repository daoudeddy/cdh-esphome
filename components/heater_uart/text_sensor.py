import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import ICON_EMPTY

from . import heater_uart_ns, HeaterUart

DEPENDENCIES = ["heater_uart"]

TEXT_SENSORS = {
    "run_state": ("Run State", ICON_EMPTY),
    "error_code": ("Error Code", ICON_EMPTY),
}

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.use_id(HeaterUart),
        **{
            cv.Optional(text_sensor_key): text_sensor.text_sensor_schema(
                icon=icon
            )
            for text_sensor_key, (name, icon) in TEXT_SENSORS.items()
        },
    }
)

async def to_code(config):
    parent = await cg.get_variable(config[cv.GenerateID()])
    for text_sensor_key, (name, _) in TEXT_SENSORS.items():
        if text_sensor_key in config:
            txt_sens = await text_sensor.new_text_sensor(config[text_sensor_key])
            cg.add(parent.set_text_sensor(text_sensor_key, txt_sens))
