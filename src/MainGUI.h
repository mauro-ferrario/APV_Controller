//
//  MainGUI.h
//  APV_Controller
//
//  Created by Mauro Ferrario on 07/02/15.
//
//

#ifndef __APV_Controller__MainGUI__
#define __APV_Controller__MainGUI__

#define FREE_DRAW_MODE
//#define USE_TIMELINE
#define MAX_OSC_VALUES 301

#include "ofMain.h"
#include "ofxGui.h"
#ifdef USE_TIMELINE
  #include "ofxTimeline.h"
#endif
#include "ofxOscParameterSync.h"
#include "ofxOsc.h"
#include "ofxSvg.h"
#include "ofPath.h"
#include "GeometricDraw.h"
#include "VectorDraw.h"
#include "ofxXmlSettings.h"
#include "PolyLineException.h"
#include "GoofyFlowField.h"
#include "GoofyPerlinNoise.h"
#include<tr1/unordered_map>

class MainGUI
{
public:
                      MainGUI();
  void                init();
#ifdef USE_TIMELINE
  void                initTimeline();
#endif
  virtual void        initGUI();
  void                initGeneralGUI();
  void                initGraphicGUI();
  void                initEffecGUI();
  void                initMovementGUI();
  void                initShaderGUI();
  void                windowResized(int w, int h);
  ofxPanel            generalGUI;
  ofxPanel            graphicGUI;
  ofxPanel            effecGUI;
  ofxPanel            movementGUI;
  ofxPanel            shaderGUI;
#ifdef USE_TIMELINE
  ofxTimeline         timeline;
#endif
  void                draw();
  virtual void        update();
  void                initEvents();
  void                clear();
#ifdef FREE_DRAW_MODE
  void                mouseDragged(ofMouseEventArgs &e);
#endif
  void                mouseReleased(ofMouseEventArgs &e);
  void                mousePressed(ofMouseEventArgs &e);
  bool                guiVisible;
#ifdef USE_TIMELINE
  bool                timelineVisible;
#endif
  virtual void        sendSinglePoint(ofPoint* tempPoint);
  virtual void        sendSinglePoint(float x, float y);
  void                addSinglePoint(int x, int y);
  void                keyReleased (ofKeyEventArgs &e);
  virtual void        savePreset(int idPreset);
  virtual void        saveGUIsPreset(int idPreset);
  virtual void        loadPreset(int idPreset);
  virtual void        loadGUIsPreset(int idPreset);
  void                moveToNextPreset();
  void                moveToPrevPreset();
  void                loadGeometric();
#ifdef USE_TIMELINE
  void                togglePlayPauseTimelineChanged(bool & value);
#endif
  void                loadGeomChanged(bool & value);
  void                loadSvgChanged(bool & value);
  void                vectorChanged(int & value);
  void                geomChanged(int & value);
  
  void                particleSpeedChanged(float & value);
  void                sameSpringChanged(float & value);
  void                sameFrictionChanged(float & value);
  void                repulsionForceChanged(float & value);
  
  void                minPerimeterChanged(float & value);
  void                maxPerimeterChanged(float & value);
  void                minLineDistanceChanged(float & value);
  void                maxLineDistanceChanged(float & value);
  void                audioInvertCoefficentChanged(float & value);
  void                scaleFactorChanged(float & value);
  void                sendFloatValue(string address, float value);
  void                sendBoolValue(string address, bool value);
#ifdef USE_TIMELINE
  void                playbackLoopedHandler(ofxTLPlaybackEventArgs& event);
  void                playbackStartedHandler(ofxTLPlaybackEventArgs& event);
  void                playbackEndedHandler(ofxTLPlaybackEventArgs& event);
  void                sendPlaybackState();
#endif
  bool                canChangePoints;
  int                 timerCanChangePoints;
  
  void                colorChanged(ofFloatColor & newColor);
  void                clearChanged();
  void                clearAllChanged();
  void                loadFromSVG(int id);
#ifdef USE_TIMELINE
  void                receivedBang(ofxTLBangEventArgs& bang);
#endif
  virtual void        drawGUI();
  void                drawPoints();
  virtual void                initOSC();
  void                sendGeometric();
  void                sendManyPoints();
  void                readXMLSettings();
  vector<ofPoint*>    points;
  string              host;
  int                 port;
  
