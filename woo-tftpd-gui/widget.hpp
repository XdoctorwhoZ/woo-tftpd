#ifndef WOOTFTPD_WIDGET_HPP
#define WOOTFTPD_WIDGET_HPP
//!
//! \file widget.hpp
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
#include <QVBoxLayout>
#include <QDesktopWidget>

// WooTftpd
#include "data.hpp"
#include "operation_view.hpp"

// ---
namespace woo { namespace tftpd {

//! Class that manage the operation visualization
//!
class widget : public QWidget
{
    Q_OBJECT

    //! Application data
    //!
    tftpd::data& m_data;

    //! Layout of this widget
    //!
    QVBoxLayout* m_layout;

    //! Number of enabled item
    //!
    int m_nb_enables_items;

    //! One item by operation
    //!
    QVector<QSharedPointer<operation_view> > m_items;

public:

    //! Constructor
    //!
    widget(tftpd::data& dat, QWidget* qparent = 0);

    //! Destructor
    //!
    ~widget();

public slots:

    //! Take settings from data to apply to widget
    //!
    void setup();

    //! Update the view element
    //!
    void update_view();

private:

    //! Adjust geometry
    //!
    void adjust_geometry();

    //! Create item for operation slot 'x'
    //!
    void create_item(int x);

    //! Delete item for operation slot 'x'
    //!
    void delete_item(int x);

};

} // tftpd
} // woo

#endif // WOOTFTPD_WIDGET_HPP
