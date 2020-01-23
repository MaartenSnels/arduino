#ifndef __functions_h__
#define __functions_h__

struct Request {
  String path;
  String command;
};


void startAccessPoint();
void handleAccessPoint();
bool startServer();
void handleServer();
void getTime();
String currentTime2String();
String getHeader(long);
Request getCommand(String);
void handleCommand(String);

#define LED1          BUILTIN_LED                // ledje op circuit
#define LED2          13                        // ledje 13, default led bij ARDUINO
#define SERIAL_SPEED  250000                     // snelheid voor Serial
#define TIME_SHIFT    1                          // afwijking t.o.v. standaard tijd

#endif
