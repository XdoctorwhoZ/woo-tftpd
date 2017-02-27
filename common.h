#ifndef WOOTFTPD_COMMON_H
#define WOOTFTPD_COMMON_H

// std
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <syslog.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>

// sys
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/types.h>

// net
#include <arpa/inet.h>

//! Path to the settings file
#define WOOTFTPD_SETTINGS   "/etc/woo-tftpd.conf"

//! Semaphore name used by processes
#define WOOTFTPD_SEM_NAME   "/wootftpd_sem"

//! Shared memory key parameters
#define WOOTFTPD_SHM_NAME   "/wootftpd_shm"

//! Maximun number of simulaneus manageable operation
#define WOOTFTPD_MAX_NBOP   5

// Operation flags
#define WOOTFTPD_RESET      0b00000000
#define WOOTFTPD_ENABLE     0b00000001
#define WOOTFTPD_END        0b00000010
#define WOOTFTPD_UPLOAD     0b00000100
#define WOOTFTPD_DOWNLOAD   0b00001000
#define WOOTFTPD_TIMEOUT    0b00010000

// To check flag
#define WOOTFTPD_CHECK_FLAG(oflags,flag) ((oflags&flag) == flag)

//! tftp operation slot
//!
struct wootftpd_operation
{
    //! Operation flags
    //!
    uint8_t oflags;

    //! Name of the file transfered
    //!
    char filename[128];

    //! Port of the tftp client socket
    //!
    int port;

    //! Ip of the tftp client socket
    //!
    char ip[INET6_ADDRSTRLEN];

    //! Current number of block transfered
    //!
    uint32_t block;

    //! Total number of block to transfer
    //!
    uint32_t total_block;
};

//! data shared between server and gui
//!
struct wootftpd_shared_data
{
    //! Client pid
    //!
    pid_t client_pid;

    //! Current number of running operation
    //!
    uint8_t nb_operation;

    //! Array with all operation slots
    //!
    struct wootftpd_operation operations[WOOTFTPD_MAX_NBOP];
};

#endif // WOOTFTPD_COMMON_H
