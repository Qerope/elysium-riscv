#pragma once
#include <stdint.h>

#define PORT_QUEUE 16
#define MSG_DATA 56

struct msg {
    uint32_t id, len;
    uint8_t data[MSG_DATA];
};

struct port {
    struct msg q[PORT_QUEUE];
    int head, count;
};

int port_send(struct port *p, const struct msg *m);
int port_receive(struct port *p, struct msg *out);
