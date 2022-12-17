#! /bin/bash

cd /home/ubuntu
mkdir install


sudo apt-get install -y libavformat-dev libswscale-dev libpq-dev
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

sudo make install > /home/ubuntu/install/freeswitch.install.log

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
sudo chmown -R
sudo wget https://eliassun.github.io/freeswitch.service
sudo cp freeswitch.service /etc/systemd/system/freeswitch.service
sudo chmod ugo+x /etc/systemd/system/freeswitch.service
sudo systemctl start freeswitch.service
sudo systemctl enable freeswitch.service
sudo systemctl status freeswitch.service > freeswitch.status

sudo git clone https://github.com/eliassun/WebRTC.git
sudo cp ./WebRTC/conf/vars.xml /usr/local/freeswitch/conf/vars.xml
sudo cp ./WebRTC/conf/autoload_configs/verto.conf.xml /usr/local/freeswitch/conf/autoload_configs/verto.conf.xml
sudo cp ./WebRTC/conf/sip_profiles/internal.xml /usr/local/freeswitch/conf/sip_profiles/internal.xml
sudo cp ./WebRTC/conf/conf/sip_profiles/external.xml /usr/local/freeswitch/conf/sip_profiles/external.xml
sudo cp ./WebRTC/conf/autoload_configs/switch.conf.xml /usr/local/freeswitch/conf/autoload_configs/switch.conf.xml

sudo systemctl restart freeswitch.service
sudo systemctl status freeswitch.service > freeswitch.status.2

sudo apt-get install -y coturn
sudo cp /etc/turnserver.conf /etc/turnserver.conf.backup
cd /home/ubuntu/
mkdir coturn
cd coturn
echo listening-ip=0.0.0.0 >> turnserver.conf
echo external-ip=$(curl -s http://checkip.amazonaws.com) >> turnserver.conf
echo syslog >> turnserver.conf
sudo cp turnserver.conf /etc/turnserver.conf
sudo systemctl start coturn
sudo systemctl status coturn > coturn.status
sudo lsof -n -P -i > ports.log

sudo echo "Done" > /home/ubuntu/install/done.log