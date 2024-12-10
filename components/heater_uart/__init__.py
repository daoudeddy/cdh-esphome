import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

heater_uart_ns = cg.esphome_ns.namespace("heater_uart")
HeaterUart = heater_uart_ns.class_("HeaterUart", cg.Component, uart.UARTDevice)

DEPENDENCIES = ["uart"]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(HeaterUart),
    }
).extend(uart.UART_DEVICE_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
