# Ma, I’m home

That project is a connected button which, when pressed, send a SMS to people. It’s used by my son, to text me when he’s back from school.


## ESP8266 Connections

- VCC to 3.3
- Ground to ground
- 0.1 nF capacitor between ground and VCC (the closest to ESP)
- RX from FTDI to TX on ESP
- TX from FTDI to RX on ESP
- DTR from FTDI to GPIO 0 on ESP
- RST from FTDI to RST on ESP
- CH_PD from ESP to 10 Kohms HIGH
- GPIO 15 from ESP to 10 Kohms LOW

## links

  * [ESP8266 official doc](https://github.com/espressif/esptool/wiki/ESP8266-Boot-Mode-Selection)
  * [A Beginner's Guide to the ESP8266](https://tttapa.github.io/ESP8266/Chap01%20-%20ESP8266.html)
  * [ESP8266 NodeMCU HTTPS Secured POST Request](https://circuits4you.com/2019/02/08/esp8266-nodemcu-https-secured-post-request/)
  * [ESP8266 Wi-Fi Button – DIY Amazon Dash Button Clone](https://randomnerdtutorials.com/esp8266-wi-fi-button-diy-amazon-dash-button-clone/)
  * [Free Mobile SMS API](https://www.freenews.fr/freenews-edition-nationale-299/free-mobile-170/nouvelle-option-notifications-par-sms-chez-free-mobile-14817)
