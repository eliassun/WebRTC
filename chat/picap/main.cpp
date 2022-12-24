/*
   main.cpp
   Copyright (C) 2022-2028 Elias Sun
*/

#include "rtc/rtc.hpp"

#include "parse_cl.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <chrono>
#include <future>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include <thread>
#include <unordered_map>
#include <thread>
#include <queue>
#include <unistd.h>
#include <nlohmann/json.hpp>
#include "capture.h"

using json = nlohmann::json;
using namespace std::chrono_literals;
using std::shared_ptr;
using std::weak_ptr;
template <class T> weak_ptr<T> make_weak_ptr(shared_ptr<T> ptr) { return ptr; }

using nlohmann::json;

std::string localId;
std::unordered_map<std::string, shared_ptr<rtc::PeerConnection>> peerConnectionMap;
std::unordered_map<std::string, shared_ptr<rtc::DataChannel>> dataChannelMap;

shared_ptr<rtc::PeerConnection> createPeerConnection(const rtc::Configuration &config,
                                                     weak_ptr<rtc::WebSocket> wws, std::string id);
std::string randomId(size_t length);
void sendScreen();

#define MODE_MASTER "dad"
#define MODE_SLAVE "sweet"

#define SEND_START  "send_start"
#define SEND_END "send_end"
#define SEND_CAPTURE "getscreen"
#define LOCAL_OPEN_IMG "open"

std::string greceivedBuf = "";
std::shared_ptr<rtc::DataChannel> gdc;
std::string gmode = MODE_SLAVE;
std::string gdadid = "";

bool gdebug = false;

