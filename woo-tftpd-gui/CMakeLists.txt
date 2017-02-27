# =============================================================================
# Copyright 2016 XdoctorwhoZ
# This file is part of WooTftpd.

# WooTftpd is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# WooTftpd is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with WooTftpd.  If not, see <http://www.gnu.org/licenses/>.
# =============================================================================

# Initialize cmake project

MESSAGE("[ in] ${CMAKE_CURRENT_LIST_FILE}")

# Configure Qt package

FIND_PACKAGE(Qt5 REQUIRED Core Widgets Svg Xml)
SET( CMAKE_AUTOMOC ON )
ADD_DEFINITIONS(
    ${Qt5Core_DEFINITIONS}
    ${Qt5Widgets_DEFINITIONS}
    )
INCLUDE_DIRECTORIES(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    )
QT5_ADD_RESOURCES(
    RESOURCE_ADDED ${CMAKE_CURRENT_LIST_DIR}/rsc/list.qrc
    )

# Configure executable

SET(LIST_SOURCE
    main.cxx
    data.cxx
    widget.cxx
    application.cxx
    operation_view.cxx
    ${RESOURCE_ADDED}
    )

ADD_EXECUTABLE(
    woo-tftpd-gui
    ${LIST_SOURCE}
    )

SET_PROPERTY(
    TARGET   woo-tftpd-gui
    PROPERTY CXX_STANDARD 14
    )

TARGET_LINK_LIBRARIES(
    woo-tftpd-gui
    -lpthread -lrt
    )

QT5_USE_MODULES(
    woo-tftpd-gui
    Core Widgets Svg
    )

# end of file
MESSAGE("[out] ${CMAKE_CURRENT_LIST_FILE}")
