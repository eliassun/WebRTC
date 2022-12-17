#! /bin/bash

cd /home/ubuntu
mkdir install

sudo apt-get install -y libavformat-dev
sudo apt-get install -y libswscale-dev
sudo apt-get install -y libpq-dev
sudo apt -y update
sudo apt install -y git subversion build-essential autoconf automake libtool libncurses5 libncurses5-dev make libjpeg-dev libtool libtool-bin libsqlite3-dev libpcre3-dev libspeexdsp-dev libldns-dev libedit-dev yasm liblua5.2-dev libopus-dev cmake
sudo apt install -y libcurl4-openssl-dev libexpat1-dev libgnutls28-dev libtiff5-dev libx11-dev unixodbc-dev libssl-dev python-dev zlib1g-dev libasound2-dev libogg-dev libvorbis-dev libperl-dev libgdbm-dev libdb-dev uuid-dev libsndfile1-dev

cd /usr/src
sudo git clone https://github.com/signalwire/libks.git
cd libks
sudo cmake .
sudo make > /home/ubuntu/install/libks.make.log
sudo make install

cd /usr/src
sudo git clone https://github.com/signalwire/signalwire-c.git
cd signalwire-c
sudo cmake .
sudo make > /home/ubuntu/install/signalwire-c.make.log
sudo make install

cd /usr/src
sudo wget https://files.freeswitch.org/freeswitch-releases/freeswitch-1.10.3.-release.zip
sudo apt -y install unzip
sudo unzip freeswitch-1.10.3.-release.zip
cd freeswitch-1.10.3.-release/

sudo ./configure -C > /home/ubuntu/install/freeswitch.configure-c.log
sudo make >  /home/ubuntu/install/freeswitch.make.log

sudo make install

sudo make all cd-sounds-install cd-moh-install >  /home/ubuntu/install/sound.make.log
sudo ln -s /usr/local/freeswitch/bin/freeswitch /usr/bin/
sudo ln -s /usr/local/freeswitch/bin/fs_cli /usr/bin
cd /usr/local

sudo groupadd freeswitch
sudo adduser --disabled-password  --quiet --system --home /usr/local/freeswitch --gecos "FreeSWITCH Voice/Video Service" --ingroup freeswitch freeswitch
sudo chown -R freeswitch:freeswitch /usr/local/freeswitch/
sudo chmod -R ug=rwX,o= /usr/local/freeswitch/
sudo chmod -R u=rwx,g=rx /usr/local/freeswitch/bin/

cd  /home/ubuntu/install
sudo wget https://eliassun.github.io/freeswitch.service
sudo cp freeswitch.service /etc/systemd/system/freeswitch.service
sudo chmod ugo+x /etc/systemd/system/freeswitch.service
sudo systemctl start freeswitch.service
sudo systemctl enable freeswitch.service
sudo systemctl status freeswitch.service > freeswitch.status

