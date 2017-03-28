# Set script right
sudo chmod +x package/DEBIAN/postinst

# Create package
dpkg -b package/ woo-tftpd_1.1.0_amd64.deb
