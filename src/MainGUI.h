//
//  MainGUI.h
//  APV_Controller
//
//  Created by Mauro Ferrario on 07/02/15.
//
//

#ifndef __APV_Controller__MainGUI__
#define __APV_Controller__MainGUI__

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxTimeline.h"
#include "ofxOscParameterSync.h"

class MainGUI
{
public:
                      MainGUI();
  void                init();
  void                initTimeline();
  void                initGeneralGUI();
  void                initGraphicGUI();
  void                initEffecGUI();
  void                initMovementGUI();
  void                initShaderGUI();
  ofxPanel            generalGUI;
  ofxPanel            graphicGUI;
  ofxPanel            effecGUI;
  ofxPanel            movementGUI;
  ofxPanel            shaderGUI;
  ofxTimeline         timeline;
  void                draw();
  
  ofxOscParameterSync syncGeneralGUI;


  // generalGUI
  ofxLabel            visualIP;
  ofxButton           toggleFullscreen;
  ofxToggle           secureLimit;
  ofxLabel            visualFrameRate;
  
  ofParameterGroup    volumeGroup;
  ofParameter<bool>   manualInvert;
  ofParameter<bool>   forceInvert;
  ofParameter<float>  volumeInvertLimit;
  // graphicGUI
  ofxButton           clear;
  ofxButton           clearAll;
  ofxToggle           directDraw;

  ofParameterGroup    vectorGroup;
  ofParameter<bool>   loadVector;
  ofParameter<int>    vectorId;

  ofParameterGroup    geomGroup;
  ofParameter<bool>   loadGeom;
  ofParameter<int>    geomId;
  ofParameter<float>  geomParam1;
  ofParameter<float>  geomParam2;

//  ofxParameterGroup   letterGroup;
//  ofxToggle           loadLetter;
//  ofParameter<int>    letterId;
  
  // efffecGUI
  ofxToggle           drawPoint;
  ofxToggle           drawTriangle;
  ofxToggle           connectPrevPoint;
  ofxToggle           connectPoints;
  
  ofParameterGroup    triangleGroup;
  ofParameter<bool>   sameColorTriangle;
  ofParameter<float>  minPerimeter;
  ofParameter<float>  maxPerimeter;
  ofParameter<float>  triangleLimit;
  ofParameter<ofFloatColor>  color;
  
  ofParameterGroup    connectLinesGroup;
  ofParameter<float>  minLineDistance;
  ofParameter<float>  maxLineDistance;
  ofParameter<float>  lineRangeLimit;
  
  // movementGUI
  ofParameter<float>  particleSpeed;
  ofParameter<float>  sameSpring;
  ofParameter<float>  sameFriction;
  ofParameter<float>  repulsionForce;
  ofParameter<bool>   repulsionFromTarget;
  
  
  // shaderGUI
  ofParameterGroup    pixelShader;
  ofParameter<bool>   pixelEffect;
  ofParameter<bool>   halfPixelEffect;
  ofParameter<bool>   samePixelSize;
  ofParameter<float>  pixelSizeLimit;
  ofParameter<float>  pixelWidth;
  ofParameter<float>  pixelHeight;
};

#endif /* defined(__APV_Controller__MainGUI__) */
