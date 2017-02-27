#ifndef WOO_TFTPD_SERVER_DATA_H
#define WOO_TFTPD_SERVER_DATA_H
//!
//! \file data.h
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

// WooTftpd
#include "common.h"

//!
//!
struct woo_data
{

    //! True if every thing is ok to use gui feature
    //! - gui enable
    //! - semaphore and shared memory usable
    //!
    int enable;

    //! Semaphore to sync access with shared memory
    //!
    sem_t* semaphore;

    //! Share memory id
    //!
    int smid;

    //! Operation id
    //!
    int opid;

    //! Pointer on the shared memory
    //!
    struct wootftpd_shared_data* smdata;

};

//! Initialize context to talk with the gui
//!
int woo_init(struct woo_data* this_obj);

//! Send a signal to the gui to wake it up
//!
int woo_wake_up_gui(struct woo_data* this_obj);

//! 
//!
int woo_alloc_operation(struct woo_data* this_obj);

//!
//!
int woo_set_ip(struct woo_data* this_obj, const char* ip);

int woo_set_port(struct woo_data* this_obj, int port);

//!
//!
int woo_set_ip_port_from_socket_id(struct woo_data* this_obj, int sid);

//!
//!
int woo_set_filename(struct woo_data* this_obj, const char* filename);

int woo_set_flag_upload(struct woo_data* this_obj);
int woo_set_flag_download(struct woo_data* this_obj);

//!
//!
int woo_set_total_block(struct woo_data* this_obj, int nb_block);

//!
//!
int woo_increment_block(struct woo_data* this_obj, int value);

//! Flag operation as ended
//!
int woo_end_operation(struct woo_data* this_obj);

//! Clean context
//!
int woo_free(struct woo_data* this_obj);

#endif // WOO_TFTPD_SERVER_DATA_H
