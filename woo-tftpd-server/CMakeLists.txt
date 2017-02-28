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

# Include root include directory

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR})

# Set sources files


SET(LIST_SOURCE
    data.c
    lib/dup2.c
    lib/daemon.c
    lib/xstrdup.c
    lib/inet_ntop.c
    lib/bsdsignal.c
    lib/getopt_long.c
    lib/getaddrinfo.c
    lib/xmalloc.c
    tftpd/misc.c
    tftpd/tftpd.c
    tftpd/remap.c
    tftpd/recvfrom.c
    common/tftpsubs.c
    )

ADD_EXECUTABLE(
    woo-tftpd-server
    ${LIST_SOURCE}
    )

TARGET_LINK_LIBRARIES(
    woo-tftpd-server
        -pthread   -lrt
    )

# Install

INSTALL(
    TARGETS     woo-tftpd-server
    DESTINATION ${PROJECT_BINARY_DIR}/package/usr/sbin
    )

INSTALL(
    TARGETS     woo-tftpd-server
    DESTINATION /usr/sbin
    )

# end of file

MESSAGE("[out] ${CMAKE_CURRENT_LIST_FILE}")
