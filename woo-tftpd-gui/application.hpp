#ifndef WOOTFTPD_APPLICATION_HPP
#define WOOTFTPD_APPLICATION_HPP
//!
//! \file application.hpp
//!
// Copyright 2016 XdoctorwhoZ
// This file is part of WooToHpa.
//
// WooToHpa is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// WooToHpa is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with WooToHpa.  If not, see <http://www.gnu.org/licenses/>.

// std
#include <iostream>

// Qt
#include <QLabel>
#include <QTimer>
#include <QWidget>
#include <QVector>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QApplication>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QCommandLineParser>

// WooToHpa
#include "data.hpp"
#include "widget.hpp"

// ---
namespace woo { namespace tftpd {

//! Class that manage the Qt application
//!
class application : public QApplication
{
    Q_OBJECT

    //! Application data
    //!
    data m_data;

    //! Widget to display operation progress
    //!
    widget m_widget;

    //! Timer to schedule to all
    //!
    QTimer m_timer;

    //! Signal catcher
    //!
    sigset_t m_sigset;

    //! Array with date for each operation end
    //!
    QVector<time_t> m_end_array;

    QVector<QPair<uint32_t, time_t>> m_timeout_array;

public:

    //! Constructor
    //!
    application(int& argc, char** argv);
    
    //! Destructor
    //!
    ~application();

    //! Start application
    //!
    int start();

public slots:

    //! Cyclic task
    //!
    void work();

private:

    //! Stop gui, hide it and wait for an external signal
    //!
    void hide_and_sleep();

    //! Check if operations are finished and removed them
    //!
    void monitor_operations();

signals:

    void refresh_items();

};

} // tftpd
} // woo

#endif // WOOTFTPD_APPLICATION_HPP
