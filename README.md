# tab2meta

Turn your tab key into the meta key (a.k.a. win key or OS key) when chorded
to another key (on key release only).

Forked and modified from [`space2meta`].

## Dependencies

- [Interception Tools][interception-tools]

## Building

```
$ git clone git@gitlab.com:interception/linux/plugins/tab2meta.git
$ cd tab2meta
$ cmake -Bbuild
$ cmake --build build
```

## Execution

`tab2meta` is an unofficial [_Interception Tools_][interception-tools] plugin. A
suggested `udevmon` job configuration is:


```yaml
- JOB: "intercept -g $DEVNODE | tab2meta | uinput -d $DEVNODE"
  DEVICE:
    EVENTS:
      EV_KEY: [KEY_TAB]

```

To compose functionality with [`caps2esc`], for example, you do:

```yaml
- JOB: "intercept -g $DEVNODE | caps2esc | tab2meta | uinput -d $DEVNODE"
  DEVICE:
    EVENTS:
      EV_KEY: [KEY_CAPSLOCK, KEY_ESC, KEY_TAB]

```

For more information about the [_Interception Tools_][interception-tools], check
the project's website.

## Installation

This project is not in any official repositories; install it by cloning this
repository and running `cmake . && sudo make install`. To uninstall, run `sudo 
make uninstall`.

## Caveats

As always, there's always a caveat:

- visual delay when inserting tab.
- `intercept -g` will "grab" the detected devices for exclusive access.
- If you tweak your key repeat settings, check whether they get reset.
  Please check [this report][key-repeat-fix] about the resolution.

## License

<a href="https://gitlab.com/interception/linux/plugins/caps2esc/blob/tab2meta/LICENSE.md">
    <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/0/0b/License_icon-mit-2.svg/120px-License_icon-mit-2.svg.png" alt="MIT">
</a>

- Copyright © 2019 Francisco Lopes da Silva
- Copyright © 2020 HactarCE

[interception]: https://github.com/oblitum/Interception
[`caps2esc`]: https://gitlab.com/interception/linux/plugins/caps2esc
[`space2meta`]: https://gitlab.com/interception/linux/plugins/space2meta
[interception-tools]: https://gitlab.com/interception/linux/tools
[key-repeat-fix]: https://github.com/oblitum/caps2esc/issues/1
