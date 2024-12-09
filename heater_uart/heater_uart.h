#pragma once
#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include <map>
#include <string>

namespace esphome {
namespace heater_uart {

class HeaterUart : public PollingComponent, public uart::UARTDevice {
 public:
  HeaterUart() = default;

  void setup() override;
  void loop() override;
  void update() override;

  // Assign sensors
  void set_current_temp_sensor(sensor::Sensor *sensor) { current_temp_ = sensor; }
  void set_fan_speed_sensor(sensor::Sensor *sensor) { fan_speed_ = sensor; }
  void set_supply_voltage_sensor(sensor::Sensor *sensor) { supply_voltage_ = sensor; }
  void set_heat_exchanger_temp_sensor(sensor::Sensor *sensor) { heat_exchanger_temp_ = sensor; }
  void set_glow_plug_voltage_sensor(sensor::Sensor *sensor) { glow_plug_voltage_ = sensor; }
  void set_glow_plug_current_sensor(sensor::Sensor *sensor) { glow_plug_current_ = sensor; }
  void set_pump_frequency_sensor(sensor::Sensor *sensor) { pump_frequency_ = sensor; }
  void set_desired_temp_sensor(sensor::Sensor *sensor) { desired_temp_ = sensor; }
  void set_fan_voltage_sensor(sensor::Sensor *sensor) { fan_voltage_ = sensor; }
  void set_on_off_state_sensor(binary_sensor::BinarySensor *sensor) { on_off_state_ = sensor; }
  void set_run_state_text_sensor(text_sensor::TextSensor *sensor) { run_state_text_ = sensor; }
  void set_error_code_text_sensor(text_sensor::TextSensor *sensor) { error_code_text_ = sensor; }
  
 private:
  void parse_frame(const uint8_t *frame, size_t length);
  void reset_frame();

  uint8_t frame_[48];
  int frame_index_ = 0;
  bool waiting_for_start_ = true;

  // Sensors
  sensor::Sensor *current_temp_ = nullptr;
  sensor::Sensor *fan_speed_ = nullptr;
  sensor::Sensor *supply_voltage_ = nullptr;
  sensor::Sensor *heat_exchanger_temp_ = nullptr;
  sensor::Sensor *glow_plug_voltage_ = nullptr;
  sensor::Sensor *glow_plug_current_ = nullptr;
  sensor::Sensor *pump_frequency_ = nullptr;
  sensor::Sensor *desired_temp_ = nullptr;
  sensor::Sensor *fan_voltage_ = nullptr;
  binary_sensor::BinarySensor *on_off_state_ = nullptr;
  text_sensor::TextSensor *run_state_text_ = nullptr;
  text_sensor::TextSensor *error_code_text_ = nullptr;
  
  float current_temperature_value_ = 0;
  int fan_speed_value_ = 0;
  float measured_voltage_ = 0;
  float desired_temp_value_ = 0;
  float heat_exchanger_temp_value_ = 0;
  float glow_plug_voltage_value_ = 0;
  float glow_plug_current_value_ = 0;
  float pump_frequency_value_ = 0;
  int error_code_value_ = 0;
  int run_state_value_ = 0;
  int on_off_value_ = 0;
  float fan_voltage_value_ = 0;
  std::string run_state_description_ = "Unknown";
  std::string error_code_description_ = "Unknown";
  const std::map<int, std::string> run_state_map = {
        {0, "Off / Standby"}, {1, "Start Acknowledge"}, {2, "Glow plug pre-heat"},
        {3, "Failed ignition - pausing for retry"}, {4, "Ignited – heating to full temp phase"},
        {5, "Running"}, {6, "Skipped – stop acknowledge?"},
        {7, "Stopping - Post run glow re-heat"}, {8, "Cooldown"}
  };
  const std::map<int, std::string> error_code_map = {
        {0, "No Error"}, {1, "No Error, but started"}, {2, "Voltage too low"},
        {3, "Voltage too high"}, {4, "Ignition plug failure"},
        {5, "Pump Failure – over current"}, {6, "Too hot"}, {7, "Motor Failure"},
        {8, "Serial connection lost"}, {9, "Fire is extinguished"}, {10, "Temperature sensor failure"}
  };
};

}  // namespace heater_uart
}  // namespace esphome