int main(int argc, char **argv) try {
	Cmdline params(argc, argv);

	rtc::InitLogger(rtc::LogLevel::Info);

	rtc::Configuration config;
	std::string stunServer = "";
	gmode = params.mode();
	if (gmode != MODE_SLAVE && gmode != MODE_MASTER) {
	    std::cout << "Run ./picap for dad mode. Run ./picap sweet for sweet mode." << std::endl;
	    params.usage(EXIT_SUCCESS);
	    return 0;
	}
	if (gmode == MODE_SLAVE) {
	    gdadid = params.dadid();
	}
    std::cout << "Mode: " << gmode << std::endl;
	if (params.noStun()) {
		std::cout
		    << "No STUN server is configured. Only local hosts and public IP addresses supported."
		    << std::endl;
	} else {
		if (params.stunServer().substr(0, 5).compare("stun:") != 0) {
			stunServer = "stun:";
		}
		stunServer += params.stunServer() + ":" + std::to_string(params.stunPort());
		std::cout << "STUN server is " << stunServer << std::endl;
		config.iceServers.emplace_back(stunServer);
	}

	if (params.udpMux()) {
		std::cout << "ICE UDP mux enabled" << std::endl;
		config.enableIceUdpMux = true;
	}

	localId = randomId(8);
	if (gmode == std::string(MODE_MASTER)) {
	    std::cout << "*********** dad ID ***********" << std::endl;
	    std::cout << localId << std::endl;
	    std::cout << "On the sweet's screen, run ./piap -o sweet -i dad_id_above ." << std::endl;
	    std::cout << "Then it will only accept the command from this dad. Or it will accept a command from any dad." << std::endl;
	    std::cout << "*********************************" << std::endl;
	} else {
	    std::cout << "*********** sweet ID ***********" << std::endl;
	    std::cout << localId << std::endl;
	    std::cout << "Please copy sweet ID to dad!!!" << std::endl;
	    std::cout << "*********************************" << std::endl;
	}

	auto ws = std::make_shared<rtc::WebSocket>();

	std::promise<void> wsPromise;
	auto wsFuture = wsPromise.get_future();

	ws->onOpen([&wsPromise]() {
	    if (gdebug) {
	        std::cout << "WebSocket connected, signaling ready" << std::endl;
	    }
		wsPromise.set_value();
	});

	ws->onError([&wsPromise](std::string s) {
		std::cout << "WebSocket error" << std::endl;
		wsPromise.set_exception(std::make_exception_ptr(std::runtime_error(s)));
	});

	ws->onClosed([]() { std::cout << "WebSocket closed" << std::endl; });

	ws->onMessage([&config, wws = make_weak_ptr(ws)](auto data) {
		// data holds either std::string or rtc::binary
		if (!std::holds_alternative<std::string>(data))
			return;

		json message = json::parse(std::get<std::string>(data));

		auto it = message.find("id");
		if (it == message.end())
			return;

		auto id = it->get<std::string>();

		it = message.find("type");
		if (it == message.end())
			return;

		auto type = it->get<std::string>();

		shared_ptr<rtc::PeerConnection> pc;
		if (auto jt = peerConnectionMap.find(id); jt != peerConnectionMap.end()) {
			pc = jt->second;
		} else if (type == "offer") {
		    if (gdebug) {
			    std::cout << "Answering to " + id << std::endl;
			}
			pc = createPeerConnection(config, wws, id);
		} else {
			return;
		}

		if (type == "offer" || type == "answer") {
			auto sdp = message["description"].get<std::string>();
			pc->setRemoteDescription(rtc::Description(sdp, type));
		} else if (type == "candidate") {
			auto sdp = message["candidate"].get<std::string>();
			auto mid = message["mid"].get<std::string>();
			pc->addRemoteCandidate(rtc::Candidate(sdp, mid));
		}
	});

	const std::string wsPrefix =
	    params.webSocketServer().find("://") == std::string::npos ? "ws://" : "";
	const std::string url = wsPrefix + params.webSocketServer() + ":" +
	                        std::to_string(params.webSocketPort()) + "/" + localId;

    if (gdebug) {
	    std::cout << "WebSocket URL is " << url << std::endl;
	}
	ws->open(url);

	std::cout << "Waiting for signaling to be connected..." << std::endl;
	wsFuture.get();
	if (gmode == std::string(MODE_SLAVE)) {
	    std::cout << "Sweet Mode. https://eliassun.github.io/daddyeye.html for more information." << std::endl;
	    while(true) {
	        sleep(1);
	    }
	}
	//while (true)
	//{
		std::string id;
		std::cout << "Enter sweet id:" << std::endl;
		std::cin >> id;
		std::cin.ignore();

		if (id.empty())
		{
			std::cout << "id is empty. End... " << "Cleaning up..." << std::endl;
			dataChannelMap.clear();
			peerConnectionMap.clear();
			return 0;
		}
			//break;

		if (id == localId) {
			std::cout << "Invalid remote ID (This is the local ID)" << std::endl;
			dataChannelMap.clear();
			peerConnectionMap.clear();
			return 0;
			//continue;
		}
        if (gdebug) {
		    std::cout << "Offering to " + id << std::endl;
		}
		auto pc = createPeerConnection(config, ws, id);

		// We are the offerer, so create a data channel to initiate the process
		const std::string label = "dad";
		std::cout << "Creating DataChannel with label \"" << label << "\"" << std::endl;
		gdc = pc->createDataChannel(label);
		auto dc = gdc;
		dc->onOpen([id, wdc = make_weak_ptr(dc)]() {
		    if (gdebug) {
			    std::cout << "DataChannel from " << id << " open" << std::endl;
			}
			if (auto dc = wdc.lock())
				dc->send("Hello from " + localId);
		});

		dc->onClosed([id]() { std::cout << "DataChannel from " << id << " closed" << std::endl; });

		dc->onMessage([id, wdc = make_weak_ptr(dc), dc](auto data) {
		    if (std::holds_alternative<std::string>(data)) {
		        if (gdebug) {
		            std::cout << "Message from " << id << " received: " << std::get<std::string>(data) << std::endl;
		        }
			    if (std::get<std::string>(data) == std::string(SEND_START)) {
			        std::cout << "Receiving screenshot..." << std::endl;
			        greceivedBuf.clear();
			    } else if (std::get<std::string>(data) == std::string(SEND_END)) {
			        std::cout << "Decoding a screen" << std::endl;
			        decode_image(greceivedBuf);
			        std::cout << "Decoded a screen" << std::endl;
			    } else {
			        greceivedBuf += std::get<std::string>(data);
			    }
		   } else {
                std::cout << "Binary message from " << id
                  << " received, size=" << std::get<rtc::binary>(data).size() << std::endl;
		   }

		});

		dataChannelMap.emplace(id, dc);
	//}
    if (gmode == std::string(MODE_MASTER)) {
        std::string msg = "";
        while (msg != "quit") {
            std::cout << "*************************************************************" << std::endl;
            std::cout << "In Dad mode, waiting for a command from keyboard." << std::endl;
            std::cout << "getscreen: get a sweet screenshot back." << std::endl;
            std::cout << "open: open the screenshot after getscreen is done." << std::endl;
            std::cout << "Enter a command on screen's anywhere from the key board." << std::endl;
            std::cout << "**************************************************************" << std::endl;
            std::cin >> msg;
            std::cin.ignore();
            if (msg == std::string(SEND_CAPTURE)) {
                msg = "{\"type\": \"" + msg + "\", \"id\": " + "\"" + localId + "\"}";
                dc->send(msg);
            } else if (msg == std::string(LOCAL_OPEN_IMG)) {
                open_image();
            } else {
                std::cout << "getscreen: capture a screenshot of the peer" << std::endl;
                std::cout << "open: open the recent captured screenshot of the peer" << std::endl;
            }
        }
    } else {
        std::cout << "In Salve mode, waiting for event from peer." << std::endl;
        while (true) {
            sleep(1);
        }

    }

	std::cout << "Cleaning up..." << std::endl;

	dataChannelMap.clear();
	peerConnectionMap.clear();
	return 0;

} catch (const std::exception &e) {
	std::cout << "Error: " << e.what() << std::endl;
	dataChannelMap.clear();
	peerConnectionMap.clear();
	return -1;
}

