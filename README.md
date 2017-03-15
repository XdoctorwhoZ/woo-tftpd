WOO-TFTPD
=========


## Build process


sudo apt-get install cmake qtbase5-dev libqt5svg5-dev

mkdir build_tftpd

cd build_tftpd

cmake ../woo-tftpd

sudo make install


# Install

delete previous tftp config in xinetd.d or disable it

## Work in progress


cmake version 2.8.12.2






Fichier de configuration

file:/etc/woo-tftpd.conf

[operation]
end_time=3

[widget]
position=@Point(64 64)
screen=0



Integration dans systemd?
/lib/systemd/system/woo-tftpd-gui.service
