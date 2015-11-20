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
  canChangePoints       = true;
  timerCanChangePoints  = 0;
  readXMLSettings();
  guiVisible            = true;
#ifdef USE_TIMELINE
  timelineVisible = true;
  initTimeline();
  guiPosY = timeline.getHeight()+10;
#endif
  initGUI();
  initEvents();
  initOSC();
  directDraw            = true;
  drawProp              = outputSize.x/outputSize.y;
  
  cleanPointers();
  setupOSCPointers();
}

void MainGUI::setupOSCPointers()
{
  setupMidiOSCPointers();
}

void MainGUI::setupMidiOSCPointers()
{
  mapToFloatValue["/midi/cc16/1"] = &minPerimeter;
  mapToFloatValue["/midi/cc17/1"] = &maxPerimeter;
  mapToFloatValue["/midi/cc18/1"] = &triangleLimit;
  
  mapToFloatValue["/midi/cc19/1"] = &minLineDistance;
  mapToFloatValue["/midi/cc20/1"] = &maxLineDistance;
  
  mapToFloatValue["/midi/cc21/1"] = &oscRed;
  mapToFloatValue["/midi/cc22/1"] = &oscGreen;
  mapToFloatValue["/midi/cc23/1"] = &oscBlue;
  
  mapToFloatValue["/midi/cc0/1"] = &particleSpeed;
  mapToFloatValue["/midi/cc1/1"] = &sameSpring;
  mapToFloatValue["/midi/cc2/1"] = &sameFriction;
  mapToFloatValue["/midi/cc3/1"] = &repulsionForce;
  mapToFloatValue["/midi/cc5/1"] = &fakeFlowField.force;
  mapToFloatValue["/midi/cc6/1"] = &fakePerlin.speed;
  mapToFloatValue["/midi/cc7/1"] = &fakePerlin.force;
}

void MainGUI::initGUI()
{
  initGeneralGUI();
  initGraphicGUI();
  initEffecGUI();
  initMovementGUI();
  initShaderGUI();
}

void MainGUI::cleanPointers()
{
  clearMidiPointers();
}

void MainGUI::clearMidiPointers()
{
  mapToFloatValue["/midi/cc16/1"] = NULL;
  mapToFloatValue["/midi/cc17/1"] = NULL;
  mapToFloatValue["/midi/cc18/1"] = NULL;
  
  mapToFloatValue["/midi/cc19/1"] = NULL;
  mapToFloatValue["/midi/cc20/1"] = NULL;
  
  mapToFloatValue["/midi/cc21/1"] = NULL;
  mapToFloatValue["/midi/cc22/1"] = NULL;
  mapToFloatValue["/midi/cc23/1"] = NULL;
  
  mapToFloatValue["/midi/cc0/1"] = NULL;
  mapToFloatValue["/midi/cc1/1"] = NULL;
  mapToFloatValue["/midi/cc2/1"] = NULL;
  mapToFloatValue["/midi/cc3/1"] = NULL;
  
  mapToFloatValue["/midi/cc5/1"] = NULL;
  mapToFloatValue["/midi/cc6/1"] = NULL;
  mapToFloatValue["/midi/cc7/1"] = NULL;
}

void MainGUI::readXMLSettings()
{
  ofxXmlSettings xml;
  xml.load("mySettings.xml");
  outputSize.x = xml.getValue("controller:output:width", 0);
  outputSize.y = xml.getValue("controller:output:height", 0);
  host = xml.getValue("controller:output:host", "");
  port = xml.getValue("controller:output:port", 0);
}

void MainGUI::initOSC()
{
  sender.setup(host, port);
  receiver.setup(6668);
}

void MainGUI::draw()
{
  ofPushStyle();
  ofSetColor(100,100);
  drawArea.width = ofGetWindowWidth();
  drawArea.height = drawArea.width/drawProp;
  drawArea.x = 0;
  drawArea.y = (ofGetWindowHeight() - drawArea.height);
  ofRect(drawArea);
  ofPopStyle();
  
  ofSetColor(255);
  drawPoints();
#ifdef USE_TIMELINE
  if(timelineVisible)
  {
    ofPushStyle();
    ofSetColor(0);
    ofRect(0,0,ofGetWindowWidth(), timeline.getHeight());
    ofPopStyle();
    timeline.draw();
  }
#endif
  if(guiVisible)
  {
    drawGUI();
  }
}

