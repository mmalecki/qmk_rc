typedef struct {
  qmk_rc_command_t command;
  uint32_t data_length;
  uint8_t data[];
} qmk_rc_parser_test_case_t;
