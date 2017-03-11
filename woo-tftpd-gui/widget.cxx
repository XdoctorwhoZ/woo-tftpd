//!
//! \file widget.cxx
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
#include "widget.hpp"

// ---
using namespace std;
using namespace woo;
using namespace woo::tftpd;

/* ============================================================================
 *
 * */
widget::widget(tftpd::data& dat, QWidget* qparent)
    : QWidget(qparent)
    , m_data(dat)
    , m_layout(new QVBoxLayout(this))
    , m_nb_enables_items(0)
    , m_items(WOOTFTPD_MAX_NBOP)
{
    // connect events
    connect(&m_data, &data::settings_loaded, this, &widget::setup);

    // configure layout 
    m_layout->setSpacing(0);
    m_layout->setContentsMargins(0,0,0,0);
    setMinimumWidth(m_data.get_widget_width());
    setMaximumWidth(m_data.get_widget_width());

    setAttribute(Qt::WA_ShowWithoutActivating);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint);
}

/* ============================================================================
 *
 * */
widget::~widget()
{ }

/* ============================================================================
 *
 * */
void widget::setup()
{

    // Compute position
    QPoint position = m_data.get_widget_position();

    // Compute other screen position
    QDesktopWidget dw;
    int screen = m_data.get_widget_screen();
    if( (screen >= 0) && (screen < dw.screenCount()) )
    {
        position += dw.screenGeometry(screen).topLeft();
    }

    // Move widget to the correct location
    move(position);
}

/* ============================================================================
 *
 * */
void widget::update_view()
{
    // Creation / Destruction loop
    for(int x=0; x<WOOTFTPD_MAX_NBOP; x++)
    {
        // get operation slot
        const wootftpd_operation& operation = m_data.get_smdata().operations[x];

        // Must be displayed?
        //  -> if enable and no item already created
        if( WOOTFTPD_CHECK_FLAG(operation.oflags, WOOTFTPD_ENABLE) && !m_items[x] )
        {
            create_item(x);
        }

        // Must be removed from display?
        if( !WOOTFTPD_CHECK_FLAG(operation.oflags, WOOTFTPD_ENABLE) && m_items[x] )
        {
            delete_item(x);
        }
    }

    // Update loop
    for( auto& item : m_items )
    {
        if(item)
        {
            item->update_view(); 
        }
    }
}

/* ============================================================================
 *
 * */
void widget::adjust_geometry()
{
    setMinimumHeight(m_nb_enables_items * m_data.get_widget_item_height());
    setMaximumHeight(m_nb_enables_items * m_data.get_widget_item_height());
}

/* ============================================================================
 *
 * */
void widget::create_item(int x)
{
    // log
    // log::debug() << "[widget] create item " << x;

    // Create the item
    QSharedPointer<operation_view> new_item(
        new operation_view(
            m_data.get_smdata().operations[x]
            )
        );

    // Append item to the view
    m_layout->addWidget( new_item.data() );

    // Append item to the management vector
    m_items[x] = new_item;

    // Count it
    m_nb_enables_items++;
    adjust_geometry();
}

/* ============================================================================
 *
 * */
void widget::delete_item(int x)
{
    // log
    // log::debug() << "[widget] delete item " << x;

    // destroy item
    m_items[x].reset(0);

    // Count it
    m_nb_enables_items--;
    adjust_geometry();
}

