#include "testApp.h"

string curDir="basic";
extern string ROOT_NAME;
extern string ROOT_DIR;

extern int pixPerInch;

extern ofColor black,white,yellow,blue,orange, gray;

//--------------------------------------------------------------
void testApp::setup(){
	
	//--------- Initialize the valid working space for the blocks
	blocks.setup(250, 0, ofGetWidth(), ofGetHeight());
	
	//sidebar = 0;
	
	//--------- Load the background images (wood panel and aluminum
	background.loadImage("images/background.jpg");
	
	//--------- Load the images for the buttons
	upBut.setup( 128,128,"images/upload2.png");
  
	//--------- Load font for drawing on screen
	titleFont.loadFont("fonts/DinC.ttf");
	titleFont.setSize(30);
	titleFont.setMode(OF_FONT_CENTER);
  
  topTitle.loadFont("fonts/DinC.ttf", 35);
  
  label.loadFont("fonts/HelveticaBold.otf");
  label.setSize(20);
  label.setMode(OF_FONT_TOP);

  
	ofHideCursor();
  
  mapps.loadImage("maps/map_2.jpg");
  
  controls.setup(&blocks, &sidebar);
}

//--------------------------------------------------------------
void testApp::update(){
	
  //blocks.update();
	
	//---------- Set which blocks are available to be pressed
	
	//upBut.setAvailable(!systemCall.isRunning());
	
  controls.update();
}


//--------------------------------------------------------------
void testApp::draw(){
    
  //--------- Draw background with slightly yellow grid over it.
  ofBackground(0x33, 0x33, 0x33);
  
  
  ofSetColor(black);
  drawHatching(0, 0, ofGetWidth(), ofGetHeight(), 3,3);
  
  //--------- Draw any blocks that are in the composition area and are not clicked.
  
  //blocks.draw();
  
  //******************** Menu Bar **************************
  
  //drawBase_and_Demo(controls.h+topTitle.h);
  
  blocks.draw(sidebar.area.width,controls.h+topTitle.h,ofGetWidth()-sidebar.area.width,ofGetHeight()-(controls.h+topTitle.h));
  
  sidebar.draw(0,controls.h+topTitle.h);
  
  controls.draw(0, topTitle.h);

  topTitle.draw("Program the "+ROOT_NAME+" behaviors",0,0);
  
  //********************** Draw the blocks which are being held by the mouse ********
  
  blocks.drawForeground();
	
	//************************ if we're uploading, draw a fancy "uploading..." on screen

	if (systemCall.isRunning()) {
    ofSetColor(0, 0, 0,128);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
		ofSetColor(255, 255, 255);
		spinner.draw((ofGetWidth())/2,ofGetHeight()/2, 300);
		titleFont.setMode(OF_FONT_LEFT);
		titleFont.setSize(70);
		string printOut="uploading";
		for (int i=0; i<(spinner.count()/3)%4; i++) {
			printOut.append(".");
		}
		ofSetColor(255, 255, 255);
		titleFont.drawString(printOut, ofGetWidth()/4, ofGetHeight()/4);
		titleFont.setMode(OF_FONT_CENTER);
		titleFont.setSize(30);
	}
	
  
  if(blocks.isTesting()){

    
    ofSetColor(0x33, 0x33, 0x33);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    
		ofSetColor(0x444400);
		for (int i=0; i*10<ofGetHeight(); i++) {
			ofRect(0, i*10, ofGetWidth(), 1);
		}
		for (int i=0; i*10<ofGetWidth(); i++) {
			ofRect(i*10, 0, 1, ofGetHeight());
		}
    
    blocks.base.draw(ofGetHeight(), 75);
    blocks.drawCurrentBlock();
    
    ofSetColor(0xCFCFCF);
    ofRect(0,0, ofGetWidth(), 75);
    ofSetShadowDarkness((0xCF-0xA8)/255.);
    ofShade(0, 0, 75, ofGetWidth(), OF_DOWN);
    ofSetShadowDarkness(.15);
    ofShade(0, 75, 10, ofGetWidth(), OF_UP);
    ofShade(0, 75, 10, ofGetWidth(), OF_DOWN);
    
    ofPushMatrix();
    ofTranslate(100,100, 0);
    ofScale(((double)ofGetHeight()-200.)/(double)blocks.mapp.width, (double(ofGetHeight())-200.)/double(blocks.mapp.width), 1);
    ofSetColor(255, 255, 255);
    blocks.mapp.draw(0, 0);
    
    blocks.turtle.draw(0, 0);
    
    if(!blocks.turtle.frontIsClear(4*pixPerInch, blocks.mapp)) ofSetColor(255, 0, 0);
    else ofSetColor(0, 255, 0);
    ofPoint ps = blocks.turtle.pos+blocks.turtle.bearing.unit()*4*pixPerInch;
    ofCircle(ps.x, ps.y, 5);
    
    /*if(!blocks.turtle.rightIsClear(2*pixPerInch, blocks.mapp)) ofSetColor(255, 0, 0);
    else ofSetColor(0, 255, 0);
    ps = blocks.turtle.pos-blocks.turtle.bearing.ortho().unit()*blocks.turtle.w/2-blocks.turtle.bearing.unit()*blocks.turtle.w/2+blocks.turtle.bearing.unit().rotate(90)*2*pixPerInch;
    ofCircle(ps.x, ps.y, 5);*/
    
    if(!blocks.turtle.leftIsClear(2*pixPerInch, blocks.mapp)) ofSetColor(255, 0, 0);
    else ofSetColor(0, 255, 0);
    ps = blocks.turtle.pos+blocks.turtle.bearing.ortho().unit()*blocks.turtle.w/2-blocks.turtle.bearing.unit()*blocks.turtle.w/2+blocks.turtle.bearing.unit().rotate(270)*2*pixPerInch;
    ofCircle(ps.x, ps.y, 5);
    ofPopMatrix();
  }
  
  controls.drawForeground();
}

