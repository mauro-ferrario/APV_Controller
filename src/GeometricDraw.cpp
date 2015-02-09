//
//  GeometricDraw.cpp
//  APV_Controller
//
//  Created by Mauro Ferrario on 08/02/15.
//
//

#include "GeometricDraw.h"
#include "MainGUI.h"

void GeometricDraw::loadGeometric(MainGUI* mainGUI, int geomId, float geomParam1, float geomParam2, ofVec2f outputSize)
{
  int width = 0;
  int height = 0;
  ofPoint tempPoint;
  if(outputSize.x == 0 && outputSize.y == 0)
  {
    width = ofGetWindowWidth();
    height = ofGetWindowHeight();
  }
  else
  {
    width = outputSize.x;
    height = outputSize.y;
  }
  float windowWidth = width;
  float windowHeight = height;
  float halfWindowWidth = windowWidth*.5;
  float halfWindowHeight = windowHeight*.5;
  switch(geomId)
  {
    case 0:
      for(int a = 0; a < 360; a += 10)
      {
        tempPoint.x = halfWindowWidth + sin(ofDegToRad(a)) *  geomParam1 * 10;
        tempPoint.y = halfWindowHeight + cos(ofDegToRad(a)) *  geomParam1 * 10;
        mainGUI->addSinglePoint(tempPoint.x, tempPoint.y);
      }
      break;
    case 1:     // Spirale
      for(int a = 0; a < 360 * 10; a += 10)
      {
        tempPoint.x = halfWindowWidth + sin(ofDegToRad(a)) * geomParam2 * (a / (geomParam1 * PI));
        tempPoint.y = halfWindowHeight + cos(ofDegToRad(a)) * geomParam2 * (a / (geomParam1 * PI));
        mainGUI->addSinglePoint(tempPoint.x, tempPoint.y);
      }
      break;
    case 2:     // Curva a farfalla
      for(int a = 0; a < 360 * 5; a += 10)
      {
        tempPoint.x = halfWindowWidth + (sin(ofDegToRad(a)) * ( pow(2, cos(ofDegToRad(a))) - 2 * cos(4*ofDegToRad(a)) - pow(sin(ofDegToRad(a)/12), 5) )) * 100;
        tempPoint.y = halfWindowHeight + (cos(ofDegToRad(a)) * ( pow(2, cos(ofDegToRad(a))) - 2 * cos(4*ofDegToRad(a)) - pow(sin(ofDegToRad(a)/12), 5) )) * 100;
        mainGUI->addSinglePoint(tempPoint.x, tempPoint.y);
      }
      break;
    case 3:     // Curva a farfalla
      for(int a = 0; a < 360 * 5; a += 10)
      {
        tempPoint.x = halfWindowWidth + geomParam1 * 10 * sin(ofDegToRad(a)) / ( 1 + (sin(ofDegToRad(a)) * sin(ofDegToRad(a))));
        tempPoint.y = halfWindowHeight + geomParam1 * 10 * sin(ofDegToRad(a)) * cos(ofDegToRad(a)) / ( 1 + (sin(ofDegToRad(a)) * sin(ofDegToRad(a))));
        mainGUI->addSinglePoint(tempPoint.x, tempPoint.y);
      }
      break;
    default:
      for(int a = 0; a < 400; a += 1)
      {
        tempPoint.x = windowWidth * ofRandom(0,1);
        tempPoint.y = windowHeight * ofRandom(0,1);
        mainGUI->addSinglePoint(tempPoint.x, tempPoint.y);
      }
      break;
  }
  mainGUI = NULL;
}