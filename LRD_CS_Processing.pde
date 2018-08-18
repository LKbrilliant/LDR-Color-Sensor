/*  LDR Color Sensor(Color view) v2.2
    Code by : LKbrilliant
    Date : 09:05:2016
 This code will read the serial port which contain the RGB
 color values which sent by the arduino in 'xxx : xxx : xxx' format.
 After that the background color will change according to the 
 most recent RGB values on the serial port.
 */


import processing.serial.*;             //Import the Serial library

Serial myPort;                          //Create a serial object
String arduColor;                       // To store color
String ports;
int numberOfPorts;                      // Number of serial ports available
int index;
int portNumber;
int pPort = 0;
boolean overUp = false;
boolean overDown = false;
boolean overStart = false;
boolean overRefresh = false;
boolean portFound = false;
boolean overBack = false;

PFont LevenimMT, Cambria ;

void setup() {
  
  size(420, 360);
  LevenimMT = loadFont("LevenimMT-30.vlw");
  Cambria = loadFont("Cambria-30.vlw");
  
  //  printArray(Serial.list());                      //Check available ports and print them as an array
  numberOfPorts = Serial.list().length;               //Get the number of ports witch available
}

void draw() {
 // background(0);                                    // Background color black   
  if (portFound == false) {    
    if (numberOfPorts != 0) {                         // if there are any available ports
//----------------------------------------------------------------------------------------//       
// Print the information on the start up display
      background(100);
      fill(0);
      textFont(Cambria);
      textSize(25);
      text("Available Serial Ports", 2, 30);
      textSize(22);
      text("Arduino is in port", 150, 300);
      text("[", 325, 300);
      text(pPort, 333, 301);
      text("]", 345, 300);
      textSize(17);
      text("Tip : If you'r having multiple serial ",150,100);
      text("ports, try disconnecting and ",188,120);
      text("reconnecting the Arduino to ",188,140);
      text("find out which port will connet ",188,160);
      text("with the Arduino. ",188,180);
//----------------------------------------------------------------------------------------//
      numberOfPorts = Serial.list().length;
      for (int i = 0; i< numberOfPorts; i++) {
        index = numberOfPorts - (numberOfPorts - i);
//-----------------------------------------------------//
// Handling Exceptions
        try{
        ports =  Serial.list()[index];
        }
        catch(ArrayIndexOutOfBoundsException e){
          numberOfPorts = Serial.list().length;
        }
//----------------------------------------------------//
// Print all available serial ports with their index number
        fill(0);     
        textFont(LevenimMT);
        textSize(17);
        text("[", 1, 55 + i*20);
        text(index, 9, 55 + i*20);
        text("]", 18, 55 + i*20);
        text(ports, 30, 55 + i*20);
                
      }                                    // End of for loop
//----------------------------------------------------------------------------------------//    
// Creating the up arrow
      if (mouseX> 330 && mouseX < 345  && mouseY > 261  && mouseY < 277) {
        overUp = true;
        stroke(255);
      } 
      else {
        overUp = false;
        stroke(100);
      }
      fill(255);
      triangle(338, 265, 342, 273, 334, 273);
//----------------------------------------------------------------------------------------//      
//Creating the down arrow
      if (mouseX > 330 && mouseX < 345  && mouseY > 310  && mouseY < 322) {
        overDown = true;
        stroke(255);
      } 
      else {
        overDown = false;
        stroke(100);
      }
      fill(255);
      triangle(338, 320, 342, 312, 334, 312);
//----------------------------------------------------------------------------------------//
// Creating the Start button
      if (mouseX > 360 && mouseX < 410  && mouseY > 283  && mouseY < 303) {
        overStart = true;
        stroke(255);
        fill(255);
      } else {
        overStart = false;
        stroke(100);
        fill(0);
      }
      noFill();
      rect(360, 283, 50, 20, 7);
      textSize(15);
      text("Start", 369, 298);      
//----------------------------------------------------------------------------------------//
    }                                                  // End of "if(numberOfPorts != 0)" 
    else {                                             // if there's no any available serial ports
      numberOfPorts = Serial.list().length;
      background(0);
      textFont(Cambria);
      textSize(50);
      text("No Serial Ports", 50, 190);                
    } 
  }                                                    // End of " if(portFound == false)"
//----------------------------------------------------------------------------------------//
  else {    
    if ( myPort.available() > 0) {                     //check if data available
      arduColor = myPort.readStringUntil('\n');        //read the serial port until a new line(/n)
    }
    if ( arduColor != null && arduColor.length()==17) {
      //try{
        String kr = arduColor.substring(0, 3);         //get only point 0 to point 3 as string
        String kg = arduColor.substring(6, 9);
        String kb = arduColor.substring(12, 15);
      //}
      //catch(StringIndexOutOfBoundException e){
        
      //}

      int R = int(kr);                                //convert string values into integers
      int G = int(kg);  
      int B = int(kb);

      //println(R + " " + G + " " + B);

      background(R, G, B);                           //change background color to newest RGB color

//----------------------------------------------------------------------------------------//
//Creating a back button
      fill(0);
      if (mouseX > 360 && mouseX < 410  && mouseY > 323  && mouseY < 353) {
        overBack = true;
        stroke(255);
        fill(150);
        rect(360, 323, 50, 20, 7);
        fill(0);
      } else {
        overBack = false;
        stroke(100);
        fill(50);
        rect(360, 323, 50, 20, 7);
        fill(200);
      }
      
      textSize(15);
      text("Back", 369, 338);
      
    }                           // End of "if (arduColor != null)"
  }  
}


void mousePressed() {
  if (overUp == true && pPort > 0) {
    pPort --;
  } 
  else if (overDown == true && pPort < numberOfPorts-1) {
    pPort ++;
  } 
  else if(overBack == true){
    myPort.clear();
    myPort.stop();
    portFound = false;
  }
  else if (overStart == true) {
    portNumber = pPort;
    portFound = true;
    String portName = Serial.list()[portNumber];
    background(0);
    fill(255);
    textFont(Cambria);
    textSize(50);
    text("  Calibrating...!", 50, 190);
    
    
  //try{
    myPort = new Serial(this, portName, 9600);
  //}
  //catch(Error opening serial port COM10 e){
  //    exit();
  //}
  }
}
