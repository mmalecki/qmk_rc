#include <stdint.h>
#include <sys/types.h>

typedef uint8_t qmk_rc_command_id_t;

// Initially, I thought we would only need one data length byte, but as it turns
// out, some Corsair keyboards have a fully addressible RGB matrix of 104 keys
// (https://twitter.com/kenperkins/status/1363869986129911820).
// Since a RGB tuple consists of bytes, that means we need 312 bytes to address
// the entire matrix at least.
// Users could, of course, apply some sorts of compression or command splitting
// to address the entire matrix, but having 4 bytes here instead just feels
// right to me.

typedef union {
  uint32_t data_length;
  uint8_t raw[4];
} qmk_rc_data_length_t;

typedef enum {
  RECEIVING_COMMAND,
  RECEIVING_DATA_LENGTH,
  RECEIVING_DATA,
} qmk_rc_parser_state_t;

typedef struct {
  qmk_rc_parser_state_t state;
  uint8_t data_length_bytes_read;
  uint32_t data_bytes_read;
  qmk_rc_data_length_t data_length;
} qmk_rc_parser_t;

typedef struct {
  qmk_rc_command_id_t id;
  uint32_t data_length;
  uint8_t* data;
} qmk_rc_command_t;

void qmk_rc_receive(uint8_t *buffer, uint16_t max_buffer_length, uint8_t* data, uint8_t data_length);
