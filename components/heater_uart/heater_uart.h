#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

#include <map>
#include <string>

namespace esphome {
namespace heater_uart {

class HeaterUart : public PollingComponent, public uart::UARTDevice {
 public:
  HeaterUart() = default;

  // Setters for sensors
  void set_sensor(const std::string &key, sensor::Sensor *sensor);
  void set_text_sensor(const std::string &key, text_sensor::TextSensor *text_sensor);
  void set_binary_sensor(const std::string &key, binary_sensor::BinarySensor *binary_sensor);

  void setup() override;
  void loop() override;
  void update() override;

 protected:
  // Sensor storage
  std::map<std::string, sensor::Sensor *> sensors_;
  std::map<std::string, text_sensor::TextSensor *> text_sensors_;
  std::map<std::string, binary_sensor::BinarySensor *> binary_sensors_;

  // Frame handling
  uint8_t frame_[48];
  int frame_index_ = 0;
  bool waiting_for_start_ = true;

  // Parsed data
  float current_temperature_value_ = 0;
  int fan_speed_value_ = 0;
  float supply_voltage_value_ = 0;
  float heat_exchanger_temp_value_ = 0;
  float glow_plug_voltage_value_ = 0;
  float glow_plug_current_value_ = 0;
  float pump_frequency_value_ = 0;
  float fan_voltage_value_ = 0;
  int desired_temperature_value_ = 0;
  int error_code_value_ = 0;
  int run_state_value_ = 0;
  bool on_off_value_ = false;

  std::string run_state_description_ = "Unknown";
  std::string error_code_description_ = "Unknown";

  void parse_frame(const uint8_t *frame, size_t length);
  void reset_frame();

  // Mappings for error and run states
  static const std::map<int, std::string> run_state_map;
  static const std::map<int, std::string> error_code_map;
};

}  // namespace heater_uart
}  // namespace esphome
