import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor, text_sensor, binary_sensor
from esphome.const import (
    UNIT_CELSIUS,
    UNIT_VOLT,
    UNIT_AMPERE,
    UNIT_REVOLUTIONS_PER_MINUTE ,
    UNIT_HERTZ,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_VOLTAGE,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_POWER,
    ICON_THERMOMETER,
    ICON_FAN,
    ICON_FLASH,
    ICON_EMPTY,
)

heater_uart_ns = cg.esphome_ns.namespace("heater_uart")
HeaterUart = heater_uart_ns.class_("HeaterUart", cg.PollingComponent, uart.UARTDevice)

DEPENDENCIES = ["uart"]

CONF_CURRENT_TEMP = "current_temp"
CONF_FAN_SPEED = "fan_speed"
CONF_SUPPLY_VOLTAGE = "supply_voltage"
CONF_HEAT_EXCHANGER_TEMP = "heat_exchanger_temp"
CONF_GLOW_PLUG_VOLTAGE = "glow_plug_voltage"
CONF_GLOW_PLUG_CURRENT = "glow_plug_current"
CONF_PUMP_FREQUENCY = "pump_frequency"
CONF_DESIRED_TEMP = "desired_temp"
CONF_FAN_VOLTAGE = "fan_voltage"
CONF_RUN_STATE_TEXT = "run_state_text"
CONF_ERROR_CODE_TEXT = "error_code_text"
CONF_ON_OFF_STATE = "on_off_state"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(HeaterUart),
        cv.Optional(CONF_CURRENT_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            device_class=DEVICE_CLASS_TEMPERATURE,
        ),
        cv.Optional(CONF_FAN_SPEED): sensor.sensor_schema(
            unit_of_measurement=UNIT_REVOLUTIONS_PER_MINUTE ,
            icon=ICON_FAN,
        ),
        cv.Optional(CONF_SUPPLY_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            device_class=DEVICE_CLASS_VOLTAGE,
        ),
        cv.Optional(CONF_HEAT_EXCHANGER_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            device_class=DEVICE_CLASS_TEMPERATURE,
        ),
        cv.Optional(CONF_GLOW_PLUG_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            device_class=DEVICE_CLASS_VOLTAGE,
        ),
        cv.Optional(CONF_GLOW_PLUG_CURRENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            icon=ICON_EMPTY,
            device_class=DEVICE_CLASS_CURRENT,
        ),
        cv.Optional(CONF_PUMP_FREQUENCY): sensor.sensor_schema(
            unit_of_measurement=UNIT_HERTZ,
            icon=ICON_EMPTY,
        ),
        cv.Optional(CONF_DESIRED_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            device_class=DEVICE_CLASS_TEMPERATURE,
        ),
        cv.Optional(CONF_FAN_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FAN,
            device_class=DEVICE_CLASS_VOLTAGE,
        ),
        cv.Optional(CONF_RUN_STATE_TEXT): text_sensor.text_sensor_schema(),
        cv.Optional(CONF_ERROR_CODE_TEXT): text_sensor.text_sensor_schema(),
        cv.Optional(CONF_ON_OFF_STATE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_POWER,
        ),
    }
).extend(cv.polling_component_schema("5s")).extend(uart.UART_DEVICE_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[cv.GenerateID()])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_CURRENT_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_CURRENT_TEMP])
        cg.add(var.set_current_temp_sensor(sens))

    if CONF_FAN_SPEED in config:
        sens = await sensor.new_sensor(config[CONF_FAN_SPEED])
        cg.add(var.set_fan_speed_sensor(sens))

    if CONF_SUPPLY_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_SUPPLY_VOLTAGE])
        cg.add(var.set_supply_voltage_sensor(sens))

    if CONF_HEAT_EXCHANGER_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_HEAT_EXCHANGER_TEMP])
        cg.add(var.set_heat_exchanger_temp_sensor(sens))

    if CONF_GLOW_PLUG_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_GLOW_PLUG_VOLTAGE])
        cg.add(var.set_glow_plug_voltage_sensor(sens))

    if CONF_GLOW_PLUG_CURRENT in config:
        sens = await sensor.new_sensor(config[CONF_GLOW_PLUG_CURRENT])
        cg.add(var.set_glow_plug_current_sensor(sens))

    if CONF_PUMP_FREQUENCY in config:
        sens = await sensor.new_sensor(config[CONF_PUMP_FREQUENCY])
        cg.add(var.set_pump_frequency_sensor(sens))

    if CONF_DESIRED_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_DESIRED_TEMP])
        cg.add(var.set_desired_temp_sensor(sens))

    if CONF_FAN_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_FAN_VOLTAGE])
        cg.add(var.set_fan_voltage_sensor(sens))

    if CONF_RUN_STATE_TEXT in config:
        text_sens = await text_sensor.new_text_sensor(config[CONF_RUN_STATE_TEXT])
        cg.add(var.set_run_state_text_sensor(text_sens))

    if CONF_ERROR_CODE_TEXT in config:
        text_sens = await text_sensor.new_text_sensor(config[CONF_ERROR_CODE_TEXT])
        cg.add(var.set_error_code_text_sensor(text_sens))

    if CONF_ON_OFF_STATE in config:
        binary_sens = await binary_sensor.new_binary_sensor(config[CONF_ON_OFF_STATE])
        cg.add(var.set_on_off_state_sensor(binary_sens))
