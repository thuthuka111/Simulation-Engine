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
    extern "C" {

    void server_start();

    void start_client();

    } // extern "C"
}


#endif //FAKEBASKETBALL_SERVER_H