void MainGUI::drawGUI()
{
  generalGUI.draw();
  graphicGUI.draw();
  effecGUI.draw();
  movementGUI.draw();
  shaderGUI.draw();  
}

void MainGUI::drawPoints()
{
  vector<ofPoint*>::iterator it = points.begin();
  vector<ofPoint*>::iterator prevIt;
  
  ofPushMatrix();
  ofVec2f offset;
  offset.x = 0;
  offset.y = drawArea.y;
  //ofTranslate(offset);
  
  int cont = 0;
  while(it != points.end())
  {
    if(cont > 0)
    {
      prevIt = it - 1;
//      ofLine((*it)->x/outputSize.x*drawArea.width, (*it)->y/outputSize.y*drawArea.height, (*prevIt)->x/outputSize.x*drawArea.width, (*prevIt)->y/outputSize.y*drawArea.height);
      ofLine((*it)->x, (*it)->y, (*prevIt)->x, (*prevIt)->y);
    }
    cont++;
    it++;
  }
  ofPopMatrix();
}

void MainGUI::loadFromSVG(int id)
{
  if(freezeDrawAndClean)
    return;
  if(addToDraw == 0)
    clear();
  PolyLineException* exception = new PolyLineException();
  VectorDraw::loadFromSVG(this, id, "svg/", outputSize, exception);
  delete exception;
  exception = NULL;
}

void MainGUI::loadGeometric()
{
  if(freezeDrawAndClean)
    return;
  if(addToDraw == 0)
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
  if(key == 's')
    savePreset(preset);
  if(key == 'l')
    loadPreset(preset);
  if(key == 'f')
    ofToggleFullscreen();
#ifdef USE_TIMELINE
  if(e.key == OF_KEY_DOWN)
    timelineVisible = !timelineVisible;
  if(e.key == ' ')
    timeline.togglePlay();
#endif
}

void MainGUI::moveToNextPreset()
{
  preset = ofClamp(preset+1, 0, preset.getMax());
  loadPreset(preset);
}

void MainGUI::moveToPrevPreset()
{
  preset = ofClamp(preset-1, 0, preset.getMax());
  loadPreset(preset);
}

void MainGUI::savePreset(int idPreset)
{
  freezeDrawAndClean = true;
  bool prevDirectDraw;
  prevDirectDraw = directDraw;
  saveGUIsPreset(idPreset);
  directDraw = prevDirectDraw;
  freezeDrawAndClean = false;
}

void MainGUI::saveGUIsPreset(int idPreset)
{
  generalGUI.saveToFile("guiPreset/"+ofToString(idPreset)+"/generalGUI_"+ofToString(idPreset)+".xml");
  graphicGUI.saveToFile("guiPreset/"+ofToString(idPreset)+"/graphicGUI_"+ofToString(idPreset)+".xml");
  effecGUI.saveToFile("guiPreset/"+ofToString(idPreset)+"/effecGUI_"+ofToString(idPreset)+".xml");
  movementGUI.saveToFile("guiPreset/"+ofToString(idPreset)+"/movementGUI_"+ofToString(idPreset)+".xml");
  shaderGUI.saveToFile("guiPreset/"+ofToString(idPreset)+"/shaderGUI_"+ofToString(idPreset)+".xml");
}

void MainGUI::loadPreset(int idPreset)
{
  freezeDrawAndClean = true;
  bool prevDirectDraw;
  prevDirectDraw = directDraw;
  loadGUIsPreset(idPreset);
  directDraw = prevDirectDraw;
  freezeDrawAndClean = false;
  if(loadGeomAfterPreset)
    loadGeometric();
  if(loadVectorAfterPreset)
    loadFromSVG(vectorId);
}

void MainGUI::loadGUIsPreset(int idPreset)
{
  generalGUI.loadFromFile("guiPreset/"+ofToString(idPreset)+"/generalGUI_"+ofToString(idPreset)+".xml");
  graphicGUI.loadFromFile("guiPreset/"+ofToString(idPreset)+"/graphicGUI_"+ofToString(idPreset)+".xml");
  effecGUI.loadFromFile("guiPreset/"+ofToString(idPreset)+"/effecGUI_"+ofToString(idPreset)+".xml");
  movementGUI.loadFromFile("guiPreset/"+ofToString(idPreset)+"/movementGUI_"+ofToString(idPreset)+".xml");
  shaderGUI.loadFromFile("guiPreset/"+ofToString(idPreset)+"/shaderGUI_"+ofToString(idPreset)+".xml");
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
  if(guiVisible)
    return;
#ifdef USE_TIMELINE
  if(timelineVisible)
    return;
#endif
  int x = e.x;
  int y = e.y;
  addSinglePoint(x, y);
  if(directDraw)
    sendSinglePoint(points[points.size() - 1]);
}
#endif

