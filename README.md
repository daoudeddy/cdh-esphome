# Heater UART ESPHome Component

## Overview
`heater_uart` is a custom ESPHome component designed to interface with heaters that use a UART protocol for communication. It parses data transmitted between the heater controller and the heater, providing real-time monitoring and control over various parameters such as temperature, fan speed, voltage, and more.

This component supports mapping of heater states and error codes into human-readable formats, making it ideal for integration with smart home systems like Home Assistant.

---

## Features

- **Real-Time Monitoring**:
  - Set Temperature
  - Fan Speed (RPM)
  - Supply Voltage (V)
  - Heat Exchanger Temperature (°C)
  - Glow Plug Voltage (V)
  - Glow Plug Current (A)
  - Pump Frequency (Hz)
  - Fan Voltage (V)
  - Heater Run State
  - Error Codes
- **Human-Readable Text Sensors**:
  - Run State Description
  - Error Code Description
- **Binary Sensors**:
  - Heater On/Off State
- **Supports Mapping**:
  - Run states and error codes mapped to meaningful strings.

---

## Requirements

### Hardware
- ESP32/ESP8266 microcontroller.
- Heater supporting UART communication at `25000 baud`.

### Software
- ESPHome >= 2023.x.x.
- Home Assistant (optional).

---

## Installation

### Add the Component to Your ESPHome Project
Add the external component to your ESPHome configuration:

   ```yaml
   external_components:
     - source: github://daoudeddy/cdh-esphome@main
       components: [ heater_uart ]
   ```

### Define UART Settings
Configure the UART interface for the heater:

```yaml
uart:
  id: uart_bus
  rx_pin: GPIOXX
  baud_rate: 25000
```

### Example YAML Configuration
Add sensors, binary sensors, and text sensors to monitor the heater:

```yaml
heater_uart:
  current_temp:
    name: "Current Temperature"
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
  run_state_text:
    name: "Run State"
  error_code_text:
    name: "Error Code"
  on_off_state:
    name: "Heater On/Off"
```

---

## Usage

### Sensors and Binary Sensors
Each sensor corresponds to a specific parameter of the heater. These values are updated in real time and can be used in Home Assistant automations or dashboards.

### Example Automations
Turn on a fan if the heater's supply voltage drops below 12V:
```yaml
automation:
  - alias: "Low Voltage Fan Trigger"
    trigger:
      - platform: numeric_state
        entity_id: sensor.supply_voltage
        below: 12
    action:
      - service: switch.turn_on
        target:
          entity_id: switch.fan
```

---

## Development

### File Structure
```
heater_uart/
├── __init__.py           # ESPHome configuration schema
├── heater_uart.h         # Component header
├── heater_uart.cpp       # Component implementation
├── heater_uart.json      # Component metadata
```

### Contributing
1. Fork the repository.
2. Create a new feature branch.
3. Commit your changes.
4. Open a pull request.

---

## Acknowledgments
- Inspired by various ESPHome UART integrations.
- Special thanks to the ESPHome community for their extensive documentation and support.
