#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <linux/input.h>

// clang-format off
const struct input_event
tab_up          = {.type = EV_KEY, .code = KEY_TAB,      .value = 0},
meta_up         = {.type = EV_KEY, .code = KEY_LEFTMETA, .value = 0},
tab_down        = {.type = EV_KEY, .code = KEY_TAB,      .value = 1},
meta_down       = {.type = EV_KEY, .code = KEY_LEFTMETA, .value = 1},
tab_repeat      = {.type = EV_KEY, .code = KEY_TAB,      .value = 2},
meta_repeat     = {.type = EV_KEY, .code = KEY_LEFTMETA, .value = 2},
syn             = {.type = EV_SYN, .code = SYN_REPORT,   .value = 0};
// clang-format on

int equal(const struct input_event *first, const struct input_event *second) {
    return first->type == second->type && first->code == second->code &&
           first->value == second->value;
}

int read_event(struct input_event *event) {
    return fread(event, sizeof(struct input_event), 1, stdin) == 1;
}

void write_event(const struct input_event *event) {
    if (fwrite(event, sizeof(struct input_event), 1, stdout) != 1)
        exit(EXIT_FAILURE);
}

int main(void) {
    int tab_is_meta = 0;
    struct input_event input, key_down, key_up, key_repeat;
    enum { START, TAB_HELD, KEY_HELD } state = START;

    setbuf(stdin, NULL), setbuf(stdout, NULL);

    while (read_event(&input)) {
        if (input.type == EV_MSC && input.code == MSC_SCAN)
            continue;

        if (input.type != EV_KEY) {
            write_event(&input);
            continue;
        }

        switch (state) {
            case START:
                if (tab_is_meta) {
                    if (input.code == KEY_SPACE) {
                        input.code = KEY_LEFTMETA;
                        if (input.value == 0)
                            tab_is_meta = 0;
                    }
                    write_event(&input);
                } else {
                    if (equal(&input, &tab_down) ||
                        equal(&input, &tab_repeat)) {
                        state = TAB_HELD;
                    } else {
                        write_event(&input);
                    }
                }
                break;
            case TAB_HELD:
                if (equal(&input, &tab_down) || equal(&input, &tab_repeat))
                    break;
                if (input.value != 1) {
                    write_event(&tab_down);
                    write_event(&syn);
                    usleep(20000);
                    write_event(&input);
                    state = START;
                } else {
                    key_down = key_up = key_repeat = input;

                    key_up.value     = 0;
                    key_repeat.value = 2;
                    state            = KEY_HELD;
                }
                break;
            case KEY_HELD:
                if (equal(&input, &tab_down) || equal(&input, &tab_repeat))
                    break;
                if (equal(&input, &key_down) || equal(&input, &key_repeat))
                    break;
                if (equal(&input, &key_up)) {
                    write_event(&meta_down);
                    tab_is_meta = 1;
                } else {
                    write_event(&tab_down);
                }
                write_event(&syn);
                usleep(20000);
                write_event(&key_down);
                write_event(&syn);
                usleep(20000);
                write_event(&input);
                state = START;
                break;
        }
    }
}
