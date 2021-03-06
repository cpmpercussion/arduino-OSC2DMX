// Synesthetic Lighting Module
// Charles P. Martin 2017
// Arduino code for controlling DMX lights, controlled via OSC.
#include <Conceptinetics.h>
#include <LiquidCrystal_I2C.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>    
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

#define DMX_MASTER_CHANNELS   512
#define RXEN_PIN                2
#define LIGHT_ONE               1
#define LIGHT_TWO               6

DMX_Master dmx_master(DMX_MASTER_CHANNELS, RXEN_PIN);
LiquidCrystal_I2C lcd(0x3f, 16, 2); // Address of I2C LCD display
EthernetUDP Udp;
const unsigned int localPort = 8888;
boolean online = false;
byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 100);

// sets a light to a specific RGB (and dimness) level.
// Communicates in Thomann 5-channel DMX format (R, G, B, Dim, Strobe)
void setLightToRGB(int light, int red, int green, int blue, int dim) {
  dmx_master.setChannelValue(light, red); // red
  dmx_master.setChannelValue(light + 1, green); // green
  dmx_master.setChannelValue(light + 2, blue); // blue
  dmx_master.setChannelValue(light + 3, dim); // dimmer
  dmx_master.setChannelValue(light + 4, 0); // strobe
}

void setup() 
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Lighting Module");
  dmx_master.enable();
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(3, HIGH);   // turn the LED on (HIGH is the voltage level)
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);
  lcd.setCursor(0,1);
  lcd.print(Ethernet.localIP());

  // Boot sequence
  setLightToRGB(LIGHT_ONE, 255, 0, 0, 255);
  delay(1000);
  setLightToRGB(LIGHT_ONE, 0, 255, 0, 255);
  delay(1000);
  setLightToRGB(LIGHT_ONE, 0, 0, 255, 255);
  delay(1000);
  setLightToRGB(LIGHT_ONE, 0, 0, 0, 0);
}

void loop()
{
  OSCMessage msg;
  int size = Udp.parsePacket();
  if (size > 0) {
    while (size--) {
      msg.fill(Udp.read());
    }
    if (!msg.hasError()) {
      msg.dispatch("/light", light);
    }
  }
}

void light(OSCMessage &msg) {
  int red = msg.getInt(0);
  int green = msg.getInt(1);
  int blue = msg.getInt(2);
  int dim = msg.getInt(3);
  setLightToRGB(LIGHT_ONE, red, green, blue, dim);
  // Put LCD status on screen.
  // lcd.clear();
  // lcd.setCursor(0,0);
  // lcd.print("R:");
  // lcd.print(red);
  // lcd.setCursor(9,0);
  // lcd.print("G:");
  // lcd.print(green);
  // lcd.setCursor(0,1);
  // lcd.print("B:");
  // lcd.print(blue);
  // lcd.setCursor(9,1);
  // lcd.print("D:");
  // lcd.print(dim);
}

