#include <stdlib.h>
#include "qmk_rc.h"

__attribute__((weak)) void qmk_rc_process_command_user(qmk_rc_command_t* command) { }

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
      command.id = data[i];
      parser.state = RECEIVING_DATA_LENGTH;

      parser.data_length_bytes_read = 0;
      parser.data_length.data_length = 0;
      continue;
    }

    if (parser.state == RECEIVING_DATA_LENGTH) {
      parser.data_length.raw[parser.data_length_bytes_read++] = data[i];

      if (parser.data_length_bytes_read == sizeof(qmk_rc_data_length_t)) {
        command.data_length = parser.data_length.data_length;

        command.data = NULL;

        if (command.data_length == 0) {
          qmk_rc_process_command_user(&command);
          parser.state = RECEIVING_COMMAND;
          continue;
        }
        else {
          parser.state = RECEIVING_DATA;
          parser.data_bytes_read = 0;
        }
      }

      continue;
    }

    if (parser.state == RECEIVING_DATA) {
      buffer[parser.data_bytes_read++] = data[i];
      if (parser.data_bytes_read == command.data_length) {
        command.data = buffer;
        qmk_rc_process_command_user(&command);
        parser.state = RECEIVING_COMMAND;
      }
      continue;
    }
  }
}
