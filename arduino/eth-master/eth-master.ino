#include <Wire.h>

#include "etherShield.h"
#include "ETHER_28J60.h"
//#define BUFFER_SIZE 384 - поменять в либе в ETHER_28J60.cpp
#define DOUT_1    0xB0
#define DOUT_2    0xD7
#define DOUT_3    0xD6
#define DOUT_4    0xD5
#define DOUT_5    0xB7
#define DOUT_6    0xB6
/*
#define DOUT_7    0xD7
#define DOUT_8    0xD6
#define DOUT_9    0xD5
#define DOUT_10   0xB7
*/
#define ETH_RES   0xB1

#define DIN_1     A1
#define DIN_2     A0
#define DIN_3     A7
#define DIN_4     A6
#define DIN_5     A2
#define DIN_6     A3
#define DIN_7     3
#define DIN_8     4

#define i2c_mask_OUTPUT 0xF0
#define i2c_slave_add   0x11

#define i2c_reg_INPUT   0x01

static uint8_t mac[6] = {0x54, 0x55, 0x58, 0x10, 0x00, 0x24};
static uint8_t ip[4] = {192, 168, 137, 15};
static uint16_t port = 80;

ETHER_28J60 ethernet;

byte byte_m_output = 0, // 0b[RES,   RES,   DOUT_6, DOUT_5, DOUT_4,  DOUT_3, DOUT_2, DOUT_1]
     byte_s_output = 0, // 0b[RES,   RES,   RES,    RES,    DOUT_10, DOUT_9, DOUT_8, DOUT_7]
     byte_m_input = 0,  // 0b[DIN_8, DIN_7, DIN_6,  DIN_5,  DIN_4,   DIN_3,  DIN_2,  DIN_1]
     byte_s_input = 0;  // 0b[RES,   RES,   RES,    RES,    DIN_12,  DIN_11, DIN_10, DIN_9]

void setup() {
  _pinMode(DOUT_1, OUTPUT);
  _pinMode(DOUT_2, OUTPUT);
  _pinMode(DOUT_3, OUTPUT);
  _pinMode(DOUT_4, OUTPUT);
  _pinMode(DOUT_5, OUTPUT);
  _pinMode(DOUT_6, OUTPUT);
  
  _pinMode(ETH_RES, OUTPUT);

  _digitalWrite(ETH_RES, LOW);
  _digitalWrite(DOUT_1, LOW);
  _digitalWrite(DOUT_2, LOW);
  _digitalWrite(DOUT_3, LOW);
  _digitalWrite(DOUT_4, LOW);
  _digitalWrite(DOUT_5, LOW);
  _digitalWrite(DOUT_6, LOW);

  Wire.begin();
  
  delay(100);
  _digitalWrite(ETH_RES, HIGH);

  ethernet.setup(mac, ip, port);

  delay(10000);
}

void loop() {
  char* params;
  
  if (params = ethernet.serviceRequest())
  {
    request_execution( params );
    
    http_TX();
    
    ethernet.respond();
  }
  
  //test_blink();
  update_outputs();
  update_inputs();
  delay(10);

  main_algoritm();
}

void http_TX(){
  //
  //ethernet.print( "<table border='1'>" );
  http_open_table_draw( "DOUT" );
  http_grid_name_draw( 10 );
  http_dout_status_draw();
  
  ethernet.print( "\n" );
  for (int i=0;i<32;i++) {
    ethernet.print( " " );
  }
  ethernet.print( "\n" );
  
  http_dout_commands_draw();

  //ethernet.print( "</table>\n" );
  http_close_table_draw();
  //
  http_open_table_draw( "DIN" );
  http_grid_name_draw( 12 );
  http_din_status_draw();
  //ethernet.print( "</table>\n" );
  http_close_table_draw();
  //
  ethernet.print("<A HREF='/'>refresh</A></p>\n");
  //
  http_byte_print("OUTM", byte_m_output);
  http_byte_print("OUTS", byte_s_output);
  http_byte_print("INM", byte_m_input);
  http_byte_print("INS", byte_s_input);
}

void http_byte_print(char* str_byte_name, byte byte_to_print)  {
  ethernet.print( "<p>" );
  ethernet.print( str_byte_name );
  ethernet.print( ":" );
  ethernet.print( byte_to_print );
  ethernet.print( "</p>\n" );
}

void http_open_table_draw(char* str_caption) {
  ethernet.print( "<table border='1'>" );
  ethernet.print( "<caption>" );
  //ethernet.print( "<tr><th>" );
  ethernet.print( str_caption );
  ethernet.print( "</caption>\n" );
  //ethernet.print( "</th></tr>\n" );
}

