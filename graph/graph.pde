import processing.serial.*;
 
 Serial myPort;        // The serial port
 int xPos = 1;         // horizontal position of the graph
int offset=-50; 
 void setup () {
 // set the window size:
 size(999, 500);        
 
 // List all the available serial ports
 println(Serial.list());
 // I know that the first port in the serial list on my mac
 // is always my  Arduino, so I open Serial.list()[0].
 // Open whatever port is the one you're using.
 myPort = new Serial(this, "/dev/ttyUSB0", 115200);
 // don't generate a serialEvent() unless you get a newline character:
 myPort.bufferUntil('\n');
 // set inital background:
 background(0);
 }
 void draw () {
 // everything happens in the serialEvent()
 }
 
 void serialEvent (Serial myPort) {
 // get the ASCII string:
 String inString = myPort.readStringUntil('\n');
 
 if (inString != null) {
 // trim off any whitespace:
 inString = trim(inString);
 // convert to an int and map to the screen height:
 float inByte = float(inString);
 inByte = map(inByte, 0, 65536, 0, );
 stroke(127,34,255); 
 switch(int(inByte)){
   case 100:
     inByte=50;
     stroke(255,255,255);
     break;
   case 150:
     xPos = -1;
     offset+=50;
     break;
 }
 // draw the line:
 
 line(xPos, offset, xPos, offset + inByte);
 
 // at the edge of the screen, go back to the beginning:
 if (xPos >= width) {
 xPos = 0;
 offset+=50;
 //background(0); 
 } 
 else {
 // increment the horizontal position:
 xPos++;
 }
 }
 }
 
