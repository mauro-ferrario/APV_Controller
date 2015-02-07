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
  initGeneralGUI();
  initGraphicGUI();
  initEffecGUI();
  initMovementGUI();
  initShaderGUI();
  initTimeline();
}

void MainGUI::draw()
{
  //the value of changingRadius will be different depending on the timeline
  float changingRadius = timeline.getValue("MyCircleRadius");
  //use the value for something amazing!
  ofCircle(0, 0, 10);
  //don't forget to draw your timeline so you can edit it.
  
  syncGeneralGUI.update();
  timeline.draw();
  generalGUI.draw();
  graphicGUI.draw();
  effecGUI.draw();
  movementGUI.draw();
  shaderGUI.draw();
}

void MainGUI::initTimeline()
{
  timeline.setup(); //registers events
  timeline.setDurationInSeconds(60); //sets time
  timeline.setLoopType(OF_LOOP_NORMAL); //turns the timeline to loop
  //add a tracks, etc
  timeline.addCurves("MyCircleRadius", ofRange(0, 200));
  timeline.setHeight(200);
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
  volumeGroup.add(volumeInvertLimit.set("Volume Invert Limit", 0, 0, 100));
  generalGUI.add(volumeGroup);
  syncGeneralGUI.setup((ofParameterGroup&)generalGUI.getParameter(),6667,"localhost",12345);
}

void MainGUI::initGraphicGUI()
{
  graphicGUI.setup("Graphic");
  graphicGUI.setPosition(ofPoint(210,230));
  graphicGUI.add(directDraw.setup("Direct draw", true));
  graphicGUI.add(clear.setup("Clear"));
  graphicGUI.add(clearAll.setup("Clear All"));
  vectorGroup.setName("Vector");
  vectorGroup.add(loadVector.set("Load vector", false));
  vectorGroup.add(vectorId.set("Vector id", 0, 0, 100));
  graphicGUI.add(vectorGroup);

 // graphicGUI.add(loadGeom.setup("Load geom", true));
  geomGroup.setName("Geom");
  geomGroup.add(loadGeom.set("Load geom", false));
  geomGroup.add(geomId.set("Geom id", 0, 0, 100));
  geomGroup.add(geomParam1.set("Geom param 1",0,0,100));
  geomGroup.add(geomParam2.set("Geom param 2", 0, 0, 100));
  graphicGUI.add(geomGroup);
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
  triangleGroup.add(minPerimeter.set("Min Perimeter", 0, 0, 100));
  triangleGroup.add(maxPerimeter.set("Max Perimeter",0,0,100));
  triangleGroup.add(triangleLimit.set("Triangle Limit", 0, 0, 1));
  triangleGroup.add(color.set("Color", ofFloatColor(1), ofFloatColor(0), ofFloatColor(1)));
  effecGUI.add(triangleGroup);
  connectLinesGroup.setName("Connect Lines");
  connectLinesGroup.add(minLineDistance.set("Min Line Distance", 0, 0, 100));
  connectLinesGroup.add(maxLineDistance.set("Max Line Distance",0,0,100));
  connectLinesGroup.add(lineRangeLimit.set("Line Range Limit", 0, 0, 1));
  effecGUI.add(connectLinesGroup);
}

void MainGUI::initMovementGUI()
{
  movementGUI.setup("Movement");
  movementGUI.setPosition(ofPoint(630,230));
  movementGUI.add(particleSpeed.set("Particle Speed", 0, 0, 100));
  movementGUI.add(sameSpring.set("Same Spring",0,0,100));
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
  pixelShader.add(pixelSizeLimit.set("Pixel Size Limit", 0, 0, 100));
  pixelShader.add(pixelWidth.set("Pixel Width",0,0,100));
  pixelShader.add(pixelHeight.set("Pixel Height", 0, 0, 1));
  shaderGUI.add(pixelShader);
}

