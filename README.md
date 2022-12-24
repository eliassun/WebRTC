# WebRTC
This project will automate a voice/video service based on FreeSWITCH by Terraform, including:

* Install FreeSWITCH from source code

* Make FreeSWITCH configured and be ready to serve on AWS

* Install and enable Coturn for STUN service

* Configure security group for SIP, RTP, STUN, http, https, WebSocket and SSH

* Enable Verto WebRTC client on FreeSWITCH: https://eliassun.github.io/freeswitch_webrtc.txt 

More:

* Instruction to install FreeSWITCH from source code manually
https://eliassun.github.io/freeswitch_setup.txt

* Instruction to configure FreeSWITCH NAT Traversal, STUN/TURN and Debug manually
https://eliassun.github.io/freeswitch_sip.txt

The automation process will take about 35 minutes to finish. During the installation, it will keep output logs 
saved into /home/ubuntu/install. The last log name will be done.log. Finally, check the ports.log to see if the 
freeswitch and coturn services are in service state.

The automation process doesn't install the Verto WebRTC client, but the detail instruction is at https://eliassun.github.io/freeswitch_webrtc.txt .

* sip4webrtc
This is to build a native App based on C++ SIP lib and C++ WebRTC lib. sip4webrtc is still an early stage project.
It currently built the PJSIP lib and WebRTC lib for the Android.

* picap for MacOS
The detail is https://eliassun.github.io/daddyeye.html

DaddyEye is a peer to peer screenshot capture command line software. NO Server Storage. DIFFERENT ID for every new connection.

It has two modes: dad and sweet.

Build:

./buid.sh

Run:

Run on dad mode
./picap

It will print a dad id as soon as it starts.

Run on sweet mode:

./picap -o sweet

./picap -o sweet -i dad_id

They will print a sweet id as soon as they start.

The first command line will allow any dad to capture the screen if he knows the sweet's id.

The 2nd command line will only allow one dad with the exact id to to capture the sweet's screen. So, the dad end needs to run first.

* Start to create a network

Add the AWS keys into vairable.tf or put the keys file to the correct location

terraform init

terraform apply --auto-approve

