#!/bin/sh
set -e
basepath=`dirname $(pwd)/$0`
cd $HOME
install_dir="$basepath"
cd $install_dir
unzip lcm-1.3.1.zip
cd $install_dir/lcm-1.3.1
sudo cp /usr/share/misc/config* .
sudo ./configure
sudo make -j4
sudo make install	
echo "lcm successful completion"
cd $install_dir
unzip glog-master.zip
cd $install_dir/glog-master
sudo cp /usr/share/misc/config* .
sudo ./configure
sudo make -j4
sudo make install
echo "glog successful completion"
cd $install_dir
tar -xzvf libsodium-1.0.3.tar.gz
cd $install_dir/libsodium-1.0.3
sudo cp /usr/share/misc/config* .
sudo ./configure
sudo make -j4
sudo make install
echo "libsodium successful completion"
cd $install_dir
tar -xzvf zeromq-4.1.2.tar.gz
cd $install_dir/zeromq-4.1.2
sudo ./configure
sudo make -j4
sudo make install
cd /usr/local/lib
sudo ldconfig
cd $install_dir
echo "zmq successful completion"
