//
//  MainGUI.cpp
//  APV_Controller
//
//  Created by Mauro Ferrario on 07/02/15.
//
//

#include "MainGUI.h"

MainGUI::MainGUI()
{
  
}

void MainGUI::init()
{
  readXMLSettings();
  guiVisible = false;
  timelineVisible = true;
  initTimeline();
  guiPosY = timeline.getHeight()+10;
  initGeneralGUI();
  initGraphicGUI();
  initEffecGUI();
  initMovementGUI();
  initShaderGUI();
  initEvents();
  initOSC();
  directDraw      = true;
  drawProp = outputSize.x/outputSize.y;
}

void MainGUI::readXMLSettings()
{
  ofxXmlSettings xml;
  xml.load("mySettings.xml");
  outputSize.x = xml.getValue("controller:output:width", 0);
  outputSize.y = xml.getValue("controller:output:height", 0);
  host = xml.getValue("controller:output:host", "");
  port = xml.getValue("controller:output:port", 0);
  cout << outputSize << endl;
  cout << host << endl;
  cout << port << endl;
}

void MainGUI::initOSC()
{
  sender.setup(host, port);
}

void MainGUI::draw()
{
  ofPushStyle();
  ofSetColor(100,100);
  drawArea.width = ofGetWindowWidth();
  drawArea.height = drawArea.width/drawProp;
  drawArea.x = 0;
  drawArea.y = (ofGetWindowHeight() - drawArea.height)*.5;
  ofRect(drawArea);
  ofPopStyle();
  
  ofSetColor(255);
  drawPoints();
  if(timelineVisible)
  {
    ofPushStyle();
    ofSetColor(0);
    ofRect(0,0,ofGetWindowWidth(), timeline.getHeight());
    ofPopStyle();
    timeline.draw();
  }
  if(guiVisible)
  {
    generalGUI.draw();
    graphicGUI.draw();
    effecGUI.draw();
    movementGUI.draw();
    shaderGUI.draw();
  }
}

void MainGUI::drawPoints()
{
  vector<ofPoint*>::iterator it = points.begin();
  vector<ofPoint*>::iterator prevIt;
  
  ofPushMatrix();
  ofVec2f offset;
  offset.x = 0;
  offset.y = drawArea.y;
  ofTranslate(offset);
  
  int cont = 0;
  while(it != points.end())
  {
    if(cont > 0)
    {
      prevIt = it - 1;
      ofLine((*it)->x/outputSize.x*drawArea.width, (*it)->y/outputSize.y*drawArea.height, (*prevIt)->x/outputSize.x*drawArea.width, (*prevIt)->y/outputSize.y*drawArea.height);
    }
    cont++;
    it++;
  }
  ofPopMatrix();
}

void MainGUI::loadFromSVG(int id)
{
  clear();
  VectorDraw::loadFromSVG(this, id, "svg/", outputSize);
}

void MainGUI::loadGeometric()
{
  clear();
  GeometricDraw::loadGeometric(this, geomId, geomParam1, geomParam2, outputSize);
}

void MainGUI::initEvents()
{
  ofAddListener(ofEvents().keyReleased, this, &MainGUI::keyReleased);
#ifdef FREE_DRAW_MODE
  ofAddListener(ofEvents().mouseDragged, this, &MainGUI::mouseDragged);
#endif
  ofAddListener(ofEvents().mousePressed, this, &MainGUI::mousePressed);
  ofAddListener(ofEvents().mouseReleased, this, &MainGUI::mouseReleased);
}

void MainGUI::keyReleased (ofKeyEventArgs &e)
{
  char key = e.key;
  if(e.key == OF_KEY_UP)
    guiVisible = !guiVisible;
  if(e.key == OF_KEY_DOWN)
    timelineVisible = !timelineVisible;
  if(e.key == ' ')
    timeline.togglePlay();
}

void MainGUI::mouseReleased(ofMouseEventArgs &e)
{
}

void MainGUI::mousePressed(ofMouseEventArgs &e)
{
}

