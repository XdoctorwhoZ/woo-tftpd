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
#include <QSvgRenderer>
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

    //! Label to display file transfered
    //!
    QLabel m_text;

    //!
    //!
    QLabel m_text_filename;

    //!
    //!
    QLabel m_text_remote_ip;

    //! Progress bar to display transfert evolution
    //!
    QProgressBar m_progessbar;

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

    //! Initialize just one image
    //!
    static void initialize_image(int icon_side, const char* rsc_name);

    //! Initialize static images used by all widgets
    //!
    static void initialize_images(int icon_side);

};

} // tftpd
} // woo

#endif // WOOTFTPD_OPERATION_VIEW_HPP
