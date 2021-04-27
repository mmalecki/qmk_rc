#include "qmk_rc.h"
#include "task.h"
#include "test-common.h"

#define QMK_RC_BUFFER_MAX 128
uint8_t buffer[QMK_RC_BUFFER_MAX];

qmk_rc_parser_test_case_t test_case = {
  .command = {
    .id = 15,
    .data_length = 5,
    .data = (uint8_t*) "qmkrc"
  },
  .data = {15, 5, 0, 0, 0, 113, 109, 107, 114, 99},
  .data_length = 10,
};

int callbacks = 0;

void qmk_rc_process_command_user(qmk_rc_command_t* command) {
  printf("command id: %d, command data: '%s'\n", command->id, command->data);
  ASSERT_EQ(++callbacks, 1);
  ASSERT_EQ(command->id, test_case.command.id);
  ASSERT_EQ(command->data_length, test_case.command.data_length);
  ASSERT_STR_EQ((const char*) command->data, (const char*) test_case.command.data);
}

int main() {
  qmk_rc_receive(buffer, QMK_RC_BUFFER_MAX, test_case.data, test_case.data_length);
  ASSERT_EQ(callbacks, 1);
}
