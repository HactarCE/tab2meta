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
    int tab_is_down = 0, tab_give_up = 0;
    struct input_event input;

    setbuf(stdin, NULL), setbuf(stdout, NULL);

    while (read_event(&input)) {
        if (input.type == EV_MSC && input.code == MSC_SCAN)
            continue;

        if (input.type != EV_KEY) {
            write_event(&input);
            continue;
        }

        if (tab_is_down) {
            if (equal(&input, &tab_down) ||
                equal(&input, &tab_repeat))
                continue;

            if (equal(&input, &tab_up)) {
                tab_is_down = 0;
                if (tab_give_up) {
                    tab_give_up = 0;
                    write_event(&meta_up);
                    continue;
                }
                write_event(&tab_down);
                write_event(&syn);
                usleep(20000);
                write_event(&tab_up);
                continue;
            }

            if (!tab_give_up && input.value) {
                tab_give_up = 1;
                write_event(&meta_down);
                write_event(&syn);
                usleep(20000);
            }
        } else if (equal(&input, &tab_down)) {
            tab_is_down = 1;
            continue;
        }

        if (input.code == KEY_TAB)
            input.code = KEY_TAB;
        write_event(&input);
    }
}
