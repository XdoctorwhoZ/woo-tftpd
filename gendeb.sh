# Set script right
sudo chmod +x package/DEBIAN/postinst

# Create package
dpkg -b package/ woo-tftpd_1.0.0_i386.deb