// Create and setup a PeerConnection
shared_ptr<rtc::PeerConnection> createPeerConnection(const rtc::Configuration &config,
                                                     weak_ptr<rtc::WebSocket> wws, std::string id) {
	auto pc = std::make_shared<rtc::PeerConnection>(config);

	pc->onStateChange(
	    [](rtc::PeerConnection::State state) { std::cout << "State: " << state << std::endl; });

	pc->onGatheringStateChange([](rtc::PeerConnection::GatheringState state) {
		std::cout << "Gathering State: " << state << std::endl;
	});

	pc->onLocalDescription([wws, id](rtc::Description description) {
		json message = {{"id", id},
		                {"type", description.typeString()},
		                {"description", std::string(description)}};

		if (auto ws = wws.lock())
			ws->send(message.dump());
	});

	pc->onLocalCandidate([wws, id](rtc::Candidate candidate) {
		json message = {{"id", id},
		                {"type", "candidate"},
		                {"candidate", std::string(candidate)},
		                {"mid", candidate.mid()}};

		if (auto ws = wws.lock())
			ws->send(message.dump());
	});

	pc->onDataChannel([id](shared_ptr<rtc::DataChannel> dc) {
		std::cout << "DataChannel from " << id << " received with label \"" << dc->label() << "\""
		          << std::endl;

		dc->onOpen([wdc = make_weak_ptr(dc)]() {
			if (auto dc = wdc.lock())
				dc->send("{\"type\": \"feedback\", \"msg\": \"Hello from the sweet " + localId + "\"}");
		});

		dc->onClosed([id]() { std::cout << "DataChannel from " << id << " closed" << std::endl; });

		dc->onMessage([id, wdc = make_weak_ptr(dc)](auto data) {
		    if (std::holds_alternative<std::string>(data)) {
		        std::string recv = std::get<std::string>(data);
		        int len = recv.length();
                std::cout << "Message from " << id << " received: " << std::get<std::string>(data) << std::endl;
                std::string msgType = "";
                std::string dadID = "";
                if (len > 5 && recv[0] == '{') {
                    auto cmd = json::parse(std::get<std::string>(data));
                    msgType = cmd["type"];
                    dadID = cmd["id"];
                }
                if (msgType == std::string(SEND_CAPTURE)) {
                    if ((gdadid.length() > 0 && dadID == gdadid) || (gdadid.length() == 0)) {
                        if (auto dc = wdc.lock()) {
                            dc->send("{\"type\": \"feedback\", \"msg\": \"Received a screenshot capture req. doing...\"}");
                            std::cout << "sending a screen" << std::endl;
                            std::string screen = capture_screen();
                            dc->send(std::string(SEND_START));
                            int n = screen.length()/512;
                            int pos = 0;
                            for (int i = 0; i < n; i++) {
                                dc->send(screen.substr(pos, 512));
                                pos += 512;
                            }
                            if (screen.length() - pos > 0) {
                                dc->send(screen.substr(pos, screen.length() - pos));
                            }
                            dc->send(std::string(SEND_END));
                            std::cout << "sent a screen" << std::endl;
                        }
			        } else {
			            std::cout << "Expected dadid: " << gdadid << " Received dadid: " << dadID << std::endl;
			        }
			    }
		   } else {
                std::cout << "Binary message from " << id
                  << " received, size=" << std::get<rtc::binary>(data).size() << std::endl;
		   }
		});
		dataChannelMap.emplace(id, dc);
	});

	peerConnectionMap.emplace(id, pc);
	return pc;
};

// Helper function to generate a random ID
std::string randomId(size_t length) {
	using std::chrono::high_resolution_clock;
	static thread_local std::mt19937 rng(
	    static_cast<unsigned int>(high_resolution_clock::now().time_since_epoch().count()));
	static const std::string characters(
	    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	std::string id(length, '0');
	std::uniform_int_distribution<int> uniform(0, int(characters.size() - 1));
	std::generate(id.begin(), id.end(), [&]() { return characters.at(uniform(rng)); });
	return id;
}

void sendScreen() {
    std::cout << "sending a screen" << std::endl;
    std::string screen = capture_screen();
    gdc->send(std::string(SEND_START));
    int n = screen.length()/512;
    int pos = 0;
    for (int i = 0; i < n; i++) {
        gdc->send(screen.substr(pos, 512));
        pos += 512;
    }
    if (screen.length() - pos > 0) {
        gdc->send(screen.substr(pos, screen.length() - pos));
    }
    gdc->send(std::string(SEND_END));
    std::cout << "sent a screen" << std::endl;
}