void MainGUI::clear()
{
  cout << "CLEAN POINT " << freezeDrawAndClean << endl;
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


void MainGUI::processOSCMessage(ofxOscMessage& message)
{
  string messageAddress = message.getAddress();
  if(messageAddress == "/framerate")
  {
    visualFrameRate = ofToString(message.getArgAsFloat(0));
  }
  if(messageAddress == "/midi/cc43/1")
  {
    if(message.getArgAsInt32(0)==0)
    {
      moveToPrevPreset();
    }
  }
  if(messageAddress == "/midi/cc44/1")
  {
    if(message.getArgAsInt32(0)==0)
    {
      moveToNextPreset();
    }
  }
  if(mapToFloatValue[messageAddress])
  {
    *mapToFloatValue[messageAddress] = message.getArgAsFloat(0);
    if(messageAddress == "/midi/cc6/1")
      *mapToFloatValue[messageAddress] = ofMap(message.getArgAsFloat(0), 0, 1, 0,.1);
    
    if(messageAddress == "/midi/cc21/1" || messageAddress == "/midi/cc22/1" || messageAddress == "/midi/cc23/1")
    {
      ofColor newColor = ofColor(oscRed*255, oscGreen*255, oscBlue*255);
      color = newColor;
    }
  }
}

void MainGUI::update()
{
  while(receiver.hasWaitingMessages())
  {
    ofxOscMessage message;
    receiver.getNextMessage(&message);
    processOSCMessage(message);
  }
  
#ifdef USE_TIMELINE
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
    repulsionForce = timeline.getValue("Repulsion Force");
    scaleFactor = timeline.getValue("Scale");
    audioInvertCoefficent = timeline.getValue("Audio Invert Coefficent");
    
    
    minPerimeter = timeline.getValue("Min Perimeter");
    maxPerimeter = timeline.getValue("Max Perimeter");
    minLineDistance = timeline.getValue("Min Line Distance");
    maxLineDistance = timeline.getValue("Max Line Distance");
  }
#endif
  
  if(windXNoise||windYNoise)
  {
    float fWindXNoise = wind->x;
    float fWindYNoise = wind->y;
    if(windXNoise)
    {
      fWindXNoise = (ofSignedNoise(sin(ofGetFrameNum()*.005)) * 8);
    }
    if(windYNoise)
    {
      fWindYNoise = ofNoise(ofGetFrameNum());
    }
    wind = ofVec2f(fWindXNoise,fWindYNoise);
  }
  
  syncGeneralGUI.update();
  //syncGraphiclGUI.update();
  syncEffecGUI.update();
  syncMovementGUI.update();
  syncShaderGUI.update();
  if(!canChangePoints)
  {
    timerCanChangePoints++; 
    if(timerCanChangePoints > 5)
    {
      canChangePoints = true;
      timerCanChangePoints = 0;
    }
  }
}

#ifdef USE_TIMELINE
void MainGUI::initTimeline()
{
  timeline.setup();
  timeline.setSpacebarTogglePlay(false);
  timeline.setDurationInSeconds(76);
  timeline.setLoopType(OF_LOOP_NORMAL);
  timeline.setHeight(200);
  timeline.addCurves("Color r");
  timeline.addCurves("Color g");
  timeline.addCurves("Color b");
  timeline.addCurves("Particle Speed");
  timeline.addCurves("Same Spring");
  timeline.addCurves("Same Friction");
  timeline.addCurves("Repulsion Force");
  timeline.addCurves("Scale");
  timeline.addCurves("Audio Invert Coefficent",ofRange(0,2));
  
  timeline.addCurves("Min Perimeter");
  timeline.addCurves("Max Perimeter");
  
  timeline.addCurves("Min Line Distance");
  timeline.addCurves("Max Line Distance");
  
  timeline.addFlags("Change Command");
  ofAddListener(timeline.events().bangFired, this, &MainGUI::receivedBang);
  ofAddListener(timeline.events().playbackLooped, this, &MainGUI::playbackLoopedHandler);
  ofAddListener(timeline.events().playbackStarted, this, &MainGUI::playbackStartedHandler);
  ofAddListener(timeline.events().playbackEnded, this, &MainGUI::playbackEndedHandler);
}