void http_close_table_draw()  {
  ethernet.print( "</table>\n" );
}

void http_grid_name_draw(byte byte_count) {
  ethernet.print( "<tr>" );
  //
  for (int i=1;i<=byte_count;i++)  {
    ethernet.print( "<th>" );
    ethernet.print( i );
    ethernet.print( "</th>" );
  }
  //
  ethernet.print( "</tr>\n" );
}

void http_dout_status_draw() {
  ethernet.print( "<tr>" );
  //
  draw_grid_data( byte_m_output&0b00000001 );
  //
  draw_grid_data( byte_m_output&0b00000010 );
  //
  draw_grid_data( byte_m_output&0b00000100 );
  //
  draw_grid_data( byte_m_output&0b00001000 );
  //
  draw_grid_data( byte_m_output&0b00010000 );
  //
  draw_grid_data( byte_m_output&0b00100000 );
  //
  draw_grid_data( byte_s_output&0b00000001 );
  //
  draw_grid_data( byte_s_output&0b00000010 );
  //
  draw_grid_data( byte_s_output&0b00000100 );
  //
  draw_grid_data( byte_s_output&0b00001000 );
  //
  ethernet.print( "</tr>\n" );
}

void http_din_status_draw() { // функция не переопределена для дискретных входов!
  ethernet.print( "<tr>" );
  //
  draw_grid_data( byte_m_input&0b00000001 );
  //
  draw_grid_data( byte_m_input&0b00000010 );
  //
  draw_grid_data( byte_m_input&0b00000100 );
  //
  draw_grid_data( byte_m_input&0b00001000 );
  //
  draw_grid_data( byte_m_input&0b00010000 );
  //
  draw_grid_data( byte_m_input&0b00100000 );
  //
  draw_grid_data( byte_m_input&0b01000000 );
  //
  draw_grid_data( byte_m_input&0b10000000 );  // входа на мастере
  //
  draw_grid_data( byte_s_input&0b00000001 );
  //
  draw_grid_data( byte_s_input&0b00000010 );
  //
  draw_grid_data( byte_s_input&0b00000100 );
  //
  draw_grid_data( byte_s_input&0b00001000 );
  //
  ethernet.print( "</tr>\n" );
}

void http_dout_commands_draw() {
  ethernet.print( "<tr>" );
  //
  draw_grid_command( byte_m_output&0b00000001, "1" );
  //
  draw_grid_command( byte_m_output&0b00000010, "2" );
  //
  draw_grid_command( byte_m_output&0b00000100, "3" );
  //
  draw_grid_command( byte_m_output&0b00001000, "4" );
  //
  draw_grid_command( byte_m_output&0b00010000, "5" );
  //
  draw_grid_command( byte_m_output&0b00100000, "6" );
  //
  draw_grid_command( byte_s_output&0b00000001, "7" );
  //
  draw_grid_command( byte_s_output&0b00000010, "8" );
  //
  draw_grid_command( byte_s_output&0b00000100, "9" );
  //
  draw_grid_command( byte_s_output&0b00001000, "10" );
  //
  ethernet.print( "</tr>\n" );
}

void draw_grid_data(byte byte_status) {
  if ( byte_status>0 )  {
    ethernet.print( "<td>H</td>" );
  } else {
    ethernet.print( "<td>L</td>" );
  }
}

void draw_grid_command(byte byte_status, char* str_channel) {
  if ( byte_status>0 )  {
    ethernet.print( "<td><A HREF='" );
    ethernet.print( str_channel );
    ethernet.print( "L'>OFF</A></td>" );
  } else {
    ethernet.print( "<td><A HREF='" );
    ethernet.print( str_channel );
    ethernet.print( "H'>ON </A></td>" );
  }
}

