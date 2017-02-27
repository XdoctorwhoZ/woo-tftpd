#ifndef WOOTFTPD_DATA_HPP
#define WOOTFTPD_DATA_HPP
//!
//! \file data.hpp
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

// Qt
#include <QPoint>
#include <QSettings>
#include <QSharedPointer>

// WooTftpd
#include "common.h"

// ---
namespace woo { namespace tftpd {

//! Gather data shared acros the application
//!
class data : public QObject
{
    Q_OBJECT

    //! If true, data from previous instance of wootftpd will be destroyed
    //!
    bool m_force_boot;

    //! Semaphore to sync access with shared memory
    //!
    sem_t* m_semaphore;

    //! Share memory id
    //!
    int m_smid;

    //! Pointer on the shared memory
    //!
    struct wootftpd_shared_data* m_smdata;

    // === SETTINGS ===

    //! User setting
    //!
    QSharedPointer<QSettings> m_settings;

    //! Width of the widget
    //!
    int m_widget_width;

    //! Height of each operation view
    //!
    int m_widget_item_height;

    //! Screen id
    //!
    int m_widget_screen;

    //! Widget position
    //!
    QPoint m_widget_position;

    //! Time between the end of the operation and the deletion of the widget
    //!
    int m_end_time_s;

public:

    //! Constructor
    //!
    data();

    //! Destructor
    //!
    ~data();

    //! Create default settings
    //!
    void create_default_settings();

    //! Initialiaze object
    //!
    int initialize();

    //! Free object
    //!
    int destroy();

    //! Set force option
    //!
    inline void set_force_boot(bool value);

    //! Getter for the shared memory data
    //!
    inline struct wootftpd_shared_data& get_smdata();

    // getters
    inline int get_widget_width() const;
    inline int get_widget_item_height() const;
    inline int get_widget_screen() const;
    inline const QPoint& get_widget_position() const;

    inline int get_end_time() const;


    void sem_lock()
    {
        sem_wait(m_semaphore);
    }
    void sem_unlock()
    {
        sem_post(m_semaphore);
    }


private:

    //! Load settings
    //!
    int load_settings();

signals:

    //! Emmitted when settings has been loaded
    //!
    void settings_loaded();

};

/* ============================================================================
 *
 * */
inline void data::set_force_boot(bool value)
{
    m_force_boot = value;
}

/* ============================================================================
 *
 * */
inline struct wootftpd_shared_data& data::get_smdata()
{
    return *m_smdata;
}

/* ============================================================================
 *
 * */
inline int data::get_widget_width() const
{
    return m_widget_width;
}

/* ============================================================================
 *
 * */
inline int data::get_widget_item_height() const
{
    return m_widget_item_height;
}

/* ============================================================================
 *
 * */
inline int data::get_widget_screen() const
{
    return m_widget_screen;
}

/* ============================================================================
 *
 * */
inline const QPoint& data::get_widget_position() const
{
    return m_widget_position;
}

/* ============================================================================
 *
 * */
inline int data::get_end_time() const
{
    return m_end_time_s;
}

} // tftpd
} // woo

#endif // WOOTFTPD_DATA_HPP
