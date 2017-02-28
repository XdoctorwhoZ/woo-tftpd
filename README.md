WOO-TFTPD
=========


## Build process


sudo apt-get install cmake qtbase5-dev libqt5svg5-dev

mkdir build_tftpd

cd build_tftpd

cmake ../woo-tftpd

make

# Install

sudo cp woo-tftpd-server/woo-tftpd-server /usr/sbin/woo-tftpd-server 

sudo cp woo-tftpd-gui/woo-tftpd-gui  /usr/bin/woo-tftpd-gui 

sudo cp package/etc/xinetd.d/woo-tftpd-server /etc/xinetd.d/woo-tftpd-server

delete previous tftp config in xinetd.d or disable it



## Work in progress


cmake version 2.8.12.2


dpkg -b package/  woo-tftpd_0.1.0_i386.deb




Fichier de configuration

file:/etc/woo-tftpd.conf

[operation]
end_time=3

[widget]
position=@Point(64 64)
screen=0



Integration dans systemd?
/lib/systemd/system/woo-tftpd-gui.service
