#ifndef __http_h__
#define __http_h__

// defines voor de wepsite
String  FUNCTION1ON  = "F_1_ON",
        FUNCTION2ON  = "F_2_ON",
        FUNCTION1OFF = "F_1_OFF",
        FUNCTION2OFF = "F_2_OFF";


#define MAIN __http_h__

#define HTML_HEAD "<html><head><title>Demo ESP8266</title></head>"
#define CLOSE_BODY "</body></html>"

String HTML      = "text/html";
#define MAIN_PAGE1 "<body><h1>You are connected to DeKlok</h1>" 
#define MAIN_PAGE2 "Hello Klok"
#define MAIN_PAGE3 "<form><P>ssid:<input type='text' name='ssid'></p>"
#define MAIN_PAGE4 "<P>ssid:<input type='text' name='pwd'></p><input type='submit' value='OK'></form>"
#define MAIN_PAGE MAIN_PAGE1 MAIN_PAGE2 MAIN_PAGE3 MAIN_PAGE4 CLOSE_BODY

#define HEAD2 "<body><font color=\"#000000\"><body bgcolor=\"#d0d0f0\">"
#define HEAD3 "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=yes\">"
#define HEAD4 "<h1>Demo ESP8266</h1>"
#define HTML_HEAD HEAD1 HEAD2 HEAD3 HEAD4

// knoppen op de led pagina
String BUTTONS1 = "<p>WIFI LED <a href=\"?pin=" + FUNCTION1ON + "\"><button>ON</button></a>&nbsp;<a href=\"?pin=" + FUNCTION1OFF + "\"><button>OFF</button></a></p>";
String BUTTONS2 = "<p>D13 LED  <a href=\"?pin=" + FUNCTION2ON + "\"><button>ON</button></a>&nbsp;<a href=\"?pin=" + FUNCTION2OFF + "\"><button>OFF</button></a></p>";


#define HEADER1 "HTTP/1.1 404 Not found\r\nContent-Length: "
#define HEADER2 "\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n"


#define PAGE_NOT_FOUND "<html><head><title>404 Not Found</title></head><body><h1>Not Found</h1><p>The requested URL was not found on this server.</p></body></html>"

#define GET "GET " // vergeet de spatie NIET

#endif
