#include <stdlib.h>
#include "qmk_rc.h"

#ifdef QMK_RC_DEFAULT_COMMANDS_ENABLE
#  include <string.h>
#  ifdef OLED_DRIVER_ENABLE
#    include "oled_driver.h"
#  endif
#  ifdef RGBLIGHT_ENABLE
#    include "rgblight.h"
#  endif
#endif

__attribute__((weak)) void qmk_rc_process_command_user(qmk_rc_command_t* command) { }


enum qmk_rc_commands_quantum {
  RESERVED = 0,
#ifdef QMK_RC_DEFAULT_COMMANDS_ENABLE
  OLED_OFF,
  OLED_ON,
  // OLED_WRITE accepts a string to write to the OLED display.
  OLED_WRITE,
  OLED_CLEAR,

  // RGBLIGHT_SETRGB_RANGE accepts 4 data bytes:
  // R - G - B - I0 - I1
  // * RGB - the color
  // * I0 - the LED index start
  // * I1 - the LED index end
  RGBLIGHT_SETRGB_RANGE,
#endif
};

#ifdef QMK_RC_DEFAULT_COMMANDS_ENABLE
void qmk_rc_process_command_quantum(qmk_rc_command_t* command) {
  char* null_terminated_data;

  switch (command->id) {
#ifdef OLED_DRIVER_ENABLE
    case OLED_OFF: oled_off(); break;
    case OLED_ON: oled_on(); break;
    case OLED_WRITE:
      null_terminated_data = malloc(command->data_length + 1);
      memcpy(null_terminated_data, command->data, command->data_length);
      null_terminated_data[command->data_length] = '\0';
      oled_write(null_terminated_data, false);
      free(null_terminated_data);
      break;
    case OLED_CLEAR: oled_clear(); break;
#endif

#ifdef RGBLIGHT_ENABLE
    case RGBLIGHT_SETRGB_RANGE:
      rgblight_setrgb_range(
        command->data[0], // R
        command->data[1], // G
        command->data[2], // B
        command->data[3], // diode index start
        command->data[4]  // diode index end
      );
      break;
#endif
    default:
      qmk_rc_process_command_user(command);
  }
}
#else
#  define qmk_rc_process_command_quantum qmk_rc_process_command_user
#endif

qmk_rc_parser_t parser = {
  .state = RECEIVING_COMMAND,
  .data_bytes_read = 0,
};

qmk_rc_command_t command = {
  .id = UINT8_MAX,
  .data_length = 0,
  .data = NULL,
};

void qmk_rc_receive(uint8_t *buffer, uint16_t max_buffer_length, uint8_t* data, uint8_t data_length) {
  for (int i = 0; i < data_length; i++) {
    if (parser.state == RECEIVING_COMMAND) {
      // Raw HID packets are padded to `RAW_EPSIZE`, 32 or 64 bytes. Since this
      // is an afterthought for this parser, we reserve the 0 command code as
      // a "skip" instruction in receiving command mode. In the sender we then
      // universally pad to 64 bytes with 0s.
      if (data[i] == 0) continue;

      command.id = data[i];
      parser.state = RECEIVING_DATA_LENGTH;

      parser.data_length_bytes_read = 0;
      parser.data_length.data_length = 0;
    }
    else if (parser.state == RECEIVING_DATA_LENGTH) {
      parser.data_length.raw[parser.data_length_bytes_read++] = data[i];

      if (parser.data_length_bytes_read == sizeof(qmk_rc_data_length_t)) {
        command.data_length = parser.data_length.data_length;

        command.data = NULL;

        if (command.data_length == 0) {
          qmk_rc_process_command_quantum(&command);
          parser.state = RECEIVING_COMMAND;
        }
        else {
          parser.state = RECEIVING_DATA;
          parser.data_bytes_read = 0;
        }
      }
    }
    else if (parser.state == RECEIVING_DATA) {
      buffer[parser.data_bytes_read++] = data[i];
      if (parser.data_bytes_read == command.data_length) {
        command.data = buffer;
        qmk_rc_process_command_quantum(&command);
        parser.state = RECEIVING_COMMAND;
      }
    }
  }
}
