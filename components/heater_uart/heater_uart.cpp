#include "heater_uart.h"
#include "esphome/core/log.h"

namespace esphome {
namespace heater_uart {

static const char *TAG = "heater_uart";
        
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
                frame[frame_index_++] = byte;
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
  if (desired_temp_) desired_temp_->publish_state(desired_temp_value_);
  if (current_temp_) current_temp_->publish_state(current_temperature_value_);
  if (fan_speed_) fan_speed_->publish_state(fan_speed_value_);
  if (supply_voltage_) supply_voltage_->publish_state(measured_voltage_);
  if (heat_exchanger_temp_) heat_exchanger_temp_->publish_state(heat_exchanger_temp_value_);
  if (glow_plug_voltage_) glow_plug_voltage_->publish_state(glow_plug_voltage_value_);
  if (glow_plug_current_) glow_plug_current_->publish_state(glow_plug_current_value_);
  if (pump_frequency_) pump_frequency_->publish_state(pump_frequency_value_);
  if (fan_voltage_) fan_voltage_->publish_state(fan_voltage_value_);
  if (on_off_state_) on_off_state_->publish_state(on_off_value_);
  if (run_state_text_) run_state_text_->publish_state(run_state_description_);
  if (error_code_text_)  error_code_text_->publish_state(error_code_description_);
}

void HeaterUart::parse_frame(const uint8_t *frame, size_t length) {
  if (length != 48) {
    ESP_LOGW("heater_uart", "Invalid frame length: %d bytes (expected 48)", length);
    return;
  }

  // Split the frame into command and response frames
  const uint8_t *command_frame = &frame[0];     // First 24 bytes
  const uint8_t *response_frame = &frame[24];   // Last 24 bytes

  // Parse the command frame
  current_temperature_value_ = command_frame[3];    // Set temperature (index 3)
  desired_temp_value_ = command_frame[4];           // Desired temperature (index 4)

  // Parse the response frame
  fan_speed_value_ = (response_frame[6] << 8) | response_frame[7];                      // Fan speed (16-bit value)
  measured_voltage_ = ((response_frame[4] << 8) | response_frame[5]) * 0.1;             // Supply voltage (index 4-5)
  heat_exchanger_temp_value_ = ((response_frame[10] << 8) | response_frame[11]);        // Heat exchanger temperature
  glow_plug_voltage_value_ = ((response_frame[12] << 8) | response_frame[13]) * 0.1;    // Glow plug voltage
  glow_plug_current_value_ = ((response_frame[14] << 8) | response_frame[15]) * 0.01;   // Glow plug current
  pump_frequency_value_ = response_frame[16] * 0.1;                                     // Pump frequency
  error_code_value_ = response_frame[17];                                               // Error code
  run_state_value_ = response_frame[2];                                                 // Run state
  on_off_value_ = response_frame[3] == 1;                                               // On/Off state
  fan_voltage_value_ = ((response_frame[8] << 8) | response_frame[9]) * 0.1;            // Fan voltage

  // Map the run state and error code to human-readable strings
  if (run_state_map.find(run_state_value_) != run_state_map.end()) {
    run_state_description_ = run_state_map.at(run_state_value_);
  } else {
    run_state_description_ = "Unknown Run State";
  }

  if (error_code_map.find(error_code_value_) != error_code_map.end()) {
    error_code_description_ = error_code_map.at(error_code_value_);
  } else {
    error_code_description_ = "Unknown Error Code";
  }
}

void HeaterUart::reset_frame() {
  frame_index_ = 0;
  waiting_for_start_ = true;
}

}  // namespace heater_uart
}  // namespace esphome