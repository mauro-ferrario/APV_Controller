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
  guiVisible = false;
  initGeneralGUI();
  initGraphicGUI();
  initEffecGUI();
  initMovementGUI();
  initShaderGUI();
  initTimeline();
  initEvents();
  initOSC();
}

void MainGUI::initOSC()
{
  sender.setup( "127.0.0.1",PORT);
}

void MainGUI::draw()
{
  ofSetColor(255);
  drawPoints();
  if(guiVisible)
  {
    ofPushStyle();
    ofSetColor(0);
    ofRect(0,0,ofGetWindowWidth(), 224);
    ofPopStyle();
    timeline.draw();
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
  
  int cont = 0;
  while(it != points.end())
  {
    if(cont > 0)
    {
      prevIt = it - 1;
      ofLine((*it)->x, (*it)->y, (*prevIt)->x, (*prevIt)->y);
    }
    cont++;
    it++;
  }
}

void MainGUI::loadFromSVG(int id)
{
  clear();
  VectorDraw::loadFromSVG(this, id, "svg/");
}

void MainGUI::loadGeometric()
{
  clear();
  GeometricDraw::loadGeometric(this, geomId, geomParam1, geomParam2);
}

void MainGUI::initEvents()
{
  ofAddListener(ofEvents().keyReleased, this, &MainGUI::keyReleased);
//  ofAddListener(ofEvents().keyPressed, this, &MainGUI::keyPressed);
  ofAddListener(ofEvents().mouseDragged, this, &MainGUI::mouseDragged);
  ofAddListener(ofEvents().mousePressed, this, &MainGUI::mousePressed);
  ofAddListener(ofEvents().mouseReleased, this, &MainGUI::mouseReleased);
}

void MainGUI::keyReleased (ofKeyEventArgs &e)
{
  char key = e.key;
  if(key == ' ')
    guiVisible = !guiVisible;
}

void MainGUI::mouseReleased(ofMouseEventArgs &e)
{
}

void MainGUI::mousePressed(ofMouseEventArgs &e)
{
}

void MainGUI::mouseDragged(ofMouseEventArgs &e)
{
  if(guiVisible)
    return;
  int x = e.x;
  int y = e.y;
  addSinglePoint(x, y);
  if(directDraw)
    sendSinglePoint(points[points.size() - 1]);
}

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
  m.addFloatArg( x  / ofGetWindowWidth() );
  m.addFloatArg( y  / ofGetWindowHeight() );
  if(directDraw)
    sender.sendMessage( m );
}

void MainGUI::update()
{
  ofColor newColor;
  newColor.r = timeline.getValue("Color r");
  newColor.g = timeline.getValue("Color g");
  newColor.b = timeline.getValue("Color b");
  color = newColor;
  syncGeneralGUI.update();
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
}

void MainGUI::initGeneralGUI()
{
  generalGUI.setup("General");
  generalGUI.setPosition(ofPoint(0,230));
  generalGUI.add(visualIP.setup("Visual IP", ""));
  generalGUI.add(toggleFullscreen.setup("Fullscreen"));
  generalGUI.add(secureLimit.setup("Secure Limit",true));
  generalGUI.add(visualFrameRate.setup("Visual Framerate",""));
  volumeGroup.setName("Volume");
  volumeGroup.add(manualInvert.set("Manual Invert", false));
  volumeGroup.add(forceInvert.set("Force Invert", false));
  volumeGroup.add(volumeInvertLimit.set("Volume Invert Limit", 0, 0, 1));
  generalGUI.add(volumeGroup);
  syncGeneralGUI.setup((ofParameterGroup&)generalGUI.getParameter(),6667,"localhost",12345);
}

void MainGUI::initGraphicGUI()
{
  graphicGUI.setup("Graphic");
  graphicGUI.setPosition(ofPoint(210,230));
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
}

void MainGUI::loadSvgChanged(bool & value)
{
  loadVector = false;
  loadFromSVG(vectorId);
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
}

void MainGUI::geomChanged(int & value)
{
  if(!directDraw)
    loadGeometric();
}

void MainGUI::clearChanged()
{;
  clear();
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
  effecGUI.setPosition(ofPoint(420,230));
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
  connectLinesGroup.add(lineRangeLimit.set("Line Range Limit", 0, 0, 1));
  effecGUI.add(connectLinesGroup);
}

void MainGUI::initMovementGUI()
{
  movementGUI.setup("Movement");
  movementGUI.setPosition(ofPoint(630,230));
  movementGUI.add(particleSpeed.set("Particle Speed", 0, 0, 1));
  movementGUI.add(sameSpring.set("Same Spring",0,0,1));
  movementGUI.add(sameFriction.set("Same Friction", 0, 0, 1));
  movementGUI.add(repulsionForce.set("Repulsion Force", 0, 0, 1));
  movementGUI.add(repulsionFromTarget.set("Repulsion From Target", false));
}

void MainGUI::initShaderGUI()
{
  shaderGUI.setup("Shaders");
  shaderGUI.setPosition(ofPoint(840,230));
  pixelShader.setName("Pixel Shader");
  pixelShader.add(pixelEffect.set("Pixel Effect", false));
  pixelShader.add(halfPixelEffect.set("Half Pixel Effect", false));
  pixelShader.add(samePixelSize.set("Same Pixel Size", false));
  pixelShader.add(pixelSizeLimit.set("Pixel Size Limit", 0, 0, 1));
  pixelShader.add(pixelWidth.set("Pixel Width",0,0,1));
  pixelShader.add(pixelHeight.set("Pixel Height", 0, 0, 1));
  shaderGUI.add(pixelShader);
}

