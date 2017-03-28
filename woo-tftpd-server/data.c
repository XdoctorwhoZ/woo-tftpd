//!
//! \file data.c
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
#include "data.h"

/* ============================================================================
 *
 * */
int woo_init(struct woo_data* this_obj)
{
    // log
    syslog(LOG_DEBUG, "Initialization...");

    // === Init

    this_obj->semaphore = SEM_FAILED;
    this_obj->smdata = MAP_FAILED;

    // === Semaphore

    // Open the semaphore, if it does not exist continue whitout the feature
    this_obj->semaphore = sem_open(WOOTFTPD_SEM_NAME, 0);
    if( this_obj->semaphore == SEM_FAILED )
    {
        syslog(LOG_ERR, "Unable to open the semaphore (%s)", strerror(errno));
        this_obj->enable = 0;
        return 20;
    }

    // === Shared Memory

    // Create shared memory
    this_obj->smid = shm_open(WOOTFTPD_SHM_NAME, O_RDWR, S_IRUSR | S_IWUSR);
    if( this_obj->smid == -1 )
    {
        syslog(LOG_ERR, "Unable to open the shared memory segment (%s)", strerror(errno));
        this_obj->enable = 0;
        woo_free(this_obj);
        return 30;
    }

    // Map shared memory
    this_obj->smdata = (struct wootftpd_shared_data*)
        mmap(NULL, sizeof(struct wootftpd_shared_data),
            PROT_READ | PROT_WRITE, MAP_SHARED, this_obj->smid, 0);
    if( this_obj->smdata == MAP_FAILED )
    {
        syslog(LOG_ERR, "Unable to map shared memory (%s)", strerror(errno));
        this_obj->enable = 0;
        woo_free(this_obj);
        return 40;
    }

    // === Check gui pid

    // Check if process with this pid exist
    if( kill(this_obj->smdata->client_pid, 0) != 0 )
    {
        syslog(LOG_ERR, "No gui process for pid %d", this_obj->smdata->client_pid);
        this_obj->enable = 0;
        woo_free(this_obj);
        return 50;
    }

    // === Success

    // Feature anabled
    this_obj->enable = 1;

    // allocate an operation id
    if( woo_alloc_operation(this_obj) != 0 )
    {
        this_obj->enable = 0;
        woo_free(this_obj);
        return 60;
    }

    // log and return
    syslog(LOG_DEBUG, "Initialization ok");

    // wake up gui
    woo_wake_up_gui(this_obj);

    return 0;
}

/* ============================================================================
 *
 * */
int woo_wake_up_gui(struct woo_data* this_obj)
{
    // check that woo feature is enabled
    if( !this_obj->enable )
    {
        return 10;
    }

    // Send signal
    syslog(LOG_DEBUG, "Wake up gui");
    kill(this_obj->smdata->client_pid, SIGUSR1);
    return 0;
}

/* ============================================================================
 *
 * */
int woo_alloc_operation(struct woo_data* this_obj)
{
    int x;

    // check that woo feature is enabled
    if( !this_obj->enable )
    {
        return 10;
    }

    // log
    syslog(LOG_DEBUG, "Allocate id");

    // lock semaphore
    sem_wait( this_obj->semaphore );

    // check nb of operations
    if( this_obj->smdata->nb_operation >= WOOTFTPD_MAX_NBOP )
    {
        // unlock semaphore and return err code
        sem_post( this_obj->semaphore );
        syslog(LOG_ERR, "Total number of id already allocated");
        return 20;
    }

    // look for a free id
    this_obj->opid = -1;
    for(x=0; x<WOOTFTPD_MAX_NBOP; x++)
    {
        uint8_t oflags = this_obj->smdata->operations[x].oflags;
        if( ! WOOTFTPD_CHECK_FLAG(oflags, WOOTFTPD_ENABLE) )
        {
            // set this operation slot as enable
            this_obj->opid = x;
            this_obj->smdata->nb_operation++;
            this_obj->smdata->operations[x].oflags  = WOOTFTPD_RESET;
            this_obj->smdata->operations[x].oflags |= WOOTFTPD_ENABLE;
            this_obj->smdata->operations[x].block = 0;
            break;
        }
    }

    // all operation id are used
    if( this_obj->opid == -1 )
    {
        this_obj->enable = 0;
        sem_post( this_obj->semaphore );
        syslog(LOG_ERR, "No free id found");
        return 30;
    }

    // log
    syslog(LOG_DEBUG, "Id found = %d", this_obj->opid);
    sem_post( this_obj->semaphore );
    return 0;
}

