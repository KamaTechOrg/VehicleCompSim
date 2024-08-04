#pragma once


class Mamge_req {
public:
    void recv();
    void send();

private:
    void *data;
};