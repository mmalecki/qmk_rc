# Commands
QMK RC supports the following commands at the moment:

| Command name              | Description                                    | Command ID | Parameters                                          |
| ------------------------- | ----------------------------------------------- | ---------- | -------------------------------------------------- |
| `OLED_OFF`                | Turn off the OLED screen                        | 1          | none                                               |
| `OLED_ON`                 | Turn on the OLED screen                         | 2          | none                                               |
| `OLED_WRITE`              | Write to the OLED screen                        | 3          | null-terminated string to write to the OLED screen |
| `OLED_CLEAR`              | Clear the OLED screen                           | 4          | none
| `RGBLIGHT_OFF`            | Turn off RGB Light                              | 5          | none
| `RGBLIGHT_ON`             | Turn on RGB Light                               | 6          | none
| `RGBLIGHT_SETRBG_RANGE`   | Set a range of RGB Light diodes to a RGB color  | 7          | 5 bytes, first three indicating the color, last two indicating the range
| `RGB_MATRIX_OFF`          | Turn off RGB Matrix                             | 8          | none
| `RGB_MATRIX_ON`           | Turn on RGB Matrix                              | 9          | none
| `RGB_MATRIX_SETRGB_RANGE` | Set a range of RGB Matrix diodes to a RGB color | 10         | 5 bytes, first three indicating the color, last two indicating the range
| `LAYER_ON`                | Turn on a layer                                 | 11         | single byte, indicating the layer number
| `LAYER_OFF`               | Turn off a layer                                | 12         | single byte, indicating the layer number
| `LAYER_CLEAR`             | Turn off all layers                             | 13         | none
| `LAYER_MOVE`              | Turn specified layer on, turning off all others | 14         | single byte, indicating the layer number
| `SEND_STRING`             | "Type" a string                                 | 15         | null-terminated string to "type"

Most of these commands correspond to a single QMK API call.

(This software is still under heavy development, and so the list is subject to change.)

## Sending commands

To issue a command via [`qmkrcd`](https://github.com/mmalecki/qmkrcd), make an HTTP request to its HTTP port, containing
the command ID and parameters, if any are required. For example, to issue a
`RGBLIGHT_SET_RANGE`, setting the first, up to the fourth diode to RGB color
`(50, 75, 90)`:

```json
{
  "id": 7,
  "data": [50, 75, 90, 1, 4]
}
```
or, in `curl`-speak:

```sh
curl 127.0.0.1:9916/command -XPOST \
  -H 'content-type: application/json' \
  -d '{"id":7,"data":[50,75,90,1,4]}'
```

(You can also write to the HID device manually, following the QMK RC protocol.)
