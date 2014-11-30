#include <apa.h>

#define LED 13
#define apa_1_in 2
#define apa_1_out 3

static struct apa_port_type port_0, port_1;

bool blink_led = false;

void setup(){
	Serial.begin(9600);
  	Serial.println("Hello I am an FTDI Arduino!");

  	pinMode(apa_1_in, INPUT);
  	pinMode(apa_1_out,OUTPUT);
  	
   	digitalWrite(apa_1_out, LOW);

   	port_0.id = '0';
	port_0.path_in_length = 0;
	port_0.path_out_length = 0;
	port_0.next_port = &port_1;

        port_1.pins_in = portInputRegister(digitalPinToPort(apa_1_in));
	port_1.port_out = portOutputRegister(digitalPinToPort(apa_1_out));
	port_1.direction_out = portModeRegister(digitalPinToPort(apa_1_out));
	port_1.pin_in = digitalPinToBitMask(apa_1_in);
	port_1.pin_out = digitalPinToBitMask(apa_1_out);
	port_1.id = '1';
	port_1.path_in_length = 0;
	port_1.path_out_length = 0;
	port_1.next_port = &port_0;

	power_on_delay();	  	
}

void loop(){
	if(Serial.available() > 0){
		apa_get_serial_packet(&port_0);
		if(port_0.path_in_length !=0){
			apa_move_packet(&port_0, &port_1);
			apa_put_packet(&port_1);
		}
	}

	if(digitalRead(*port_1.pins_in)){
		apa_get_packet(&port_1);
		if(port_1.path_in_length != 0){
			apa_move_packet(&port_1,&port_0);
			apa_put_serial_packet(&port_0);
		}
	}

}

void apa_get_serial_packet(struct apa_port_type *port){
	//
	// get packet on port
	//
	char c;
	unsigned char cnt;
	//
	// find start
	//
	cnt = apa_timeout;
	do {
		c = Serial.read();
		cnt -= 1;
		if (cnt == 0) {
			port->path_in_length = 0;
			return;
			}
		} while (c != apa_start);
	//
	// get path
	//
	port->path_in_length = 0;
	cnt = apa_max_packet;
	do {
	   c = Serial.read();
	   cnt -= 1;
	   if (cnt == 0) {
	      port->path_in_length = 0;
	      return;
	      }
	   port->path_in[port->path_in_length] = c;
	   port->path_in_length += 1;
	   } while (c != apa_divider);
	port->path_in_length -= 1;
	//
	// get payload
	//
	port->payload_in_length = 0;
	cnt = apa_max_packet;
	do {
	   //
	   // get next char
	   //
	   c = Serial.read();
	   cnt -= 1;
	   if (cnt == 0) {
	      port->path_in_length = 0;
	      return;
	      }
	   //
	   // check for escape
	   //
	   if (c == apa_escape) {
	      //
	      // get escaped char
	      //
	      c = Serial.read();
	      cnt -= 1;
	      if (cnt == 0) {
	         port->path_in_length = 0;
	         return;
	         }
	      port->payload_in[port->payload_in_length] = c;
	      port->payload_in_length += 1;
	      //
	      // get next char
	      //
	      c = Serial.read();
	      cnt -= 1;
	      if (cnt == 0) {
	         port->path_in_length = 0;
	         return;
	         }
	      }
	   //
	   // save char
	   //
	   port->payload_in[port->payload_in_length] = c;
	   port->payload_in_length += 1;
	   } while (c != apa_end);
	port->payload_in_length -= 1;
}

void apa_put_serial_packet(struct apa_port_type *port) {
	//
	// put packet on port
	//	
	unsigned char i;
	char c;
	//
	// put start
	//
	Serial.print(apa_start);
	//
	// put path
	//	
	for(int i = 0; i < port->path_out_length;i++){
		Serial.print(char(port->path_out[i]));
	}
	port->path_out_length = 0;
	//
	// put divider
	//	
	Serial.print(apa_divider);
	//
	// put payload
	//
	for (i = 0; i < port->payload_out_length; ++i) {
		//
		// get next char
		//
		c = port->payload_out[i];
		//
		// check if escape needed
		//
		if ((c == apa_end) || (c == apa_escape))
			//
			// yes, put escape
			//
			Serial.print(apa_escape);
		//
		// put char
		//
		Serial.print(c);
		}
	port->payload_out_length = 0;
	//
	// put end
	//
	Serial.println(apa_end);
}
