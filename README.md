WOO-TFTPD
=========

This project provides a server tftp and a gui to visualize tftp operations performed by the server. The server is based on tftpd-hpa.

## Build process

Easy with cmake

```Shell
sudo apt-get install cmake qtbase5-dev
mkdir build_tftpd
cd build_tftpd
cmake ../woo-tftpd
make -j
sudo make install
``` 

# Install

To install the woo-tftpd from the directory build_tftpd

```Shell
sudo chmod +x install.sh 
./install.sh
``` 

This will install files in your computer but the server is disabled by default.
Before enabling it, first disable any previous tftp server installation.

Then open the file /etc/xinet.d/woo-tftpd-server and set ‘disable’ parameter to the value ‘no’

```Shell
sudo service xinetd restart
```

Finaly enter in terminal

```Shell
sudo woo-tftpd-gui -f
```

# Configuration file

The configuration file is located at /etc/woo-tftpd.conf

### option: [operation][end_time]

This is the number of seconds between the end of the operation and the deletion of the widget that display this operation. 3 seconds by default.

### option: [operation][position]

This is the position of the widget relative to the top left corner of the selected screen

### option: [operation][screen]

This is the index of the selected screen. 0 for the main screen, 1 for the second...
