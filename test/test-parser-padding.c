#include "qmk_rc.h"
#include "task.h"
#include "test-common.h"

#define QMK_RC_BUFFER_MAX 128
uint8_t buffer[QMK_RC_BUFFER_MAX];

int callbacks = 0;

void qmk_rc_process_command_user(qmk_rc_command_t* command) {
  printf("command id: %d, command data: '%s'\n", command->id, command->data);
  ASSERT_EQ(++callbacks, 1);
  ASSERT_EQ(command->id, 7);
  ASSERT_EQ(command->data_length, 0);
}

int main() {
  uint8_t data[] = {
    7, 0, 0, 0, 0, // command
    0, 0, 0, 0, 0  // padding
  };
  qmk_rc_receive(buffer, QMK_RC_BUFFER_MAX, data, sizeof(data));
  ASSERT_EQ(callbacks, 1);
}