  void                directDrawChanged(bool & value);
  void                addToDrawChanged(bool & value);
  float               guiPosY;
  
  float               drawProp;
  ofVec2f             outputSize;
  ofRectangle         drawArea;
  
  ofxOscParameterSync syncGeneralGUI;
  ofxOscParameterSync syncEffecGUI;
  ofxOscParameterSync syncMovementGUI;
  ofxOscParameterSync syncShaderGUI;
  ofParameterGroup*   windParams;
  ofParameter<ofVec2f> wind;
  ofParameter<bool>   windXNoise;
  ofParameter<bool>   windYNoise;
  ofParameter<bool>   toggleWind;
  void                toggleWindChanged(bool & value);
  void                windChanged(ofVec2f & value);
  
  ofParameterGroup*   getWindGroup();

  ofxOscSender        sender;
  ofxOscReceiver      receiver;

  // generalGUI
  ofxLabel            visualIP;
  ofParameter<int>    preset;
  ofxToggle           secureLimit;
  ofxLabel            visualFrameRate;
#ifdef USE_TIMELINE
  ofParameter<bool>   togglePlayPauseTimeline;
#endif
  ofParameter<float>  audioInvertCoefficent;
  
  ofParameterGroup    volumeGroup;
  ofParameter<bool>   manualInvert;
  ofParameter<bool>   forceInvert;
  ofParameter<float>  volumeLevel;
  // graphicGUI
  ofxButton           bClear;
  ofParameter<bool>   bClearAfdterPreset;
  ofxButton           clearAll;
  ofParameter<bool>   clearAllAfterPreset;
  ofxToggle           directDraw;
  ofxToggle           addToDraw;

  ofParameterGroup    vectorGroup;
  ofParameter<bool>   loadVector;
  ofParameter<bool>   loadVectorAfterPreset;
  ofParameter<int>    vectorId;

  ofParameterGroup    geomGroup;
  ofParameter<bool>   loadGeom;
  ofParameter<bool>   loadGeomAfterPreset;
  ofParameter<int>    geomId;
  ofParameter<float>  geomParam1;
  ofParameter<float>  geomParam2;
  
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
  ofParameter<float>  scaleFactor;
  ofParameter<ofFloatColor>  color;
  
  ofParameterGroup    connectLinesGroup;
  ofParameter<float>  minLineDistance;
  ofParameter<float>  maxLineDistance;
  ofParameter<float>  lineAlpha;
  
  // movementGUI
  ofParameter<float>  particleSpeed;
  ofParameter<float>  sameSpring;
  ofParameter<float>  sameFriction;
  ofParameter<float>  repulsionForce;
  ofParameter<bool>   repulsionFromTarget;
  ofParameter<bool>   followFlow;
  
  GoofyFlowField      fakeFlowField;
  void                resetFlowChanged(bool & value);
  void                flowForceChanged(float & value);
  void                followFlowChanged(bool & value);
  
  GoofyPerlinNoise    fakePerlin;
  ofParameter<bool>   enablePerlin;
  void                enablePerlinChanged(bool & value);
  void                resXPerlinChanged(float & value);
  void                resYPerlinChanged(float & value);
  void                speedPerlinChanged(float & value);
  void                forcePerlinChanged(float & value);
  
  void                cleanPointers();
  void                clearMidiPointers();
  void                setupOSCPointers();
  void                setupMidiOSCPointers();
  
  virtual void        processOSCMessage(ofxOscMessage& message);
  
  // shaderGUI
  ofParameterGroup    pixelShader;
  ofParameter<bool>   pixelEffect;
  ofParameter<bool>   allLine;
  ofParameter<bool>   halfPixelEffect;
  ofParameter<bool>   samePixelSize;
  ofParameter<float>  pixelSizeLimit;
  ofParameter<float>  pixelWidth;
  ofParameter<float>  pixelHeight;
  
  ofParameter<float>               oscRed;
  ofParameter<float>               oscGreen;
  ofParameter<float>               oscBlue;
  
  tr1::unordered_map<string, ofParameter<float>* > mapToFloatValue;
  
  bool                freezeDrawAndClean;
};

#endif /* defined(__APV_Controller__MainGUI__) */