#ifdef FREE_DRAW_MODE
void MainGUI::mouseDragged(ofMouseEventArgs &e)
{
  if(guiVisible||timelineVisible)
    return;
  int x = e.x;
  int y = e.y;
  addSinglePoint(x, y);
  if(directDraw)
    sendSinglePoint(points[points.size() - 1]);
}
#endif

void MainGUI::clear()
{
  int size = points.size();
  for(int a = 0; a < size; a++)
  {
    ofPoint* pParticle = points[points.size() - 1];
    points.pop_back();
    delete pParticle;
  }
}

void MainGUI::addSinglePoint(int x, int y)
{
  ofPoint* tempPoint;
  tempPoint = new ofPoint();
  tempPoint->x = x;
  tempPoint->y = y;
  points.push_back(tempPoint);
}

void MainGUI::sendSinglePoint(ofPoint* tempPoint)
{
  sendSinglePoint(tempPoint->x, tempPoint->y);
  tempPoint = NULL;
}

void MainGUI::sendSinglePoint(float x, float y)
{
  ofxOscMessage m;
  m.setAddress( "/addSinglePoint" );
  m.addFloatArg( x  / outputSize.x );
  m.addFloatArg( y  / outputSize.y );
  if(directDraw)
    sender.sendMessage( m );
}

