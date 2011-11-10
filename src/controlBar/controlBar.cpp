/*
 *  controlBar.cpp
 *  arenaBot
 *
 *  Created by Exhibits on 11/7/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "controlBar.h"
#include "../sideBar/sideBar.h"

extern ofColor black, white, gray,blue,yellow,orange, red;

extern int CURRENT_ROBOT_NUMBER;

string ROOT_NAME="";
string ROOT_DIR="";

//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_
//_-_-_-_-_//_-_-_-_-_button Hldr//_-_-_-_-_//_-_-_-_-_

buttonHolder::buttonHolder()
{
  area.width=0;
  area.height=0;
}

void buttonHolder::addObj(ofInterObj & obj)
{
  objs.push_back(&obj);
  area.width+=obj.w;
}

void buttonHolder::internalSpace(int spc)
{
  space=spc;
  area.width+=space*(objs.size()-1);
}

double buttonHolder::maxHeight()
{
  double ret=0;
  for (unsigned int i=0; i<objs.size(); i++) {
    ret=max(ret,objs[i]->h);
  }
  return ret;
}

void buttonHolder::padding(int pd)
{
  pad=pd;
  area.width+=pad;
}

void buttonHolder::draw(int _x, int _y)
{
  drawShadowsAroundRect(area,10);
  area.x=x=_x;
  area.y=y=_y;
  int xPos=pad/2;
  for (unsigned int i=0; i<objs.size(); i++) {
    objs[i]->draw(x+xPos-((objs.size()==1)?objs[i]->w/2:0),y+(area.height-objs[i]->h)/2);
    xPos+=objs[i]->w+space;
  }
}



//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_
//_-_-_-_-_//_-_-_-_-_control Bar//_-_-_-_-_//_-_-_-_-_

void controlBar::setup(bGroup * bG, sbGroup * sbG)
{
  blocks=bG;
  sideBar=sbG;
  
  clearBut.setup("Clear blocks", "fonts/HelveticaBold.otf", 19);
  redoBut.setup(64, 64, "images/redo.png");
	undoBut.setup(64, 64, "images/undo.png");
  demo.setup("View Demo", "fonts/HelveticaBold.otf", 19);
  skipBut.setup(300, 100, "images/skipBut.png");
  
  anim.setup(blocks, sideBar);
  
  subtitle.loadFont("fonts/DinC.ttf");
  subtitle.setSize(22);
  subtitle.setMode(OF_FONT_TOP);
  
  subBar.height=subtitle.stringHeight("Kjhg")*1.5;
  subBar.width=ofGetWidth();
  
  ROOT_DIR=config("config.cfg");
  
  sets.load(ROOT_DIR);
  loadBlocks(sets[0]);
  
  sets[0].choice.setAvailable(false);
  
  upload.setup(blocks);
  
  //_-_-_-_-_//_-_-_-_-_ spacing setup //_-_-_-_-_//_-_-_-_-_
  demoHldr.addObj(demo);
  
  for (unsigned int i=0; i<sets.size(); i++) setsHldr.addObj(sets(i));
  setsHldr.internalSpace(20);
  
  
  undoHldr.addObj(undoBut);
  undoHldr.addObj(redoBut);
  undoHldr.internalSpace(20);
  
  clearHldr.addObj(clearBut);
  
  double maxH=max(demoHldr.maxHeight(),max(setsHldr.maxHeight(),undoHldr.maxHeight()));
  buttonBar=ofRectangle(x,y,ofGetWidth(),maxH*1.2);
  
  demoHldr.area.height=setsHldr.area.height=undoHldr.area.height=clearHldr.area.height=buttonBar.height;
  
  int pad=(buttonBar.width-(demoHldr.area.width+setsHldr.area.width+undoHldr.area.width+clearHldr.area.width))/4;
  demoHldr.padding(pad);
  setsHldr.padding(pad);
  undoHldr.padding(pad);
  clearHldr.padding(pad);
  
  h=subBar.height+buttonBar.height;
  w=ofGetWidth();
  
  loadBlocks(sets[0]);
  setAvailableButtons();
}

void controlBar::loadBlocks(blockGroup & bg){
	
  if(bg.nLoaded>=3){
    
    //--------- load the new blocks with the blockGroup data
    ROOT_NAME=bg.title;
    sideBar->clear();
    sideBar->setup(bg.blockXML,blocks);
    
    anim.changeAnimXML(bg.animXML);
  }
}

void controlBar::draw(int _x, int _y)
{
  buttonBar.x=x=_x;
  buttonBar.y=y=_y;
  subBar.x=x;
  subBar.y=y+buttonBar.height;
  
  
  //_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_
  //_-_-_-_-_//_-_-_-_-_//buttonbar//_-_-_-_-_//_-_-_-_-_
  
  ofSetColor(black);
  ofRect(buttonBar);
  
  ofSetColor(gray);
  drawHatching(buttonBar.x, buttonBar.y, buttonBar.width, buttonBar.height, 85,80);
  
  demoHldr.draw(buttonBar.x,buttonBar.y);
  setsHldr.draw(demoHldr.x+demoHldr.area.width,buttonBar.y);
  undoHldr.draw(setsHldr.x+setsHldr.area.width,buttonBar.y);
  clearHldr.draw(undoHldr.x+undoHldr.area.width,buttonBar.y);
  
  //_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_
  //_-_-_-_-_//_-_-_-_-_//subTitle //_-_-_-_-_//_-_-_-_-_
  
  ofSetColor(gray);
  ofRect(subBar);
  drawShadowsAroundRect(subBar, 10);
  
  ofSetColor(yellow);
  ofRect(subBar.x, subBar.y, subBar.width, 1);
  
  if(sets.getSelected()){
    ofButton & t=sets.getSelected()->choice;
    int wid=t.w/16+1;
    ofSetColor(yellow);
    ofRect(t.x-wid, y, t.w+wid*2, buttonBar.height);
    wid=t.w/16;
    ofSetColor(gray);
    ofRect(t.x-wid, y, t.w+wid*2, buttonBar.height+10);
    t.draw(t.x,t.y);
    
    ofSetColor(yellow);
    subtitle.drawString(sets.getSelected()->subtitle, 50, subBar.y+(subBar.height-subtitle.stringHeight(sets.getSelected()->subtitle))/2);
  }
  
  //_-_-_-_-_//_-_-_-_-_ shadow below bar //_-_-_-_-_//_-_-_-_-_
  //ofSetShadowDarkness(.5);
  //ofShade(x, y+h, 10, w, OF_DOWN);
}

void controlBar::drawForeground(){
  anim.drawCursor();
  
  if(anim.isPlaying()){
    ofSetColor(0, 0, 0,128);
    ofRect(skipBut.x-10, skipBut.y-10, skipBut.w+20, skipBut.h+20);
    skipBut.draw((ofGetWidth()-skipBut.w)/2, ofGetHeight()*3./4);
  }
  
  serChk.drawWaitScreen();
  upload.drawUploadWait();
}

void controlBar::update()
{
  serChk.threadCheckAvailability();
  anim.update();
  
  if(serChk.justLostDevice()){
    cout << "lost it\n";
    blocks->saveXML("programs/"+ofToString(CURRENT_ROBOT_NUMBER)+"/program.xml");
    blocks->clear();
  }
  if(serChk.justFoundDevice()){
    blocks->loadFile("programs/"+ofToString(CURRENT_ROBOT_NUMBER)+"/program.xml");
    blocks->updatePositions();
  }
}

//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_//_-_-_-_-_
//_-_-_-_-_//_-_-_-_-_ mouse func//_-_-_-_-_//_-_-_-_-_

bool controlBar::clickDown(int _x, int _y, int button)
{
  if(sets.clickDown(_x,_y)&&!anim.isPlaying()){
    if(sets.getSelected())
      loadBlocks((*sets.getSelected()));
  }
  
  if (clearBut.clickDown(_x, _y)) {
    blocks->clear();
  }
  
  if(demo.clickDown(_x, _y)){
    anim.play();
  }
  
  //--------- if we press the undo button, roll back the state of the blockGroup
  if (undoBut.clickDown(_x, _y)) {
    blocks->undoState();
  }
  
  //--------- if we press the redo button, push the state forward
  if (redoBut.clickDown(_x, _y)) {
    blocks->redoState();
  }
  
  //--------- if we press the skip button while the anim is running, stop anim
  if(anim.isPlaying()&&skipBut.clickDown(_x, _y)){
    anim.stop();
  }
  
  upload.clickDown(_x, _y);
}

bool controlBar::clickUp()
{
  sets.clickUp();
  clearBut.clickUp();
  demo.clickUp();
  undoBut.clickUp();
  redoBut.clickUp();
  skipBut.clickUp();
}

bool controlBar::mouseLockout(int button)
{
  return (!anim.isPlaying()||(anim.isPlaying()&&button==VMOUSE_BUTTON));
}

void controlBar::setAvailableButtons()
{
  redoBut.setAvailable(blocks->redoAvailable());
	undoBut.setAvailable(blocks->undoAvailable());
  demo.setAvailable(!anim.isPlaying());
}