void testApp::upload()
{
	//--------- Generate the file which we will compile and upload to the arduino
	blocks.writeFile("arduino_make/arduino_make.pde");
	
	//--------- once we've generated the file, compile and upload with one of teh following scripts
#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
	systemCall.run(ofToDataPath("arduino_make/upload.sh"));
#else
	systemCall.run(ofToDataPath("\"data\\arduino_make\\build.bat\""));
#endif
	
	//--------- unpress the upload button 
	upBut.setPressed(false);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	//************** only the key=='n' is used, to signal when the animation has finished a step.
	if(key=='w'){
		upload();
	}
	
  
  if(key=='t'){
    if (blocks.isTesting()) blocks.stopTesting();
    else blocks.startTesting();
  }
  if(key==OF_KEY_UP){
    blocks.turtle.move(6);
  }
  if(key==OF_KEY_RIGHT){
    blocks.turtle.turn(6);
  }
  if(key==OF_KEY_LEFT){
    blocks.turtle.turn(-6);
  }
  if(key==OF_KEY_DOWN){
    blocks.turtle.move(-6);
  }
  if(key=='P'){
    blocks.startSequence();
  }
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	//--------- if the mouse is moving and not clicked, inform the blocks
	blocks.motion(x, y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
	//--------- if we are dragging the mouse, tell blocks so it can update the positions of any of the held blocks
  if(controls.mouseLockout(button))
     blocks.drag(x, y);
}

//***************** Mouse Click down function ***************
void testApp::mousePressed(int x, int y, int button){
  
  controls.clickDown(x, y);
  
  //--------- Check the blocks in the sidebar, to see if they have been clicked
  sidebar.clickDown(x, y);
  
  //--------- Check the blocks in the comp area
  blocks.newClickDown(x, y);
  
  //--------- Run upload function if the upload button is pressed
  if(upBut.clickDown(x, y)){
    upload();
  }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
  controls.clickUp();
	if(controls.mouseLockout(button)){
    //--------- do a bunch of clickups
    blocks.newClickUp(x, y);
    upBut.clickUp();
    sets.clickUp();
  }
  skipBut.clickUp();
  controls.setAvailableButtons();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

