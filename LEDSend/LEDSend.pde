import ddf.minim.*;
import ddf.minim.analysis.*;
import processing.serial.*;

Minim minim;
AudioInput in; 
FFT fft;
Serial myPort;

void setup() {
  size(1028,400);
  
  // ready for FFT
  minim=new Minim(this);
  in=minim.getLineIn(Minim.STEREO,512);
  fft=new FFT(in.bufferSize(),in.sampleRate());
  fft.window(FFT.HAMMING);
  
  // setting color mode & stroke weight 
  colorMode(HSB,360,100,100);
  strokeWeight(2);
  
  // setting serial port
  myPort = new Serial(this, "/dev/cu.usbmodem1411", 119200);
}

void draw() {
  background(0);

  // FFT analysis
  fft.forward(in.mix);

  float max_data = 0;
  float max_hue = 0;
    
  // draw line
  for(int i=0; i<fft.specSize();i++){
    // setting color
    float hue = map(i, 0, fft.specSize(), 0, 360 * 6.5);
    stroke(hue, 100, 100);
    
    // calc x, y point for drawing line
    float x = map(i, 0, fft.specSize(), 0, width);
    float data = map(fft.getBand(i), 0, 30, 0, height);
    line(x, height, x, height - data);
    
    // get highest band & hue
    if (max_data < data) {
      max_data = data;
      max_hue = hue;
    }
  }

  int send_data;
  int thres = 200;
  if (max_data > thres) {
     send_data = int(max_hue / 2);
  } else {
     send_data = 255; 
  }
    
  // send to Arudino
  myPort.write(send_data);
}

void stop() {
  minim.stop();   
  super.stop(); 
}