# Heater UART ESPHome Component

This project is a custom ESPHome component designed to interface with heaters using UART communication. It provides a comprehensive integration for monitoring and controlling various aspects of heater operation, including temperature, fan speed, voltage, and error diagnostics.

## Features

- **Full UART Frame Parsing**: Processes and decodes 48-byte frames from the heater's UART communication.
- **Sensors**:
  - Current Temperature
  - Desired Temperature
  - Fan Speed
  - Supply Voltage
  - Heat Exchanger Temperature
  - Glow Plug Voltage
  - Glow Plug Current
  - Pump Frequency
  - Fan Voltage
- **Binary Sensors**:
  - On/Off State
- **Text Sensors**:
  - Run State
  - Error Code
- **Mappings for Readability**:
  - Human-readable descriptions for error codes and run states.

## Getting Started

### Installation

Update your ESPHome configuration to include this component.

### Example Configuration

```yaml
external_components:
  - source: github://daoudeddy/cdh-esphome@main
    components: [ heater_uart ]

uart:
  id: heater_uart
  rx_pin: <your_rx_pin>
  baud_rate: 25000

heater_uart:
  id: heater
  update_interval: 30s

sensor:
  - platform: heater_uart
    current_temperature:
      name: "Current Temperature"
    desired_temperature:
      name: "Desired Temperature"
    fan_speed:
      name: "Fan Speed"
    supply_voltage:
      name: "Supply Voltage"
    heat_exchanger_temp:
      name: "Heat Exchanger Temperature"
    glow_plug_voltage:
      name: "Glow Plug Voltage"
    glow_plug_current:
      name: "Glow Plug Current"
    pump_frequency:
      name: "Pump Frequency"
    fan_voltage:
      name: "Fan Voltage"

binary_sensor:
  - platform: heater_uart
    on_off_state:
      name: "On/Off State"

text_sensor:
  - platform: heater_uart
    run_state:
      name: "Run State"
    error_code:
      name: "Error Code"
```

### Notes

- Ensure the `rx_pin` match your ESP32 or ESP8266 configuration.
- The baud rate must be set to `25000` to communicate with the heater.

## Development

### Code Structure

- **`heater_uart.h`**: Header file defining the component.
- **`heater_uart.cpp`**: Core logic for parsing UART frames and managing sensors.
- **`__init__.py`**: Python module to integrate the component with ESPHome.
- **`sensor.py`**, **`text_sensor.py`**, and **`binary_sensor.py`**: Python definitions for each sensor type.

### Debugging

Enable debug logging in your ESPHome configuration to monitor the UART frames and parsed data:

```yaml
logger:
  level: DEBUG
```