/* ============================================================================
 *
 * */
int woo_set_ip(struct woo_data* this_obj, const char* ip)
{
    // check that woo feature is enabled
    if( !this_obj->enable )
    {
        return 10;
    }

    // copy ip
    const int opid = this_obj->opid;
    strcpy(this_obj->smdata->operations[opid].ip, ip);
}

/* ============================================================================
 *
 * */
int woo_set_port(struct woo_data* this_obj, int port)
{
    // check that woo feature is enabled
    if( !this_obj->enable )
    {
        return 10;
    }

    // copy port
    const int opid = this_obj->opid;
    this_obj->smdata->operations[opid].port = port;
}


/* ============================================================================
 *
 * */
int woo_set_ip_port_from_socket_id(struct woo_data* this_obj, int sid)
{
    // check that woo feature is enabled
    if( !this_obj->enable )
    {
        return 10;
    }

    // assume s is a connected socket
    socklen_t len;
    struct sockaddr_storage addr;
    char ipstr[INET6_ADDRSTRLEN];
    int port;

    len = sizeof addr;
    getpeername(sid, (struct sockaddr*)&addr, &len);

    // deal with both IPv4 and IPv6:
    if (addr.ss_family == AF_INET) {
        struct sockaddr_in *s = (struct sockaddr_in *)&addr;
        port = ntohs(s->sin_port);
        inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
    } else { // AF_INET6
        struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
        port = ntohs(s->sin6_port);
        inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
    }

    syslog(LOG_DEBUG, "Peer IP address: %s\n", ipstr);
    syslog(LOG_DEBUG, "Peer port      : %d\n", port);

    woo_set_ip(this_obj, ipstr);
    woo_set_port(this_obj, port);

    return 0;
}


/* ============================================================================
 *
 * */
int woo_set_filename(struct woo_data* this_obj, const char* filename)
{
    // check that woo feature is enabled
    if( !this_obj->enable )
    {
        return 10;
    }

    // copy filename
    const int opid = this_obj->opid;
    strcpy(this_obj->smdata->operations[opid].filename, filename);
}

/* ============================================================================
 *
 * */
int woo_set_flag_upload(struct woo_data* this_obj)
{
    // check that woo feature is enabled
    if( !this_obj->enable )
    {
        return 10;
    }

    //
    const int opid = this_obj->opid;
    this_obj->smdata->operations[opid].oflags |= WOOTFTPD_UPLOAD;
}

/* ============================================================================
 *
 * */
int woo_set_flag_download(struct woo_data* this_obj)
{
    // check that woo feature is enabled
    if( !this_obj->enable )
    {
        return 10;
    }

    //
    const int opid = this_obj->opid;
    this_obj->smdata->operations[opid].oflags |= WOOTFTPD_DOWNLOAD;
}

/* ============================================================================
 *
 * */
int woo_set_total_block(struct woo_data* this_obj, int nb_block)
{
    // check that woo feature is enabled
    if( !this_obj->enable )
    {
        return 10;
    }

    // copy filename
    const int opid = this_obj->opid;
    this_obj->smdata->operations[opid].total_block = nb_block;
}

/* ============================================================================
 *
 * */
int woo_increment_block(struct woo_data* this_obj, int value)
{
    // check that woo feature is enabled
    if( !this_obj->enable )
    {
        return 10;
    }

    // append new value
    const int opid = this_obj->opid;
    this_obj->smdata->operations[opid].block += value;
}

/* ============================================================================
 *
 * */
int woo_end_operation(struct woo_data* this_obj)
{
    // check that woo feature is enabled
    if( !this_obj->enable )
    {
        return 10;
    }

    // set flags and put progression at 100%
    syslog(LOG_DEBUG, "End operation");
    const int opid = this_obj->opid;
    this_obj->smdata->operations[opid].oflags |= WOOTFTPD_END;
    if( WOOTFTPD_CHECK_FLAG(this_obj->smdata->operations[opid].oflags, WOOTFTPD_UPLOAD) )
    {
        this_obj->smdata->operations[opid].block = 1;
        this_obj->smdata->operations[opid].total_block = 1;    
    }
    else
    {
        
    }
}

/* ============================================================================
 *
 * */
int woo_free(struct woo_data* this_obj)
{
    if( this_obj->smdata != MAP_FAILED )
    {
        munmap(this_obj->smdata, sizeof(struct wootftpd_shared_data));    
    }
    if( this_obj->semaphore != SEM_FAILED )
    {
        sem_close(this_obj->semaphore);
    }
    return 0;
}
