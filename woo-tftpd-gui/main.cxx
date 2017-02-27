//!
//! \file main.cxx
//!
// Copyright 2016 XdoctorwhoZ
// This file is part of WooTftpd.
//
// WooTftpd is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// WooTftpd is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with WooTftpd.  If not, see <http://www.gnu.org/licenses/>.

// std
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <syslog.h>
#include <string.h>
#include <iostream>

// sys
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

// Woo
#include "application.hpp"

// ---
using namespace std;
using namespace woo;
using namespace woo::tftpd;

//! handler for USR1
//!
void sigusr1_callback_handler(int signum)
{ }

//! Run the application
//!
int run_application(int& argc, char** argv)
{
    // catch USR1 signal
    signal(SIGUSR1, sigusr1_callback_handler);

    // start application
    QScopedPointer<application> app(new application(argc, argv));
    return app->start();
}

//! Program enter point
//!
int main(int argc, char *argv[])
{
    int status = 0;

    // Create syslog channel
    openlog("woo-tftpd-gui", LOG_PID, LOG_USER);

    // first fork
    pid_t pid_0 = fork();
    if( pid_0 == 0 )
    {
        // second fork
        pid_t pid_1 = fork();
        if( pid_1 == 0 )
        {
            status = run_application(argc, argv);
        }
    }
    else
    {
        // wait for first child
        wait();
    }

    // close syslog channel
    closelog();

    // end
    return status;
}
