#include <Wire.h>

#define i2c_slave_add   0x11

#define i2c_reg_INPUT   0x01
#define i2c_reg_OUTPUT  0x02

#define i2c_mask_OUTPUT 0xF0

#define DOUT_7  10
#define DOUT_8  9
#define DOUT_9  8
#define DOUT_10 7

#define DIN_9   0
#define DIN_10  1
#define DIN_11  2
#define DIN_12  3

byte  transceive_byte = 0x00,
      received_byte = 0x00;

byte  byte_s_output = 0,
      byte_s_input = 0;

void setup() {

  pinMode(DOUT_7, OUTPUT);
  pinMode(DOUT_8, OUTPUT);
  pinMode(DOUT_9, OUTPUT);
  pinMode(DOUT_10, OUTPUT);

  pinMode(DIN_9, INPUT);
  pinMode(DIN_10, INPUT);
  pinMode(DIN_11, INPUT);
  pinMode(DIN_12, INPUT);

  digitalWrite( DOUT_7, LOW );
  digitalWrite( DOUT_8, LOW );
  digitalWrite( DOUT_9, LOW );
  digitalWrite( DOUT_10, LOW );
  
  Wire.begin( i2c_slave_add );
  Wire.onRequest( requestEvent );
  Wire.onReceive( receiveEvent );

}

void loop() {
    delay(100);
    update_outputs();
    update_inputs();
}

void test_blink() {
  digitalWrite( DOUT_7, HIGH );
  delay(1000);
  digitalWrite( DOUT_7, LOW );

  digitalWrite( DOUT_8, HIGH );
  delay(1000);
  digitalWrite( DOUT_8, LOW );

  digitalWrite( DOUT_9, HIGH );
  delay(1000);
  digitalWrite( DOUT_9, LOW );

  digitalWrite( DOUT_10, HIGH );
  delay(1000);
  digitalWrite( DOUT_10, LOW );
}

void receiveEvent() {
  
  received_byte = 0x00;
  transceive_byte = 0x00;
  
  while (Wire.available()>0) { 
    received_byte = Wire.read(); // receive byte as a character
  }
  //
  if ( (received_byte & i2c_mask_OUTPUT) == i2c_mask_OUTPUT )  {
    byte_s_output = received_byte;//(received_byte & ~i2c_mask_OUTPUT);
  }
  //
  if (received_byte == i2c_reg_INPUT)  {
    transceive_byte = byte_s_input;
  }
  //
}

void requestEvent() {
  Wire.write( transceive_byte );
}

void update_outputs() {
  //
  if ( (byte_s_output&0b00000001)>0 )  {
    digitalWrite(DOUT_7, HIGH);
  } else {
    digitalWrite(DOUT_7, LOW);
  }
  //
  if ( (byte_s_output&0b00000010)>0 )  {
    digitalWrite(DOUT_8, HIGH);
  } else {
    digitalWrite(DOUT_8, LOW);
  }
  //
  if ( (byte_s_output&0b00000100)>0 )  {
    digitalWrite(DOUT_9, HIGH);
  } else {
    digitalWrite(DOUT_9, LOW);
  }
  //
  if ( (byte_s_output&0b00001000)>0 )  {
    digitalWrite(DOUT_10, HIGH);
  } else {
    digitalWrite(DOUT_10, LOW);
  }
  //
}

void update_inputs()  {
  if ((digitalRead(DIN_9))==HIGH) {
    byte_s_input |= (1<<0);
  } else {
    byte_s_input &= ~(1<<0);
  }
  //
  if ((digitalRead(DIN_10))==HIGH) {
    byte_s_input |= (1<<1);
  } else {
    byte_s_input &= ~(1<<1);
  }
  //
  if ((digitalRead(DIN_11))==HIGH) {
    byte_s_input |= (1<<2);
  } else {
    byte_s_input &= ~(1<<2);
  }
  //
  if ((digitalRead(DIN_12))==HIGH) {
    byte_s_input |= (1<<3);
  } else {
    byte_s_input &= ~(1<<3);
  }
  //
}
