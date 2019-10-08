#ifndef __MQTT_GATEWAY__
#define ____MQTT_GATEWAY____

#include <Ethernet.h>
#include <PubSubClient.h>
#include <Sensor.h>
#include <Utils.h>

class MQTTServer {
	public:
		IPAddress ip;
		unsigned int port = 1883;

	public:
		MQTTServer(const IPAddress serverIP) {
			this->ip = serverIP;
		}
		MQTTServer(const IPAddress serverIP, unsigned int port) {
			this->ip = serverIP;
			this->port = port;
		}
};

class MQTTUser {
// @todo
};

enum GatewayStatus {
	INIT,
	CONNECTED,
	RECONNECT,
	NETWORK_ERROR,
	NETWORK_CONNECTED,
};

class MQTTGateway {
	public:
		GatewayStatus status = INIT;
		const uint8_t* mac;
		MQTTServer *server;
		String ns;
		char *connectionId;
		PubSubClient *client;
		int reconnectionDelay = 2000;

	public:
	MQTTGateway(const uint8_t* mac, MQTTServer *server, String ns, const char *connectionId) {
		this->mac = mac;
		this->server = server;
		this->ns = ns;
		this->connectionId = connectionId;
	}

	void connect() {
		EthernetClient ethClient;
		this->client = new PubSubClient (ethClient);

		// connect by DHCP
		if (Ethernet.begin(this->mac) == 0) {
			this->status = NETWORK_ERROR;
			return; 
		}

		this->client->setServer(this->server->ip, this->server->port);
		//this->client->setCallback(callback);

		this->status = NETWORK_CONNECTED;
	}

	void update() {
		if (this->client->connected()) {
			this->status = CONNECTED;
			return;
		}
		
		this->status = RECONNECT;
  		if (this->client->connect(this->connectionId)) {
			// todo use client and password
			this->status = CONNECTED;

		} else {
			delay(this->reconnectionDelay);
		}
	}

	void push(String path, String message) {
		String target = this->getTarget(path);
		char* topic = target.c_str();
		char* msg = message.c_str();
    	this->client->publish(topic, msg);
	}

	/**
	 * get target of path with namespace
	 */
	String getTarget(String path) {
		if (path == "") {
			return this->ns;
		}
		return this->ns + "/" + path;
	}
};

#endif