#include "heater_uart.h"
#include "esphome/core/log.h"

namespace esphome {
namespace heater_uart {

static const char *TAG = "heater_uart";

// Mappings for error and run states
const std::map<int, std::string> HeaterUart::run_state_map = {
    {0, "Off / Standby"}, {1, "Start Acknowledge"}, {2, "Glow Plug Pre-heat"},
    {3, "Failed Ignition - Pause for Retry"}, {4, "Ignited – Heating to Full Temp"},
    {5, "Running"}, {6, "Stop Acknowledge"}, {7, "Stopping - Post Run Glow Re-heat"},
    {8, "Cooldown"}
};

const std::map<int, std::string> HeaterUart::error_code_map = {
    {0, "No Error"}, {1, "No Error, But Started"}, {2, "Voltage Too Low"},
    {3, "Voltage Too High"}, {4, "Ignition Plug Failure"}, {5, "Pump Failure – Over Current"},
    {6, "Too Hot"}, {7, "Motor Failure"}, {8, "Serial Connection Lost"},
    {9, "Fire Extinguished"}, {10, "Temperature Sensor Failure"}
};

void HeaterUart::setup() {
    ESP_LOGCONFIG(TAG, "Setting up Heater UART...");
}

void HeaterUart::loop() {
    const int FRAME_SIZE = 48;
    const int TX_FRAME_END_INDEX = 23;
    const int RX_FRAME_START_INDEX = 24;
    const uint8_t END_OF_FRAME_MARKER = 0x00;

    while (available()) {
        uint8_t byte = read();
        if (waiting_for_start_) {
            if (byte == 0x76) {
                frame_[frame_index_++] = byte;
                waiting_for_start_ = false;
            }
        } else {
            frame_[frame_index_++] = byte;
            if (frame_index_ == TX_FRAME_END_INDEX + 1) {
                if (frame_[21] == END_OF_FRAME_MARKER) {
                    ESP_LOGW("heater_uart", "Invalid Transmit Packet. Resetting frame.");
                    reset_frame();
                    return;
                }
            }
            if (frame_index_ == FRAME_SIZE) {
                if (frame_[45] == END_OF_FRAME_MARKER && frame_[RX_FRAME_START_INDEX] == 0x76) {
                    parse_frame(frame_, FRAME_SIZE);
                } else {
                    ESP_LOGW("heater_uart", "Invalid Receive Packet or incorrect order. Resetting frame.");
                }
                reset_frame();
            }
        }
    }
}

void HeaterUart::update() {
    for (const auto &sensor_entry : sensors_) {
        const std::string &key = sensor_entry.first;
        sensor::Sensor *sensor = sensor_entry.second;

        if (key == "current_temperature")
            sensor->publish_state(current_temperature_value_);
        else if (key == "fan_speed")
            sensor->publish_state(fan_speed_value_);
        else if (key == "supply_voltage")
            sensor->publish_state(supply_voltage_value_);
        else if (key == "heat_exchanger_temp")
            sensor->publish_state(heat_exchanger_temp_value_);
        else if (key == "glow_plug_voltage")
            sensor->publish_state(glow_plug_voltage_value_);
        else if (key == "glow_plug_current")
            sensor->publish_state(glow_plug_current_value_);
        else if (key == "pump_frequency")
            sensor->publish_state(pump_frequency_value_);
        else if (key == "fan_voltage")
            sensor->publish_state(fan_voltage_value_);
        else if (key == "desired_temperature")
            sensor->publish_state(desired_temperature_value_);
    }

    for (const auto &text_entry : text_sensors_) {
        const std::string &key = text_entry.first;
        text_sensor::TextSensor *text_sensor = text_entry.second;

        if (key == "run_state")
            text_sensor->publish_state(run_state_description_);
        else if (key == "error_code")
            text_sensor->publish_state(error_code_description_);
    }

    for (const auto &binary_entry : binary_sensors_) {
        const std::string &key = binary_entry.first;
        binary_sensor::BinarySensor *binary_sensor = binary_entry.second;

        if (key == "on_off_state")
            binary_sensor->publish_state(on_off_value_);
    }
}

void HeaterUart::parse_frame(const uint8_t *frame, size_t length) {
    if (length != 48) {
        ESP_LOGW(TAG, "Invalid frame length: %d bytes (expected 48)", length);
        return;
    }

    const uint8_t *command_frame = &frame[0];
    const uint8_t *response_frame = &frame[24];

    current_temperature_value_ = command_frame[3];
    desired_temperature_value_ = command_frame[4];
    fan_speed_value_ = (response_frame[6] << 8) | response_frame[7];
    supply_voltage_value_ = ((response_frame[4] << 8) | response_frame[5]) * 0.1;
    heat_exchanger_temp_value_ = ((response_frame[10] << 8) | response_frame[11]);
    glow_plug_voltage_value_ = ((response_frame[12] << 8) | response_frame[13]) * 0.1;
    glow_plug_current_value_ = ((response_frame[14] << 8) | response_frame[15]) * 0.01;
    pump_frequency_value_ = response_frame[16] * 0.1;
    fan_voltage_value_ = ((response_frame[8] << 8) | response_frame[9]) * 0.1;
    run_state_value_ = response_frame[2];
    on_off_value_ = response_frame[3] == 1;
    error_code_value_ = response_frame[17];

    run_state_description_ = run_state_map.count(run_state_value_)
                                 ? run_state_map.at(run_state_value_)
                                 : "Unknown Run State";

    error_code_description_ = error_code_map.count(error_code_value_)
                                  ? error_code_map.at(error_code_value_)
                                  : "Unknown Error Code";
}

void HeaterUart::reset_frame() {
    frame_index_ = 0;
    waiting_for_start_ = true;
}

void HeaterUart::set_sensor(const std::string &key, sensor::Sensor *sensor) {
    sensors_[key] = sensor;
}

void HeaterUart::set_text_sensor(const std::string &key, text_sensor::TextSensor *text_sensor) {
    text_sensors_[key] = text_sensor;
}

void HeaterUart::set_binary_sensor(const std::string &key, binary_sensor::BinarySensor *binary_sensor) {
    binary_sensors_[key] = binary_sensor;
}

}  // namespace heater_uart
}  // namespace esphome
