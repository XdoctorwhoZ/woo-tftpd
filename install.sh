
cp -fv ./package/usr/bin/woo-tftpd-gui /usr/bin/woo-tftpd-gui
cp -fv ./package/usr/sbin/woo-tftpd-server /usr/sbin/woo-tftpd-server
cp -fv ./package/etc/woo-tftpd/.config/autostart/woo-tftpd-gui.desktop ~/.config/autostart/woo-tftpd-gui.desktop
cp -fv ./package/etc/xinetd.d/woo-tftpd-server  /etc/xinetd.d/woo-tftpd-server
cp -fv ./package/etc/woo-tftpd.conf  /etc/woo-tftpd.conf