void MainGUI::update()
{
  if(timeline.getIsPlaying())
  {
    ofColor newColor;
    newColor.r = timeline.getValue("Color r") * 255;
    newColor.g = timeline.getValue("Color g") * 255;
    newColor.b = timeline.getValue("Color b") * 255;
    color = newColor;
    
    particleSpeed = timeline.getValue("Particle Speed");
    sameSpring = timeline.getValue("Same Spring");
    sameFriction = timeline.getValue("Same Friction");
    repulsionForce = timeline.getValue("Repulson Force");
    
    
    minPerimeter = timeline.getValue("Min Perimeter");
    maxPerimeter = timeline.getValue("Max Perimeter");
    minLineDistance = timeline.getValue("Min Line Distance");
    maxLineDistance = timeline.getValue("Max Line Distance");
  }
  syncGeneralGUI.update();
  //syncGraphiclGUI.update();
  syncEffecGUI.update();
  syncMovementGUI.update();
  syncShaderGUI.update();
  ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

void MainGUI::initTimeline()
{
  timeline.setup();
  timeline.setSpacebarTogglePlay(false);
  timeline.setDurationInSeconds(97);
  timeline.setLoopType(OF_LOOP_NORMAL);
  timeline.setHeight(200);
  timeline.addCurves("Color r");
  timeline.addCurves("Color g");
  timeline.addCurves("Color b");
  timeline.addCurves("Particle Speed");
  timeline.addCurves("Same Spring");
  timeline.addCurves("Same Friction");
  timeline.addCurves("Repulson Force");
  
  timeline.addCurves("Min Perimeter");
  timeline.addCurves("Max Perimeter");
  
  timeline.addCurves("Min Line Distance");
  timeline.addCurves("Max Line Distance");
  
  timeline.addFlags("Change Command");
  ofAddListener(timeline.events().bangFired, this, &MainGUI::receivedBang);
}

void MainGUI::receivedBang(ofxTLBangEventArgs& bang)
{
  ofLogNotice("Bang fired from track " + bang.flag);
  if(bang.flag == "Draw")
  {
    directDraw = true;
    sendManyPoints();
  }
  if(bang.flag == "DrawFalse")
  {
    directDraw = false;
  }
  if(bang.flag == "Clear")
  {
    clearChanged();
  }
  if(bang.flag == "ClearAll")
  {
    clearAllChanged();
  }
  int loadVectorPos = bang.flag.find("LoadVector");
  if(loadVectorPos >= 0)
  {
    bool fakeBool = true;
    vectorId = ofToInt(ofSplitString(bang.flag, "LoadVector")[1]);
    loadSvgChanged(fakeBool);
  }
  if(bang.flag == "DrawPoint")
  {
    drawPoint = true;
  }
  if(bang.flag == "DrawPointFalse")
  {
    drawPoint = false;
  }
  if(bang.flag == "ConnectPoints")
  {
    connectPoints = true;
  }
  if(bang.flag == "ConnectPointsFalse")
  {
    connectPoints = false;
  }
  if(bang.flag == "PrevPoint")
  {
    connectPrevPoint = true;
  }
  if(bang.flag == "PrevPointFalse")
  {
    connectPrevPoint = false;
  }
  if(bang.flag == "DrawTriangle")
  {
    drawTriangle = true;
  }
  if(bang.flag == "DrawTriangleFalse")
  {
    drawTriangle = false;
  }
}

void MainGUI::initGeneralGUI()
{
  generalGUI.setup("General");
  generalGUI.setPosition(ofPoint(0,guiPosY));
  generalGUI.add(visualIP.setup("Visual IP", ""));
  generalGUI.add(toggleFullscreen.set("Fullscreen", true));
  generalGUI.add(secureLimit.setup("Secure Limit",true));
  generalGUI.add(visualFrameRate.setup("Visual Framerate",""));
  generalGUI.add(togglePlayPauseTimeline.set("Play/Pause Timeline",false));
  volumeGroup.setName("Volume");
  volumeGroup.add(manualInvert.set("Manual Invert", false));
  volumeGroup.add(forceInvert.set("Force Invert", false));
  volumeGroup.add(volumeInvertLimit.set("Volume Invert Limit", 0, 0, 1));
  generalGUI.add(volumeGroup);
  toggleFullscreen.addListener(this, &MainGUI::toggleFullscreenChanged);
  togglePlayPauseTimeline.addListener(this, &MainGUI::togglePlayPauseTimelineChanged);
  syncGeneralGUI.setup((ofParameterGroup&)generalGUI.getParameter(),6660, host,port);
}

void MainGUI::initGraphicGUI()
{
  graphicGUI.setup("Graphic");
  graphicGUI.setPosition(ofPoint(210,guiPosY));
  graphicGUI.add(directDraw.setup("Direct draw", true));
  graphicGUI.add(bClear.setup("Clear"));
  graphicGUI.add(clearAll.setup("Clear All"));
  vectorGroup.setName("Vector");
  vectorGroup.add(loadVector.set("Load vector", false));
  vectorGroup.add(vectorId.set("Vector id", 0, 0, VectorDraw::getTotSvgs("svg/")-1));
  graphicGUI.add(vectorGroup);
  geomGroup.setName("Geom");
  geomGroup.add(loadGeom.set("Load geom", false));
  geomGroup.add(geomId.set("Geom id", 0, 0, 4));
  geomGroup.add(geomParam1.set("Geom param 1",0,0,1));
  geomGroup.add(geomParam2.set("Geom param 2", 0, 0, 1));
  graphicGUI.add(geomGroup);
  loadGeom.addListener(this, &MainGUI::loadGeomChanged);
  loadVector.addListener(this, &MainGUI::loadSvgChanged);
  vectorId.addListener(this, &MainGUI::vectorChanged);
  geomId.addListener(this, &MainGUI::geomChanged);
  bClear.addListener(this, &MainGUI::clearChanged);
  clearAll.addListener(this, &MainGUI::clearAllChanged);
  directDraw.addListener(this, &MainGUI::directDrawChanged);
}

void MainGUI::directDrawChanged(bool & value)
{
  if(value == 1)
    sendManyPoints();
}

void MainGUI::loadSvgChanged(bool & value)
{
  loadVector = false;
  loadFromSVG(vectorId);
  if(directDraw)
    sendManyPoints();
}

void MainGUI::vectorChanged(int & value)
{
  if(!directDraw)
    loadFromSVG(value);
}

void MainGUI::loadGeomChanged(bool & value)
{
  loadGeom = false;
  loadGeometric();
  if(directDraw)
    sendManyPoints();
}

void MainGUI::geomChanged(int & value)
{
  if(!directDraw)
    loadGeometric();
}

void MainGUI::toggleFullscreenChanged(bool & value)
{
  toggleFullscreen = false;
  ofToggleFullscreen();
}

void MainGUI::togglePlayPauseTimelineChanged(bool & value)
{
  togglePlayPauseTimeline = false;
  timeline.togglePlay();
}

void MainGUI::clearChanged()
{
  clear();
}

void MainGUI::sendGeometric()
{
  }

void MainGUI::sendManyPoints()
{
  ofxOscMessage message;
  message.clear();
  message.setAddress( "/loadShape" );
  int cont = 0;
  int contPoint = 0;
  message.addIntArg(points.size());
  cont = -1;
  for(int a = 0; a < points.size(); a++)
  {
    if(a%301 == 0 && a >= 0)
    {
      message.addFloatArg( points[a]->x  / outputSize.x );
      message.addFloatArg( points[a]->y  / outputSize.y );
      sender.sendMessage(message);
      cont++;
      message.clear();
      message.setAddress( "/addPoint" );
      int pointToSend = (points.size() - a);
      if(pointToSend >= 301)
        pointToSend = 301;
      message.addFloatArg(pointToSend);
      contPoint = 0;
    }
    message.addFloatArg( points[a]->x  / outputSize.x );
    message.addFloatArg( points[a]->y  / outputSize.y );
    contPoint++;
  }
  if(points.size()%301 != 0)
  {
    sender.sendMessage( message );
  }
}


void MainGUI::clearAllChanged()
{
  clear();
  ofxOscMessage message;
  message.clear();
  message.setAddress( "/clear" );
  sender.sendMessage(message);
}

void MainGUI::initEffecGUI()
{
  effecGUI.setup("Effect");
  effecGUI.setPosition(ofPoint(420,guiPosY));
  effecGUI.add(drawPoint.setup("Draw point", false));
  effecGUI.add(drawTriangle.setup("Draw triangle", false));
  effecGUI.add(connectPrevPoint.setup("Connect to prev point", false));
  effecGUI.add(connectPoints.setup("Connect points", false));
  triangleGroup.setName("Triangles");
  triangleGroup.add(sameColorTriangle.set("Same Color Triangles", true));
  triangleGroup.add(minPerimeter.set("Min Perimeter", 0, 0, 1));
  triangleGroup.add(maxPerimeter.set("Max Perimeter",0,0,1));
  triangleGroup.add(triangleLimit.set("Triangle Limit", 0, 0, 1));
  triangleGroup.add(color.set("Color", ofFloatColor(1), ofFloatColor(0), ofFloatColor(1)));
  effecGUI.add(triangleGroup);
  connectLinesGroup.setName("Connect Lines");
  connectLinesGroup.add(minLineDistance.set("Min Line Distance", 0, 0, 1));
  connectLinesGroup.add(maxLineDistance.set("Max Line Distance",0,0,1));
  effecGUI.add(connectLinesGroup);
  color.addListener(this, &MainGUI::colorChanged);
  minPerimeter.addListener(this, &MainGUI::minPerimeterChanged);
  maxPerimeter.addListener(this, &MainGUI::maxPerimeterChanged);
  minLineDistance.addListener(this, &MainGUI::minLineDistanceChanged);
  maxLineDistance.addListener(this, &MainGUI::maxLineDistanceChanged);
  syncEffecGUI.setup((ofParameterGroup&)effecGUI.getParameter(),6662, host,port);
}


void MainGUI::colorChanged(ofFloatColor & newColor)
{
  if(timeline.getIsPlaying())
  {
    ofxOscMessage message;
    message.setAddress("/Effect/Triangles/Color");
    message.addStringArg(ofToString(newColor.r) +", "+ofToString(newColor.g)+ ","+ofToString(newColor.b)+","+ofToString(newColor.a));
    sender.sendMessage(message);
  }
}

void MainGUI::initMovementGUI()
{
  movementGUI.setup("Movement");
  movementGUI.setPosition(ofPoint(630,guiPosY));
  movementGUI.add(particleSpeed.set("Particle Speed", 0, 0, 1));
  movementGUI.add(sameSpring.set("Same Spring",0,0,1));
  movementGUI.add(sameFriction.set("Same Friction", 0, 0, 1));
  movementGUI.add(repulsionForce.set("Repulsion Force", 0, 0, 1));
  movementGUI.add(repulsionFromTarget.set("Repulsion From Target", false));
  particleSpeed.addListener(this, &MainGUI::particleSpeedChanged);
  sameSpring.addListener(this, &MainGUI::sameSpringChanged);
  sameFriction.addListener(this, &MainGUI::sameFrictionChanged);
  repulsionForce.addListener(this, &MainGUI::repulsionForceChanged);
  syncMovementGUI.setup((ofParameterGroup&)movementGUI.getParameter(),6663,host,port);
}

void MainGUI::particleSpeedChanged(float & value)
{
  if(timeline.getIsPlaying())
  {
    ofxOscMessage message;
    message.setAddress("/Movement/Particle_Speed");
    message.addFloatArg(value);
    sender.sendMessage(message);
  }
}

void MainGUI::sameSpringChanged(float & value)
{
  if(timeline.getIsPlaying())
  {
    ofxOscMessage message;
    message.setAddress("/Movement/Same_Spring");
    message.addFloatArg(value);
    sender.sendMessage(message);
  }
}

void MainGUI::sameFrictionChanged(float & value)
{
  if(timeline.getIsPlaying())
  {
    ofxOscMessage message;
    message.setAddress("/Movement/Same_Friction");
    message.addFloatArg(value);
    sender.sendMessage(message);
  }
}

void MainGUI::repulsionForceChanged(float & value)
{
  if(timeline.getIsPlaying())
  {
    ofxOscMessage message;
    message.setAddress("/Movement/Repulsion_Force");
    message.addFloatArg(value);
    sender.sendMessage(message);
  }
}

void MainGUI::minPerimeterChanged(float & value)
{
  if(timeline.getIsPlaying())
  {
    ofxOscMessage message;
    message.setAddress("/Effect/Triangles/Min_Perimeter");
    message.addFloatArg(value);
    sender.sendMessage(message);
  }
}

void MainGUI::maxPerimeterChanged(float & value)
{
  if(timeline.getIsPlaying())
  {
    ofxOscMessage message;
    message.setAddress("/Effect/Triangles/Max_Perimeter");
    message.addFloatArg(value);
    sender.sendMessage(message);
  }
}

void MainGUI::minLineDistanceChanged(float & value)
{
  if(timeline.getIsPlaying())
  {
    ofxOscMessage message;
    message.setAddress("/Effect/Connect_Lines/Min_Line_Distance");
    message.addFloatArg(value);
    sender.sendMessage(message);
  }
}

void MainGUI::maxLineDistanceChanged(float & value)
{
  if(timeline.getIsPlaying())
  {
    ofxOscMessage message;
    message.setAddress("/Effect/Connect_Lines/Max_Line_Distance");
    message.addFloatArg(value);
    sender.sendMessage(message);
  }
}

void MainGUI::initShaderGUI()
{
  shaderGUI.setup("Shaders");
  shaderGUI.setPosition(ofPoint(840,guiPosY));
  pixelShader.setName("Pixel Shader");
  pixelShader.add(pixelEffect.set("Pixel Effect", false));
  pixelShader.add(halfPixelEffect.set("Half Pixel Effect", false));
  pixelShader.add(samePixelSize.set("Same Pixel Size", false));
  pixelShader.add(pixelSizeLimit.set("Pixel Size Limit", 0, 0, 1));
  pixelShader.add(pixelWidth.set("Pixel Width",0,0,1));
  pixelShader.add(pixelHeight.set("Pixel Height", 0, 0, 1));
  shaderGUI.add(pixelShader);
  syncShaderGUI.setup((ofParameterGroup&)shaderGUI.getParameter(),6664,host, port);
}

void MainGUI::windowResized(int w, int h)
{
  
}