void MainGUI::playbackStartedHandler(ofxTLPlaybackEventArgs& event)
{
  sendPlaybackState();
}

void MainGUI::playbackEndedHandler(ofxTLPlaybackEventArgs& event)
{
  sendPlaybackState();
}

void MainGUI::playbackLoopedHandler(ofxTLPlaybackEventArgs& event)
{
  ofxOscMessage message;
  message.setAddress("/Reset_Track");
  sender.sendMessage(message);
}

void MainGUI::sendPlaybackState()
{
  ofxOscMessage message;
  message.setAddress("/Play");
  message.addIntArg(timeline.getIsPlaying());
  sender.sendMessage(message);
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
  if(bang.flag == "AddToDraw")
  {
    addToDraw = true;
  }
  if(bang.flag == "AddToDrawFalse")
  {
    addToDraw = false;
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
  int fadeInPos = bang.flag.find("FadeIn");
  int overlayId = -1;
  ofxOscMessage fadeInOutMessage;
  if(fadeInPos >= 0)
  {
    overlayId = ofToInt(ofSplitString(bang.flag, "FadeIn")[1]);
    fadeInOutMessage.setAddress("/FadeIn");
  }
  int fadeOutPos = bang.flag.find("FadeOut");
  if(fadeOutPos >= 0)
  {
    overlayId = ofToInt(ofSplitString(bang.flag, "FadeOut")[1]);
    fadeInOutMessage.setAddress("/FadeOut");
  }
  if(fadeOutPos >= 0 || fadeInPos >= 0)
  {
    fadeInOutMessage.addIntArg(overlayId);
    sender.sendMessage(fadeInOutMessage);
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
#endif

void MainGUI::initGeneralGUI()
{
  generalGUI.setup("General");
  generalGUI.setPosition(ofPoint(0,guiPosY));
  generalGUI.add(preset.set("Preset", 0, 0, 100));
  generalGUI.add(visualIP.setup("Visual IP", ""));
  generalGUI.add(secureLimit.setup("Secure Limit",true));
//  generalGUI.add(visualFrameRate.setup("Visual Framerate",""));
#ifdef USE_TIMELINE
  generalGUI.add(togglePlayPauseTimeline.set("Play/Pause Timeline",false));
#endif
  volumeGroup.setName("Volume");
  volumeGroup.add(audioInvertCoefficent.set("Audio Invert Coefficent", 1, 0, 2));
  volumeGroup.add(manualInvert.set("Manual Invert", false));
  volumeGroup.add(forceInvert.set("Force Invert", false));
  volumeGroup.add(volumeLevel.set("Volume Level", 1, 0, 5));
  generalGUI.add(volumeGroup);
  audioInvertCoefficent.addListener(this, &MainGUI::audioInvertCoefficentChanged);
#ifdef USE_TIMELINE
  togglePlayPauseTimeline.addListener(this, &MainGUI::togglePlayPauseTimelineChanged);
#endif
  syncGeneralGUI.setup((ofParameterGroup&)generalGUI.getParameter(),6660, host,port);
}

void MainGUI::initGraphicGUI()
{
  graphicGUI.setup("Graphic");
  graphicGUI.setPosition(ofPoint(210,guiPosY));
  graphicGUI.add(directDraw.setup("Direct draw", true));
  graphicGUI.add(addToDraw.setup("Add to draw", false));
  /*
  graphicGUI.add(rotationSpeed.setup("Rotation speed", 0, 0, 1));
  graphicGUI.add(clockwiseRotation.setup("Clockwise rotation", true));
  graphicGUI.add(
    */             
  graphicGUI.add(bClear.setup("Clear"));
  graphicGUI.add(clearAll.setup("Clear All"));
  vectorGroup.setName("Vector");
  vectorGroup.add(loadVector.set("Load vector", false));
  vectorGroup.add(loadVectorAfterPreset.set("Load vector after preset", false));
  vectorGroup.add(vectorId.set("Vector id", 0, 0, VectorDraw::getTotSvgs("svg/")-1));
  graphicGUI.add(vectorGroup);
  geomGroup.setName("Geom");
  geomGroup.add(loadGeom.set("Load geom", false));
  geomGroup.add(loadGeomAfterPreset.set("Load geom after preset", false));
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
  addToDraw.addListener(this, &MainGUI::addToDrawChanged);
}
                  
void MainGUI::audioInvertCoefficentChanged(float & value)
  {
    ofxOscMessage message;
    message.setAddress("/General/Audio_Invert_Coefficent");
    message.addFloatArg(value);
    sender.sendMessage(message);
}

void MainGUI::directDrawChanged(bool & value)
{
  if(value == 1 && !freezeDrawAndClean)
    sendManyPoints();
}

void MainGUI::addToDrawChanged(bool & value)
{
  addToDraw = value;
}

void MainGUI::loadSvgChanged(bool & value)
{
  if(freezeDrawAndClean)
    return;
  loadVector = false;
  if(canChangePoints)
  {
    loadFromSVG(vectorId);
    if(directDraw)
    {
      canChangePoints = false;
      sendManyPoints();
    }
  }
}

void MainGUI::vectorChanged(int & value)
{
  if(!directDraw&&canChangePoints)
    loadFromSVG(value);
}

void MainGUI::loadGeomChanged(bool & value)
{
  loadGeom = false;
  loadGeometric();
  if(directDraw&&canChangePoints)
    sendManyPoints();
}

void MainGUI::geomChanged(int & value)
{
  if(!directDraw)
    loadGeometric();
}

#ifdef USE_TIMELINE
void MainGUI::togglePlayPauseTimelineChanged(bool & value)
{
  togglePlayPauseTimeline = false;
  timeline.togglePlay();
  sendPlaybackState();
}
#endif

void MainGUI::clearChanged()
{
  if(!freezeDrawAndClean)
    clear();
}

void MainGUI::sendGeometric()
{
  }

void MainGUI::sendManyPoints()
{
  if(timerCanChangePoints != 0)
  {
    return;
  }
  timerCanChangePoints = 1;
  canChangePoints = false;
  ofxOscMessage message;
  message.clear();
  message.setAddress( "/loadShape" );
  int cont = 0;
  int contPoint = 0;
  message.addIntArg(points.size());
  cont = -1;
  for(int a = 0; a < points.size(); a++)
  {
    if(a%MAX_OSC_VALUES == 0 && a >= 0)
    {
      message.addFloatArg( points[a]->x  / outputSize.x );
      message.addFloatArg( points[a]->y  / outputSize.y );
      sender.sendMessage(message);
      cont++;
      message.clear();
      // Controllare che il load SVG non venga richiamto più volte
      message.setAddress( "/addPoint" );
      int pointToSend = (points.size() - a);
      if(pointToSend >= MAX_OSC_VALUES)
        pointToSend = MAX_OSC_VALUES;
      message.addFloatArg(pointToSend);
      contPoint = 0;
    }
    message.addFloatArg( points[a]->x  / outputSize.x );
    message.addFloatArg( points[a]->y  / outputSize.y );
    contPoint++;
  }
  if(points.size()%MAX_OSC_VALUES != 0)
  {
    sender.sendMessage( message );
  }
}


void MainGUI::clearAllChanged()
{
  if(!freezeDrawAndClean)
  {
    clear();
    ofxOscMessage message;
    message.clear();
    message.setAddress( "/clear" );
    sender.sendMessage(message);
  }
}

void MainGUI::initEffecGUI()
{
  effecGUI.setup("Effect");
  effecGUI.setPosition(ofPoint(420,guiPosY));
  effecGUI.add(drawPoint.setup("Draw point", false));
  effecGUI.add(drawTriangle.setup("Draw triangle", false));
  effecGUI.add(connectPrevPoint.setup("Connect to prev point", false));
  effecGUI.add(connectPoints.setup("Connect points", false));
  effecGUI.add(scaleFactor.set("Scale Factor", 0.24, 0, 1));
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
  connectLinesGroup.add(lineAlpha.set("Line Alpha", 200, 0,255));
  effecGUI.add(connectLinesGroup);
  color.addListener(this, &MainGUI::colorChanged);
  minPerimeter.addListener(this, &MainGUI::minPerimeterChanged);
  maxPerimeter.addListener(this, &MainGUI::maxPerimeterChanged);
  minLineDistance.addListener(this, &MainGUI::minLineDistanceChanged);
  maxLineDistance.addListener(this, &MainGUI::maxLineDistanceChanged);
  
  
  scaleFactor.addListener(this, &MainGUI::scaleFactorChanged);
  syncEffecGUI.setup((ofParameterGroup&)effecGUI.getParameter(),6662, host,port);
}

void MainGUI::colorChanged(ofFloatColor & newColor)
{
#ifdef USE_TIMELINE
  if(timeline.getIsPlaying())
  {
#endif
    ofxOscMessage message;
    message.setAddress("/Effect/Triangles/Color");
    message.addStringArg(ofToString(newColor.r) +", "+ofToString(newColor.g)+ ","+ofToString(newColor.b)+","+ofToString(newColor.a));
    sender.sendMessage(message);
#ifdef USE_TIMELINE
  }
#endif
}

void MainGUI::initMovementGUI()
{
  fakeFlowField.init(100, 100, 10);
  movementGUI.setup("Movement");
  movementGUI.setPosition(ofPoint(630,guiPosY));
  movementGUI.add(particleSpeed.set("Particle Speed", 0.63, 0, 1));
  movementGUI.add(sameSpring.set("Same Spring",0.26,0,1));
  movementGUI.add(sameFriction.set("Same Friction", 0.17, 0, 1));
  movementGUI.add(repulsionForce.set("Repulsion Force", 0.135, 0, 1));
  movementGUI.add(repulsionFromTarget.set("Repulsion From Target", false));
  movementGUI.add(followFlow.set("Follow Flow", false));
  movementGUI.add(enablePerlin.set("Enable Perlin", false));
  movementGUI.add(*fakeFlowField.getParameterGroup());
  movementGUI.add(*fakePerlin.getParameterGroup());
  movementGUI.add(*getWindGroup());
  particleSpeed.addListener(this, &MainGUI::particleSpeedChanged);
  sameSpring.addListener(this, &MainGUI::sameSpringChanged);
  sameFriction.addListener(this, &MainGUI::sameFrictionChanged);
  repulsionForce.addListener(this, &MainGUI::repulsionForceChanged);
  syncMovementGUI.setup((ofParameterGroup&)movementGUI.getParameter(),6663,host,port);
  fakeFlowField.bResetFlow.addListener(this, &MainGUI::resetFlowChanged);
  fakeFlowField.force.addListener(this, &MainGUI::flowForceChanged);
  followFlow.addListener(this, &MainGUI::followFlowChanged);
  enablePerlin.addListener(this, &MainGUI::enablePerlinChanged);
  
  fakePerlin.resX.addListener(this, &MainGUI::resXPerlinChanged);
  fakePerlin.resY.addListener(this, &MainGUI::resYPerlinChanged);
  fakePerlin.speed.addListener(this, &MainGUI::speedPerlinChanged);
  fakePerlin.force.addListener(this, &MainGUI::forcePerlinChanged);
}

ofParameterGroup* MainGUI::getWindGroup()
{
  if(!windParams)
  {
    windParams = new ofParameterGroup();
  }
  if(windParams->getName() == "")
  {
    windParams->setName("Wind");
    windParams->add(wind.set("Wind",ofVec2f(0,-10),ofVec2f(-50,-50),ofVec2f(50,50)));
    windParams->add(toggleWind.set("Wind actived", false));
    windParams->add(windXNoise.set("Wind x noise", false));
    windParams->add(windYNoise.set("Wind y noise", false));
    toggleWind.addListener(this, &MainGUI::toggleWindChanged);
    wind.addListener(this, &MainGUI::windChanged);
  }
  return windParams;
}

void MainGUI::windChanged(ofVec2f & value)
{
  ofxOscMessage message;
  message.clear();
  message.setAddress( "/Movement/Wind");
  message.addFloatArg(value.x);
  message.addFloatArg(value.y);
  sender.sendMessage(message);
}

void MainGUI::toggleWindChanged(bool & value)
{
  ofxOscMessage message;
  message.clear();
  message.setAddress( "/Movement/Wind/ApplyWind");
  message.addIntArg(value);
  sender.sendMessage(message);
}

void MainGUI::enablePerlinChanged(bool & value)
{
  ofxOscMessage message;
  message.clear();
  message.setAddress( "/enablePerlin" );
  message.addIntArg(value);
  sender.sendMessage(message);
}

void MainGUI::resetFlowChanged(bool & value)
{
  ofxOscMessage message;
  message.clear();
  message.setAddress( "/resetFlow" );
  sender.sendMessage(message);
  fakeFlowField.bResetFlow = false;
}

void MainGUI::followFlowChanged(bool & value)
{
//  ofxOscMessage message;
//  message.clear();
//  message.setAddress( "/FollowFlow");
//  message.addIntArg(value);
//  sender.sendMessage(message);
}

void MainGUI::flowForceChanged(float & value)
{
  //if(timeline.getIsPlaying())
  sendFloatValue("/Flow/force", value);
}

void MainGUI::sendFloatValue(string address, float value)
{
  ofxOscMessage message;
  message.setAddress(address);
  message.addFloatArg(value);
  sender.sendMessage(message);
}

void MainGUI::sendBoolValue(string address, bool value)
{
  ofxOscMessage message;
  message.setAddress(address);
  message.addIntArg(value);
  sender.sendMessage(message);
}

void MainGUI::resXPerlinChanged(float & value)
{
  sendFloatValue("/Movement/Perlin/ResX", value);
}

void MainGUI::resYPerlinChanged(float & value)
{
  sendFloatValue("/Movement/Perlin/ResY", value);
}

void MainGUI::speedPerlinChanged(float & value)
{
  sendFloatValue("/Movement/Perlin/Speed", value);
}

void MainGUI::forcePerlinChanged(float & value)
{
 sendFloatValue("/Movement/Perlin/Force", value);
}

void MainGUI::particleSpeedChanged(float & value)
{
#ifdef USE_TIMELINE
  if(timeline.getIsPlaying())
#endif
    sendFloatValue("/Movement/Particle_Speed", value);
}

void MainGUI::sameSpringChanged(float & value)
{
#ifdef USE_TIMELINE
  if(timeline.getIsPlaying())
#endif
    sendFloatValue("/Movement/Same_Spring", value);
}

void MainGUI::sameFrictionChanged(float & value)
{
#ifdef USE_TIMELINE
  if(timeline.getIsPlaying())
#endif
    sendFloatValue("/Movement/Same_Friction", value);
}

void MainGUI::repulsionForceChanged(float & value)
{
#ifdef USE_TIMELINE
  if(timeline.getIsPlaying())
#endif
    sendFloatValue("/Movement/Repulsion_Force", value);
}

void MainGUI::minPerimeterChanged(float & value)
{
#ifdef USE_TIMELINE
  if(timeline.getIsPlaying())
#endif
    sendFloatValue("/Effect/Triangles/Min_Perimeter", value);
}

void MainGUI::maxPerimeterChanged(float & value)
{
#ifdef USE_TIMELINE
  if(timeline.getIsPlaying())
#endif
    sendFloatValue("/Effect/Triangles/Max_Perimeter", value);
}

void MainGUI::minLineDistanceChanged(float & value)
{
#ifdef USE_TIMELINE
  if(timeline.getIsPlaying())
#endif
    sendFloatValue("/Effect/Connect_Lines/Min_Line_Distance", value);
}

void MainGUI::maxLineDistanceChanged(float & value)
{
#ifdef USE_TIMELINE
  if(timeline.getIsPlaying())
#endif
    sendFloatValue("/Effect/Connect_Lines/Max_Line_Distance", value);
}

void MainGUI::scaleFactorChanged(float & value)
{
#ifdef USE_TIMELINE
  if(timeline.getIsPlaying())
#endif
    sendFloatValue("/Effect/Scale_Factor", value);
}

void MainGUI::initShaderGUI()
{
  shaderGUI.setup("Shaders");
  shaderGUI.setPosition(ofPoint(840,guiPosY));
  pixelShader.setName("Pixel Shader");
  pixelShader.add(pixelEffect.set("Pixel Effect", false));
  pixelShader.add(halfPixelEffect.set("Half Pixel Effect", false));
  pixelShader.add(samePixelSize.set("Same Pixel Size", false));
  pixelShader.add(allLine.set("All line", false));
//  pixelShader.add(pixelSizeLimit.set("Pixel Size Limit", 0, 0, 1));
  pixelShader.add(pixelWidth.set("Pixel Width",0,0,1));
  pixelShader.add(pixelHeight.set("Pixel Height", 0, 0, 1));
  shaderGUI.add(pixelShader);
  syncShaderGUI.setup((ofParameterGroup&)shaderGUI.getParameter(),6664,host, port);
}

void MainGUI::windowResized(int w, int h)
{
  
}


