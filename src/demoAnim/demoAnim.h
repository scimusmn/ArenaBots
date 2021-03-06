/*
 *  demoAnim.h
 *  arenaBot
 *
 *  Created by Exhibits on 11/7/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#pragma once

#include "../sideBar/sideBar.h"

#include "ofVMouse.h"

class demoAnim : public ofAnimation {
protected:
  int animStep;
  ofXML animXML;
	ofImage pointer;
  
  bGroup * blocks;
  sbGroup * sideBar;
public:
  void setup(bGroup * bG, sbGroup * sbG);
  void animationStepRequested();
  void handleAnimStep(ofTag tag);
  ofInterObj * searchForObject(ofTag & tag, int & _x, int & _y);
  ofInterObj * searchBlock(vector<string> spl, block & b, int offset);
  void changeAnimXML(ofXML & newXML);
  void update();
  void drawCursor();
  void play();
  bool isPlaying();
  void stop();
  
};