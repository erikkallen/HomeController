#include <SPI.h>
#include <RF12B.h>
#include <rf_packet.h>
#include <Messenger.h>

#define CLOUD_PAN_ID 12
#define FREQUENCY 433.0

Messenger message = Messenger(); 

void setup() {	
	Serial.begin(57600);
	RF12.begin();
        RF12.setPanID(CLOUD_PAN_ID);
        RF12.setFrequency(FREQUENCY);
        //RF12.setDatarate(9600);

	Serial.println("LED Control started");
        message.attach(messageCompleted);
}

void messageCompleted() {

  if ( message.checkString("set_color") ) { // Read pins (analog or digital)
  // Read from serial
    byte cloud_id = message.readInt();
    byte r = message.readInt();
    byte g = message.readInt();
    byte b = message.readInt();
    byte cmd[] = {'c',r,g,b};
    RFPacket packet(cmd, 4, 1, 1,2);
    RF12.setPanID(cloud_id);
    RF12.send(packet, packet.size());
    Serial.print("Sent msg: ");
    Serial.print(packet);
    Serial.print(" to ");
    Serial.println(cloud_id); 
  } 
}

void loop() {
	if (Serial.available()>0) {
          message.process(Serial.read( ) );
	}
	if (RF12.packetAvailable()) {
		RFPacket p = RF12.recvPacket();
		// Only print valid packets
		if (p.valid()) {
			Serial.println("Received:");
			Serial.print(p);
		}
	}
}
