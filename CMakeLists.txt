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

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)
PROJECT(WooTftpd C CXX)
MESSAGE("[ in] ${CMAKE_CURRENT_LIST_FILE}")

# Include common configuration

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR})

# Compile server and gui

ADD_SUBDIRECTORY(woo-tftpd-gui)
ADD_SUBDIRECTORY(woo-tftpd-server)

# Installation

INSTALL(
    DIRECTORY   ${CMAKE_CURRENT_LIST_DIR}/package
    DESTINATION ${PROJECT_BINARY_DIR}
    )

INSTALL(
    FILES       ${CMAKE_CURRENT_LIST_DIR}/gendeb.sh
    DESTINATION ${PROJECT_BINARY_DIR}
    )

INSTALL(
    FILES       ${CMAKE_CURRENT_LIST_DIR}/package/etc/xinetd.d/woo-tftpd-server
    DESTINATION /etc/xinetd.d
    )

INSTALL(
    FILES       ${CMAKE_CURRENT_LIST_DIR}/package/etc/X11/xinit/xinitrc.d/woo-tftpd-gui
    DESTINATION /etc/X11/xinit/xinitrc.d
    )

# end of file

MESSAGE("[out] ${CMAKE_CURRENT_LIST_FILE}")