void request_execution(char* http_get){
  if (strcmp(http_get, "1H") == 0) {
    byte_m_output |= (1<<0);
  }
  if (strcmp(http_get, "1L") == 0) {
    byte_m_output &= ~(1<<0);
  }
  //
  if (strcmp(http_get, "2H") == 0) {
    byte_m_output |= (1<<1);
  }
  if (strcmp(http_get, "2L") == 0) {
    byte_m_output &= ~(1<<1);
  }
  //
  
  if (strcmp(http_get, "3H") == 0) {
    byte_m_output |= (1<<2);
  }
  if (strcmp(http_get, "3L") == 0) {
    byte_m_output &= ~(1<<2);
  }
  //
  if (strcmp(http_get, "4H") == 0) {
    byte_m_output |= (1<<3);
  }
  if (strcmp(http_get, "4L") == 0) {
    byte_m_output &= ~(1<<3);
  }
  //
  if (strcmp(http_get, "5H") == 0) {
    byte_m_output |= (1<<4);
  }
  if (strcmp(http_get, "5L") == 0) {
    byte_m_output &= ~(1<<4);
  }
  //
  if (strcmp(http_get, "6H") == 0) {
    byte_m_output |= (1<<5);
  }
  if (strcmp(http_get, "6L") == 0) {
    byte_m_output &= ~(1<<5);
  }
  //
  if (strcmp(http_get, "7H") == 0) {
    byte_s_output |= (1<<0);
  }
  if (strcmp(http_get, "7L") == 0) {
    byte_s_output &= ~(1<<0);
  }
  //
  if (strcmp(http_get, "8H") == 0) {
    byte_s_output |= (1<<1);
  }
  if (strcmp(http_get, "8L") == 0) {
    byte_s_output &= ~(1<<1);
  }
  //
  if (strcmp(http_get, "9H") == 0) {
    byte_s_output |= (1<<2);
  }
  if (strcmp(http_get, "9L") == 0) {
    byte_s_output &= ~(1<<2);
  }
  //
  if (strcmp(http_get, "10H") == 0) {
    byte_s_output |= (1<<3);
  }
  if (strcmp(http_get, "10L") == 0) {
    byte_s_output &= ~(1<<3);
  }
  //
}

void update_outputs() {
  if ( (byte_m_output&0b00000001)>0 )  {
    _digitalWrite(DOUT_1, HIGH);
  } else {
    _digitalWrite(DOUT_1, LOW);
  }
  
  if ( (byte_m_output&0b00000010)>0 )  {
    _digitalWrite(DOUT_2, HIGH);
  } else {
    _digitalWrite(DOUT_2, LOW);
  }

  if ( (byte_m_output&0b00000100)>0 )  {
    _digitalWrite(DOUT_3, HIGH);
  } else {
    _digitalWrite(DOUT_3, LOW);
  }

  if ( (byte_m_output&0b00001000)>0 )  {
    _digitalWrite(DOUT_4, HIGH);
  } else {
    _digitalWrite(DOUT_4, LOW);
  }

  if ( (byte_m_output&0b00010000)>0 )  {
    _digitalWrite(DOUT_5, HIGH);
  } else {
    _digitalWrite(DOUT_5, LOW);
  }

  if ( (byte_m_output&0b00100000)>0 )  {
    _digitalWrite(DOUT_6, HIGH);
  } else {
    _digitalWrite(DOUT_6, LOW);
  }

  Wire.beginTransmission( i2c_slave_add );
  Wire.write( (byte_s_output | i2c_mask_OUTPUT) ); 
  Wire.endTransmission();
  
}

void update_inputs() {
  int a_in = 0;

  a_in = analogRead(DIN_1);
  if (a_in>500) {
    byte_m_input |= (1<<0);
  } else {
    byte_m_input &= ~(1<<0);
  }

  a_in = analogRead(DIN_2);
  if (a_in>500) {
    byte_m_input |= (1<<1);
  } else {
    byte_m_input &= ~(1<<1);
  }

  a_in = analogRead(DIN_3);
  if (a_in>500) {
    byte_m_input |= (1<<2);
  } else {
    byte_m_input &= ~(1<<2);
  }

  a_in = analogRead(DIN_4);
  if (a_in>500) {
    byte_m_input |= (1<<3);
  } else {
    byte_m_input &= ~(1<<3);
  }

  a_in = analogRead(DIN_5);
  if (a_in>500) {
    byte_m_input |= (1<<4);
  } else {
    byte_m_input &= ~(1<<4);
  }

  a_in = analogRead(DIN_6);
  if (a_in>500) {
    byte_m_input |= (1<<5);
  } else {
    byte_m_input &= ~(1<<5);
  }

  //a_in = analogRead(DIN_7);
  if ((digitalRead(DIN_7))==HIGH) {
    byte_m_input |= (1<<6);
  } else {
    byte_m_input &= ~(1<<6);
  }

  //a_in = analogRead(DIN_8);
  if ((digitalRead(DIN_8))==HIGH) {
    byte_m_input |= (1<<7);
  } else {
    byte_m_input &= ~(1<<7);
  }

  Wire.beginTransmission( i2c_slave_add );
  Wire.write( i2c_reg_INPUT ); 
  Wire.endTransmission();
  Wire.requestFrom(i2c_slave_add, 1);

  while (Wire.available()) { 
    byte_s_input = Wire.read(); 
  }
  
}
