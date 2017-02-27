//!
//! \file data.cxx
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
#include <iostream>

// Qt
#include <QFileInfo>

// WooTftpd
#include "data.hpp"

// ---
using namespace woo;
using namespace woo::tftpd;

/* ============================================================================
 *
 * */
data::data()
    : QObject()
    , m_force_boot(false)
    , m_semaphore(0)
    , m_smdata(0)
    , m_widget_width(512)
    , m_widget_item_height(64)
{ }

/* ============================================================================
 *
 * */
data::~data()
{ }

/* ============================================================================
 *
 * */
int data::initialize()
{
    int status;

    // === Force boot

    if(m_force_boot)
    {
        syslog(LOG_NOTICE, "Force boot");
        sem_unlink(WOOTFTPD_SEM_NAME);
        shm_unlink(WOOTFTPD_SHM_NAME);
    }

    // === Settings

    status = load_settings();
    if(status)
    {
        return status;
    }

    // === Semaphore

    // Check that no other gui is activated
    m_semaphore = sem_open(WOOTFTPD_SEM_NAME, 0);
    if( m_semaphore != SEM_FAILED )
    {
        syslog(LOG_ERR, "An other woo-tftpd instance is probably already started (or try -f)");
        return 10;
    }

    // Create the semaphore, with intial value 1 (it will be used as mutex)
    m_semaphore = sem_open(WOOTFTPD_SEM_NAME, O_CREAT, 0644, 0);
    if( m_semaphore == SEM_FAILED )
    {
        syslog(LOG_ERR, "Unable to create the semaphore");
        return 20;
    }

    // === Shared Memory

    // Create shared memory
    m_smid = shm_open(WOOTFTPD_SHM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if( m_smid == -1 )
    {
        syslog(LOG_ERR, "Unable to create the shared memory segment (%s)", strerror(errno));
        return 30;
    }

    // Resize shared memory segment
    status = ftruncate(m_smid, sizeof(struct wootftpd_shared_data));
    if( status == -1 )
    {
        syslog(LOG_ERR, "Fail to resize shared memory (%s)", strerror(errno));
        return 40;
    }

    // Map shared memory
    m_smdata = static_cast<struct wootftpd_shared_data*>(
        mmap(NULL, sizeof(struct wootftpd_shared_data),
            PROT_READ | PROT_WRITE, MAP_SHARED, m_smid, 0)
    );
    if (m_smdata == MAP_FAILED)
    {
        syslog(LOG_ERR, "Fail map shared memory (%s)", strerror(errno));
        return 50;
    }

    // === Initialization of shared memory data

    // Shared the pid of the client
    m_smdata->client_pid = getpid();

    // Reset all operation
    m_smdata->nb_operation = 0;
    for(int x=0; x<WOOTFTPD_MAX_NBOP; x++)
    {
        m_smdata->operations[x].oflags = WOOTFTPD_RESET;
    }

    // === unlock semaphore

    sem_post(m_semaphore);

    return 0;
}

/* ============================================================================
 *
 * */
int data::destroy()
{
    // If the semaphore has been created, close and delete
    if(m_semaphore != 0)
    {
        sem_close(m_semaphore);
        sem_unlink(WOOTFTPD_SEM_NAME);
    }

    // detach from the shared memory segment
    if(m_smdata != 0)
    {
        munmap(m_smdata, sizeof(struct wootftpd_shared_data));
        shm_unlink(WOOTFTPD_SHM_NAME);
    }
}

/* ============================================================================
 *
 * */
int data::load_settings()
{
    // Create setting manager
    m_settings = 
        QSharedPointer<QSettings>(
            new QSettings(WOOTFTPD_SETTINGS, QSettings::IniFormat)
            );

    // load settings
    m_widget_screen = m_settings->value("widget/screen").toInt();
    m_widget_position = m_settings->value("widget/position").toPoint();
    m_end_time_s = m_settings->value("operation/end_time").toInt();

    // log
    syslog(LOG_DEBUG, "Settings[widget] screen=%d", m_widget_screen);
    syslog(LOG_DEBUG, "Settings[widget] position=point(%d,%d)", m_widget_position.x(), m_widget_position.y());
    syslog(LOG_DEBUG, "Settings[operation] end_time (%d)", m_end_time_s);

    // signal to app
    emit settings_loaded();

    return 0;
}
