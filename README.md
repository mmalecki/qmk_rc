# QMK RC
QMK RC is a project that aims to bring the same convenience to controlling
your [QMK](https://qmk.fm/) keyboard from your computer, as QMK did to programming keyboards.

## Quick start
To get started with QMK RC, you need to set up the keyboard and the host. Let's
start with the keyboard:

### Keyboard firmware
First, fetch the QMK RC source either into your keymap, or user directory:

```sh
bash <(wget -qO- https://raw.githubusercontent.com/mmalecki/qmk_rc/latest/bin/qmkrc_fetch)
```
Next, add these files to the `SRC` list in your `rules.mk` and enable the necessary
QMK feature:

```
SRC += qmk_rc.c
RAW_ENABLE = yes
```

And include the header in your `keymap.c`:

```c
#include "qmk_rc.h"
```

From here on, you can start feeding the QMK RC raw HID data:

```c
#ifdef RAW_ENABLE
#define QMK_RC_BUFFER_MAX 64
uint8_t qmk_rc_buffer[QMK_RC_BUFFER_MAX] = {};

void raw_hid_receive(uint8_t *data, uint8_t length) {
  qmk_rc_receive(qmk_rc_buffer, QMK_RC_BUFFER_MAX, data, length);
}
#endif
```
Flash your keyboard with the modified software, as you usually do.

### Host software
The host application is distributed as an npm package. To get it installed, run:

```sh
npm -g install qmkrcd
```

### Play time!
Now, to the fun part.

Launch `qmkrcd` by typing `qmkrcd`. It will scan the HID devices attached to
the host, and attempt to find one that quacks like a QMK-supported keybaord.

Alternatively, you can have `qmkrcd` connect to a specified device by passing
its name as an argument: `qmkrcd 'foostan corne'`.

If `qmkrcd` doesn't find your device, please open an issue on [its repository](https://github.com/mmalecki/qmkrcd).

From now on, you should be able to control your keyboard.
For example, to write a string to an OLED attached to your keyboard:

```sh
curl 127.0.0.1:9916/command -XPOST \
  -H 'content-type: application/json' \
  -d '{"id":3,"data":"Hello, QMK RC!"}' -v
```

For a full list of built-in commands, see [Commands](doc/commands.md).

## Acknowledgments
* The QMK team for authoring an amazing piece of software
* `libuv` team for their test framework
