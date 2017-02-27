//!
//! \file application.cxx
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

// WooToHpa
#include "application.hpp"

// ---
using namespace std;
using namespace woo::tftpd;

/* ============================================================================
 *
 * */
application::application(int& argc, char** argv)
    : QApplication(argc, argv)
    , m_widget(m_data)
    , m_end_array(WOOTFTPD_MAX_NBOP)
    , m_timeout_array(WOOTFTPD_MAX_NBOP)
{ }

/* ============================================================================
 *
 * */
application::~application()
{
    // kill data allocated
    m_data.destroy();
}

/* ============================================================================
 *
 * */
int application::start()
{
    int status;

    // set syslog mask
    // setlogmask(LOG_UPTO(LOG_ERR));

    // log
    syslog(LOG_DEBUG, "Initialization...");

    // configure signal catcher
    sigemptyset(&m_sigset);
    sigaddset(&m_sigset, SIGUSR1);
    sigprocmask(SIG_BLOCK, &m_sigset, NULL);

    // no end at beginning
    m_end_array.fill(0);
    m_timeout_array.fill(QPair<uint32_t, time_t>(0,0));

    // Create a command line parser
    QCommandLineParser parser;

    // A boolean option with multiple names (-f, --force)
    QCommandLineOption forceOption(QStringList() << "f" << "force");
    parser.addOption(forceOption);

    // Start parsing
    parser.process(*this);
    m_data.set_force_boot(parser.isSet(forceOption));

    // Initialize data
    status = m_data.initialize();
    if(status)
    {
        return status;
    }

    // connect signals
    connect(&m_timer, &QTimer::timeout, this, &application::work);
    connect(&m_timer, &QTimer::timeout, &m_widget, &widget::update_view);

    // log
    syslog(LOG_DEBUG, "Initialization ok");

    // start timer and widget
    m_widget.show();
    m_timer.start(100);

    // start event loop
    return exec();
}

/* ============================================================================
 *
 * */
void application::work()
{
    // check if there are some running operations
    const int nbop = m_data.get_smdata().nb_operation;
    if(nbop == 0)
    {
        hide_and_sleep();
    }
    else
    {
        monitor_operations();
    }
}

/* ============================================================================
 *
 * */
void application::hide_and_sleep()
{
    // log
    syslog(LOG_DEBUG, "Hide and sleep");

    // hide
    m_widget.hide();

    // wait for signal
    int sig;
    int result = sigwait(&m_sigset, &sig);

    // log
    syslog(LOG_DEBUG, "Wake up and show window");

    // show again
    m_widget.show();
}

/* ============================================================================
 *
 * */
void application::monitor_operations()
{
    time_t current = time(NULL);
    for(int x=0; x<WOOTFTPD_MAX_NBOP; x++)
    {
        // get operation
        wootftpd_operation& operation = m_data.get_smdata().operations[x];

        if( WOOTFTPD_CHECK_FLAG(operation.oflags, WOOTFTPD_ENABLE) 
        && !WOOTFTPD_CHECK_FLAG(operation.oflags, WOOTFTPD_END) 
        && !WOOTFTPD_CHECK_FLAG(operation.oflags, WOOTFTPD_TIMEOUT)
        )
        {
            if( m_timeout_array[x].first == operation.block )
            {
                // log
                syslog(LOG_DEBUG, "Operation %d same block twice", x);

                if( current >= m_timeout_array[x].second )
                {
                    time_t diff = current - m_timeout_array[x].second;

                    if( diff > 3 )
                    {
                        // timeout
                        operation.oflags |= WOOTFTPD_END;
                        operation.oflags |= WOOTFTPD_TIMEOUT;

                        // log
                        syslog(LOG_DEBUG, "Operation %d timeout", x);
                    }

                }
                else
                {
                    m_timeout_array[x].second = current;
                }
            }
            else
            {
                m_timeout_array[x].first = operation.block;
                m_timeout_array[x].second = current;
            }
        }

        // check if the operation is finished
        if( WOOTFTPD_CHECK_FLAG(operation.oflags, WOOTFTPD_END) && (m_end_array[x]==0) )
        {
            // set time end
            m_end_array[x] = current + m_data.get_end_time();

            // log
            syslog(LOG_DEBUG, "Operation %d finished, timeout at %ld + %d = %ld"
                , x, current, m_data.get_end_time(), m_end_array[x]);
        }

        // expiration du timer, delete operation
        if( (m_end_array[x]!=0) && (current>m_end_array[x]) )
        {
            // remove operation
            m_data.sem_lock();
            operation.oflags = WOOTFTPD_RESET;
            m_data.get_smdata().nb_operation--;
            m_end_array[x] = 0;
            m_data.sem_unlock();        

            // log
            syslog(LOG_DEBUG, "Operation %d removed at %ld/%ld, %d left"
                , x, current, m_end_array[x], m_data.get_smdata().nb_operation);
        }
    }
}
