/*
 *  turtle.cpp
 *  robotBlocks
 *
 *  Created by Exhibits on 7/19/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "turtle.h"

extern int pixPerInch;

extern ofColor black, white, gray, yellow, blue, red, orange;

ofTurtle::ofTurtle()
{
  start.x=start.y=0;
  pos.x=pos.y=0;
  w=h=0;
}

void ofTurtle::setup(int _x, int _y, int _w, int _h)
{
  numLineStored=pixPerInch*16;
  start.x=pos.x=_x,start.y=pos.y=_y,w=_w,h=_h;
  whlHgt=h/2;
  whlWid=w/8;
  bearing=ofVector(0,-1);
}

void ofTurtle::setup(string filename)
{
  map.loadImage(filename);
  pixPerInch=map.width/48;
  setup(2.5*(map.width/12.),map.height-2.5*(map.height/12.), pixPerInch*5,7.25*pixPerInch);
}

void ofTurtle::move(int pixels)
{
  pos=pos+bearing.unit()*pixels;
  if(!bTurnSinceRecord&&lines.size()>1) lines.pop_front();
  else bTurnSinceRecord=false;
  lines.push_front(pos+bearing.unit()*pixels);
  if(lines.size()>numLineStored) lines.pop_back();
}

void ofTurtle::turn(int degrees)
{
  bTurnSinceRecord=true;
  bearing.rotate(degrees);
}

bool ofTurtle::sensorIsClear(ofPoint strtPnt,int pixels, ofImage & walls, int direction)
{
  bool ret=true;
  unsigned char * k=walls.getPixels();
  for (int i=0; i<4; i++) {
    ofPoint ps=strtPnt+bearing.unit().rotate(direction)*(i+1)*pixels/4;
    int wid=walls.width;
    int hgt=walls.height;
    if(ps.x>0&&ps.x<wid&&ps.y>0&&ps.y<hgt){
      //ret=true;
      if(k[int(ps.y)*wid*3+int(ps.x)*3+1]<200){
        ret&=true;
      }
      else ret=false;
    }
  }
  return ret;
}

bool ofTurtle::frontIsClear(int pixels, ofImage & walls)
{
  
  return sensorIsClear(pos, pixels, walls);
}

bool ofTurtle::leftIsClear(int pixels, ofImage & walls)
{
  ofPoint ps=pos+bearing.ortho().unit()*w/2-bearing.unit()*h/2;
  return sensorIsClear(ps, pixels, walls,270);
}

bool ofTurtle::frontIsClear(int distance)
{
  return frontIsClear(distance,map);
}

bool ofTurtle::leftIsClear(int distance)
{
  return leftIsClear(distance,map);
}

ofPoint ofTurtle::pointAlongBearing(int pix)
{
   return pos+bearing.unit()*pix;
}

void ofTurtle::draw(int _x, int _y)
{
  int ppi=pixPerInch;
  ofPoint wheel(.25*ppi,(2+3/8)*ppi);
  ofPoint rWheel(1.125*ppi,1.875*ppi);
  ofRectangle body(-3.25*ppi/2,-wheel.y/2,ppi*3.25,ppi*4.5);
  //int body=w-whlWid*2;
  //int leng=h;
  for (unsigned int i=0; i<lines.size()-1&&lines.size()>1; i++) {
    ofSetLineWidth(2);
    ofLine(lines[i].x, lines[i].y, lines[i+1].x, lines[i+1].y);
  }
  ofPushMatrix();
  ofTranslate(pos.x, pos.y, 0);
  ofRotate(360-bearing.absoluteAngle());
  
  ofSetColor(black.opacity(.5));
  ofRect(body);
  ofSetColor(white);
  ofNoFill();
  ofRect(body);
  ofFill();
  ofSetColor(black);
  ofRect(body.x-ppi*3/8, body.y, wheel.x, wheel.y);
  ofRect(body.x+body.width+ppi*3/32, body.y, wheel.x, wheel.y);
  ofSetColor(white*.8);
  ofRect(body.x+(body.width-ppi*1.375)/2, body.y+body.height, 1.375*ppi, .25*ppi);
  ofRect(body.x+(body.width-ppi*1.375)/2+1.125*ppi, body.y+body.height, .25*ppi,1.375*ppi);
  ofSetColor(white*.5);
  ofRect(body.x+(body.width-ppi*1.375)/2+.125*ppi, body.y+body.height+.5*ppi, .875*ppi, 1.5*ppi);
  ofSetColor(orange);
  ofRect(body.x+(body.width-ppi*1.375)/2+.125*ppi, body.y+body.height+.50625*ppi, .375*ppi, .4375*ppi);
  ofRect(body.x+(body.width-ppi*1.375)/2+.125*ppi, body.y+body.height+(2.-.4375)*ppi, .375*ppi, .4375*ppi);
  ofRect(body.x+(body.width-ppi*1.375)/2+(1-.375)*ppi, body.y+body.height+(.5+(1.5-.4375)/2)*ppi, .375*ppi, .4375*ppi);
  ofPopMatrix();
  
  if(!frontIsClear(4*pixPerInch)) ofSetColor(255, 0, 0);
  else ofSetColor(0, 255, 0);
  ofPoint ps = pointAlongBearing(4*pixPerInch);
  ofCircle(ps.x, ps.y, 5);
  
  if(!leftIsClear(2*pixPerInch)) ofSetColor(255, 0, 0);
  else ofSetColor(0, 255, 0);
  ps = pos+bearing.ortho().unit()*w/2-bearing.unit()*w/2+bearing.unit().rotate(270)*2*pixPerInch;
  ofCircle(ps.x, ps.y, 5);
}
