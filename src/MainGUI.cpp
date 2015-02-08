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
  initSVGs();
  initGeneralGUI();
  initGraphicGUI();
  initEffecGUI();
  initMovementGUI();
  initShaderGUI();
  initTimeline();
  sender.setup( "127.0.0.1",PORT);
  path = new ofPath();
  path->setFilled(false);
  path->newSubPath();
  path->setStrokeWidth(1);
  initEvents();
}

void MainGUI::draw()
{
  path->draw(0, 0);
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

void MainGUI::initSVGs()
{
  svgUrl.push_back("svg/1.svg");
  svgUrl.push_back("svg/2.svg");
  svgUrl.push_back("svg/3.svg");
  svgUrl.push_back("svg/4.svg");
  svgUrl.push_back("svg/5.svg");
  svgUrl.push_back("svg/6.svg");
  svgUrl.push_back("svg/7.svg");
  svgUrl.push_back("svg/8.svg");
  svgUrl.push_back("svg/9.svg");
  svgUrl.push_back("svg/10.svg");
  svgUrl.push_back("svg/11.svg");
  svgUrl.push_back("svg/12.svg");
  svgUrl.push_back("svg/13.svg");
  svgUrl.push_back("svg/14.svg");
  svgUrl.push_back("svg/15.svg");
  svgUrl.push_back("svg/16.svg");
  svgUrl.push_back("svg/17.svg");
  svgUrl.push_back("svg/18.svg");
  svgUrl.push_back("svg/19.svg");
  svgUrl.push_back("svg/19.svg");
  svgUrl.push_back("svg/20.svg");
  svgUrl.push_back("svg/21.svg");
}

void MainGUI::loadGeometric()
{
  clear();
  path->newSubPath();
  ofPoint tempPoint;
  switch(geomId)
  {
    case 0:
      for(int a = 0; a < 360; a += 10)
      {
        tempPoint.x = (ofGetWindowWidth() * .5) + sin(ofDegToRad(a)) *  geomParam1 * 10;
        tempPoint.y = (ofGetWindowHeight() * .5) + cos(ofDegToRad(a)) *  geomParam1 * 10;
        path->lineTo(tempPoint.x, tempPoint.y);
        addSinglePoint(tempPoint.x, tempPoint.y);
      }
      break;
    case 1:     // Spirale
      for(int a = 0; a < 360 * 10; a += 10)
      {
        tempPoint.x = (ofGetWindowWidth() * .5) + sin(ofDegToRad(a)) * geomParam2 * (a / (geomParam1 * PI));
        tempPoint.y = (ofGetWindowHeight() * .5) + cos(ofDegToRad(a)) * geomParam2 * (a / (geomParam1 * PI));
        path->lineTo(tempPoint.x, tempPoint.y);
        addSinglePoint(tempPoint.x, tempPoint.y);
      }
      break;
    case 2:     // Curva a farfalla
      for(int a = 0; a < 360 * 5; a += 10)
      {
        tempPoint.x = (ofGetWindowWidth() * .5) + (sin(ofDegToRad(a)) * ( pow(2, cos(ofDegToRad(a))) - 2 * cos(4*ofDegToRad(a)) - pow(sin(ofDegToRad(a)/12), 5) )) * 100;
        tempPoint.y = (ofGetWindowHeight() * .5) + (cos(ofDegToRad(a)) * ( pow(2, cos(ofDegToRad(a))) - 2 * cos(4*ofDegToRad(a)) - pow(sin(ofDegToRad(a)/12), 5) )) * 100;
        path->lineTo(tempPoint.x, tempPoint.y);
        addSinglePoint(tempPoint.x, tempPoint.y);
      }
      break;
    case 3:     // Curva a farfalla
      for(int a = 0; a < 360 * 5; a += 10)
      {
        tempPoint.x = (ofGetWindowWidth() * .5) + geomParam1 * 10 * sin(ofDegToRad(a)) / ( 1 + (sin(ofDegToRad(a)) * sin(ofDegToRad(a))));
        tempPoint.y = (ofGetWindowHeight() * .5) + geomParam1 * 10 * sin(ofDegToRad(a)) * cos(ofDegToRad(a)) / ( 1 + (sin(ofDegToRad(a)) * sin(ofDegToRad(a))));
        path->lineTo(tempPoint.x, tempPoint.y);
        addSinglePoint(tempPoint.x, tempPoint.y);
      }
      break;
    default:
      for(int a = 0; a < 400; a += 1)
      {
        tempPoint.x = ofGetWindowWidth() *  ofRandom(0,1);
        tempPoint.y = ofGetWindowHeight() *  ofRandom(0,1);
        path->lineTo(tempPoint.x, tempPoint.y);
        addSinglePoint(tempPoint.x, tempPoint.y);
      }
      break;
  }
  path->close();
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
  {
    guiVisible = !guiVisible;
  }
//    shiftMode = false;
}

void MainGUI::mouseReleased(ofMouseEventArgs &e)
{
  int x = e.x;
  int y = e.y;
  path->close();
//  straightLineMode = false;
//  straightDirection = "";
//  pointStraight.x = -100;
//  pointStraight.y = -100;
}

void MainGUI::mousePressed(ofMouseEventArgs &e)
{
  int x = e.x;
  int y = e.y;
  path->newSubPath();
}

void MainGUI::mouseDragged(ofMouseEventArgs &e)
{
  /*
  if(shiftMode)
  {
    if(pointStraight.x < 0 && straightLineMode)
    {
      if( abs(startPointStraight.x - e.x) >=2 || abs(startPointStraight.y - e.y) >=2  )
      {
        //  cout << "dentoo" << endl;
        if(startPointStraight.x == e.x)
          straightDirection = "vertical";
        else //if(e.y > (startPointStraight.y) || e.y < (startPointStraight.y))
          straightDirection = "horizontal";
        
        
        pointStraight.x = e.x;
        pointStraight.y = e.y;
      }
    }
    if(!straightLineMode)
    {
      // cout << "dentoo 2" << endl;
      // cout << pointStraight.x << endl,
      straightLineMode = true;
      startPointStraight.x = e.x;
      startPointStraight.y = e.y;
    }
  }
  */
  
  if(guiVisible)
    return;
  int x = e.x;
  int y = e.y;
  
  /*
  if(straightDirection == "horizontal")
    y = pointStraight.y;
  else if(straightDirection == "vertical")
    x = pointStraight.x;
  */
  
  
  
//  if(!cleanMode)
//  {
    path->lineTo(x, y);
    addSinglePoint(x, y);
//  }
//  else
//  {
//    cleanPosition.x = x;
//    cleanPosition.y = y;
//  }
  
  if(directDraw) //&&!cleanMode)
    sendSinglePoint(points[points.size() - 1]);
//  if(cleanMode&&points.size() > 0)
//    this->sendSinglePoint(x, y);
}

void MainGUI::clear()
{
  path->close();
  path->clear();
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
  ofxOscMessage m;
  m.setAddress( "/addSinglePoint" );
  m.addFloatArg( tempPoint->x  / ofGetWindowWidth() );
  m.addFloatArg( tempPoint->y  / ofGetWindowHeight() );
//  if(cleanMode)
//    m.setAddress( "/removePoint" );
  // if(touchBridge)
  //cout << "send single point 1 " << endl;
 // senderToTouch.sendMessage( m );
  if(directDraw)
    sender.sendMessage( m );
}

void MainGUI::sendSinglePoint(float x, float y)
{
  ofxOscMessage m;
  m.setAddress( "/addSinglePoint" );
  m.addFloatArg( x  / ofGetWindowWidth() );
  m.addFloatArg( y  / ofGetWindowHeight() );
  //if(cleanMode)
   // m.setAddress( "/removePoint" );
  // if(touchBridge)
  //cout << "send single point 1 " << endl;
  //senderToTouch.sendMessage( m );
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
  vectorGroup.add(vectorId.set("Vector id", 0, 0, svgUrl.size()-1));
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
  loadFromSVG(svgUrl[vectorId]);
}

void MainGUI::vectorChanged(int & value)
{
  if(!directDraw)
    loadFromSVG(svgUrl[value]);
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

void MainGUI::loadFromSVG(string url)
{
  //url = "21.svg";
  this->clear();
  path->newSubPath();
  ofPoint tempPoint;
  
  ofPoint max;
  ofPoint min;
  
  max.x = -1000;
  max.y = -1000;
  
  min.x = 10000;
  min.y = 10000;
  
  
  ofxSVG* mySVG;
  mySVG = new ofxSVG();
  mySVG->load(url);
  
  ofPolyline newPoly;
  
  ofPath newPath;
  float totPath = mySVG->getNumPath();
  
  for(int a = 0; a < totPath; a++)
  {
    int totOutline = mySVG->getPathAt(a).getOutline().size();
    ofPath tempPath = mySVG->getPathAt(a);
    for(int i =0; i < totOutline; i++)
    {
      ofPolyline tempPolyline = tempPath.getOutline()[i];
      int totVertex = tempPolyline.getVertices().size();
      for(int z = 0; z < totVertex;z++)
      {
        if(mySVG->getPathAt(a).getOutline()[i].getVertices()[z].x > max.x)
          max.x = mySVG->getPathAt(a).getOutline()[i].getVertices()[z].x;
        else if(mySVG->getPathAt(a).getOutline()[i].getVertices()[z].y > max.y)
          max.y = mySVG->getPathAt(a).getOutline()[i].getVertices()[z].y;
        
        if(mySVG->getPathAt(a).getOutline()[i].getVertices()[z].x < min.x)
          min.x = mySVG->getPathAt(a).getOutline()[i].getVertices()[z].x;
        else if(mySVG->getPathAt(a).getOutline()[i].getVertices()[z].y < min.y)
          min.y = mySVG->getPathAt(a).getOutline()[i].getVertices()[z].y;
        newPoly.addVertex(mySVG->getPathAt(a).getOutline()[i].getVertices()[z]);
      }
    }
  }

  
  ofPoint newPosition;
  newPosition.x = (ofGetWindowWidth() - (max.x - min.x)) * .5;
  newPosition.y = (ofGetWindowHeight() - (max.y - min.y)) * .5;
  
  newPoly.simplify();
  
  for(int a = 0; a <  newPoly.getVertices().size(); a++)
  {
    tempPoint.x = newPoly[a].x + newPosition.x;
    tempPoint.y = newPoly[a].y + newPosition.y;
    
    path->lineTo(tempPoint.x, tempPoint.y);
    addSinglePoint(tempPoint.x, tempPoint.y);
  }
  
  delete mySVG;
  path->close();
}



void MainGUI::clearChanged()
{;
  clear();
}

void MainGUI::clearAllChanged()
{
//  clearPoints = false;
//  clearAll = false;
  clear();
  ofxOscMessage message;
  message.clear();
  message.setAddress( "/clear" );
//  if(clearAll)
//  {
    sender.sendMessage(message);
//  }
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

