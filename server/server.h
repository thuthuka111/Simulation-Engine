//
// Created by Thuthuka on 2023/02/06.
//

#ifndef FAKEBASKETBALL_SERVER_H
#define FAKEBASKETBALL_SERVER_H

#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <new>

#pragma comment(lib, "bcrypt.lib")

namespace networking {
    struct Position {
        float x;
        float y;
    };

    struct Velocity {
        float x;
        float y;
    };

    struct Player {
        uint32_t rigid_body_id;
        Position position;
        Velocity velocity;
    };

    extern "C" {

    bool start_client(const char *server_string,
                      void (*send_data_func)(Player),
                      Player (*get_data_func)());

    void server_start();

    } // extern "C"
}


#endif //FAKEBASKETBALL_SERVER_H
