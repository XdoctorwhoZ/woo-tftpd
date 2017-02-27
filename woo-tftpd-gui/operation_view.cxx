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
#include <QFile>
#include <QPainter>
#include <QGridLayout>

// WooTftpd
#include "operation_view.hpp"

// ---
using namespace std;
using namespace woo;
using namespace woo::tftpd;

int operation_view::spacing = 4;
QSize operation_view::wsize = QSize(500,64);

/* ============================================================================
 *
 * */
QVector<QPixmap> operation_view::images;

/* ============================================================================
 *
 * */
operation_view::operation_view(const wootftpd_operation& opdata, QWidget* qparent)
    : QWidget(qparent)
    , m_opdata(opdata)
{
    int icon_side = qMin(wsize.width(), wsize.height()) - 2*spacing;
    m_icon.setMinimumSize(icon_side,icon_side);
    m_icon.setMaximumSize(icon_side,icon_side);

    // Check static images initialization
    if( operation_view::images.isEmpty() )
    {
        initialize_images(icon_side);
    }

    // Set fix size
    setMinimumHeight(64);
    setMaximumHeight(64);

    // Compose layout
    QGridLayout*  lay = new QGridLayout(this);
    lay->addWidget(&m_icon              , 0, 0, 2, 1);
    lay->addWidget(&m_text_remote_ip    , 0, 1, 1, 1);
    lay->addWidget(&m_text_filename     , 0, 2, 1, 1);
    lay->addWidget(&m_progessbar        , 1, 1, 1, 2);
    lay->setSpacing(spacing);
    lay->setContentsMargins(spacing,spacing,spacing,spacing);

    QString style_0 = QString(
"QWidget {"  
        "background-color: #fdf6e3;"
        "}"
"QProgressBar {"
   " border: none;"
   "text-align: center;"
   "background-color: #93a1a1;"
    "font-family: \"ubuntu mono\";"
    "font-size: 16px;"
    "color: #FFFFFF;"
"}"
"QProgressBar::chunk {"
"    background-color: #2aa198;"
"    width: 20px;"
"}"
        )
        ;
        setStyleSheet(style_0);


    // text alignement        
    m_text_remote_ip.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_text_filename.setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    // First update
    update_view();
}

/* ============================================================================
 *
 * */
void operation_view::update_view()
{

    QString style_0 = QString(
        "QLabel {"  
        "font-family: \"ubuntu mono\";"
        "font-size: 16px;"
        "}"
        )
        ;

    QString ip_port = QString(m_opdata.ip) + ":" + QString::number(m_opdata.port);

    m_text_remote_ip.setText(ip_port);
    m_text_filename.setText(m_opdata.filename);


    m_text_filename.setStyleSheet(style_0);
    m_text_remote_ip.setStyleSheet(style_0);

    // Set progression
    m_progessbar.setRange(0, m_opdata.total_block);
    m_progessbar.setValue(m_opdata.block);


    // Display good icon
    if( WOOTFTPD_CHECK_FLAG(m_opdata.oflags, WOOTFTPD_END) )
    {
        if( WOOTFTPD_CHECK_FLAG(m_opdata.oflags, WOOTFTPD_TIMEOUT) )
        {
            m_icon.setPixmap(operation_view::images[image_id::failure]);
        }
        else
        {
            m_icon.setPixmap(operation_view::images[image_id::success]);
        }
    }
    else
    {
        if( WOOTFTPD_CHECK_FLAG(m_opdata.oflags, WOOTFTPD_UPLOAD) )
        {
            m_icon.setPixmap(operation_view::images[image_id::upload]);
        }
        else // WOOTFTPD_DOWNLOAD
        {
            m_icon.setPixmap(operation_view::images[image_id::download]);
        }
    }
}

/* ============================================================================
 *
 * */
void operation_view::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

/* ============================================================================
 *
 * */
void operation_view::initialize_image(int icon_side, const char* rsc_name)
{
    // Prepare empty image
    QImage image(icon_side, icon_side, QImage::Format_ARGB32);
    image.fill(0x00000000);

    // Prepare painter
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // Load file and associate it to svg renderer
    QFile file(rsc_name);
    file.open(QIODevice::ReadOnly);
    QByteArray baData = file.readAll();

    QSvgRenderer renderer;
    renderer.load(baData);
    renderer.render(&painter);

    // Append image to vector
    operation_view::images << QPixmap::fromImage(image);
}

/* ============================================================================
 *
 * */
void operation_view::initialize_images(int icon_side)
{
    initialize_image(icon_side, ":/upload");    // send file
    initialize_image(icon_side, ":/download");  // receive file
    initialize_image(icon_side, ":/success");
    initialize_image(icon_side, ":/failure");
}
