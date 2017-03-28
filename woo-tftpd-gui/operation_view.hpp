#ifndef WOOTFTPD_OPERATION_VIEW_HPP
#define WOOTFTPD_OPERATION_VIEW_HPP
//!
//! \file operation_view.hpp
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
#include <QLabel>
#include <QWidget>
#include <QPixmap>
#include <QtGlobal>
#include <QProgressBar>
#include <QStyleOption>

// std
#include <iostream>

// WooTftpd
#include "common.h"
#include "data.hpp"

// ---
namespace woo { namespace tftpd {

//! Class that manage the operation visualization
//!
class operation_view : public QWidget
{
    Q_OBJECT

    //! Id for each image
    //!
    enum image_id
    {
          upload      = 0
        , download    = 1
        , success     = 2
        , failure     = 3
        , number      = 4
    };

    //! Images that can be loaded
    //!
    static QVector<QPixmap> images;

    //! Data about the operation
    //!
    const wootftpd_operation& m_opdata;

    //! Icon
    //!
    QLabel m_icon;

    //! Label to display filename
    //!
    QLabel m_text_filename;

    //! Label to display ip of the tftp client
    //!
    QLabel m_text_remote_ip;

    //! Widget that will be loaded with a progress bar or a label
    //! It will depend on the operation type (upload or download)
    //!
    QWidget m_contextual_box;

    //! Layout of the contextual box
    //!
    QGridLayout* m_lay_box;

    //! Label to display transfert evolution in download (size unkown)
    //!
    QLabel* m_progesslab;

    //! Progress bar to display transfert evolution in upload (size known)
    //!
    QProgressBar* m_progessbar;

    // ===

    //! Space beetween widget
    //!
    static int spacing;

    //! Widget requested size
    //!
    static QSize wsize;

public:

    //! Constructor
    //!
    operation_view(const wootftpd_operation& opdata, QWidget* qparent = 0);

public slots:

    //! Update the view element with the data
    //!
    void update_view();

protected:

    //! From QWidget
    //!
    void paintEvent(QPaintEvent*);

private:

    //! Initialize static images used by all widgets
    //!
    static void initialize_images();

};

} // tftpd
} // woo

#endif // WOOTFTPD_OPERATION_VIEW_HPP
