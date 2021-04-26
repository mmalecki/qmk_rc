# Custom commands
No one size fits all. If QMK RC doesn't recognize a built-in command, it
(in a fashion similar to QMK itself), calls a user function:

```
__attribute__((weak)) void qmk_rc_process_command_user(qmk_rc_command_t* command) { }
```

And so, if you wanted to re-implement the `OLED_OFF` command, you could write:

```c
#define MY_OLED_OFF 128
void qmk_rc_process_command_user(qmk_rc_command_t* command) {
  switch (command->id) {
    case MY_OLED_OFF: oled_off(); break;
  }
}
```
