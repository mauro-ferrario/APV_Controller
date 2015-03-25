//
//  VectorDraw.cpp
//  APV_Controller
//
//  Created by Mauro Ferrario on 08/02/15.
//
//

#include "VectorDraw.h"
#include "MainGUI.h"

void VectorDraw::loadFromSVG(MainGUI* mainGUI, int id, string path, ofVec2f outputSize, PolyLineException* exception, bool addDirtyPoints)
{
  string url = loadSVGAddress(path)[id];
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
      
      if(exception)
      {
        exception->applyException(tempPolyline, url);
      }
      
      int totVertex = tempPolyline.getVertices().size();
      for(int z = 0; z < totVertex;z++)
      {
        if(tempPolyline.getVertices()[z].x > max.x)
          max.x = tempPolyline.getVertices()[z].x;
        else if(tempPolyline.getVertices()[z].y > max.y)
          max.y = tempPolyline.getVertices()[z].y;
        
        if(tempPolyline.getVertices()[z].x < min.x)
          min.x = tempPolyline.getVertices()[z].x;
        else if(tempPolyline.getVertices()[z].y < min.y)
          min.y = tempPolyline.getVertices()[z].y;
        newPoly.addVertex(tempPolyline.getVertices()[z]);
      }
    }
  }
  exception = NULL;
  ofPoint newPosition;
  newPosition.x = (outputSize.x - (max.x - min.x)) * .5;
  newPosition.y = (outputSize.y - (max.y - min.y)) * .5;
  for(int a = 0; a <  newPoly.getVertices().size(); a++)
  {
    tempPoint.x = newPoly[a].x + newPosition.x;
    tempPoint.y = newPoly[a].y + newPosition.y;
    if(addDirtyPoints)
    {
      if(ofRandom(0,1) > .7)
      {
        if(ofRandom(0,1) > .7)
        {
          float newRandom = ofRandom(0,1);
          if(newRandom < .25)
          {
            mainGUI->addSinglePoint(ofRandom(-200,-100), ofRandom(1080));
          }
          else if(newRandom < .5)
          {
            mainGUI->addSinglePoint(1920 + ofRandom(100,200), ofRandom(1080));
          }
          else if(newRandom < .75)
          {
            mainGUI->addSinglePoint(ofRandom(1920), ofRandom(-200,100));
          }
          else
          {
            mainGUI->addSinglePoint(ofRandom(1920), 1080 + ofRandom(100,200));
          }
        }
      }
    }
    mainGUI->addSinglePoint(tempPoint.x, tempPoint.y);
  }
  mainGUI = NULL;
  delete mySVG;
}

vector<string> VectorDraw::loadSVGAddress(string path)
{
  vector<string> svgUrl;
  ofDirectory dir;
  dir.listDir(path);
  dir.sort();
  vector<ofFile> files = dir.getFiles();
  int totFiles = files.size();
  for(int a = 0; a < totFiles; a++)
  {
    if(files[a].isFile())
    {
      svgUrl.push_back(path+files[a].getFileName());
    }
  }
  return svgUrl;
}

int VectorDraw::getTotSvgs(string path)
{
  return loadSVGAddress(path).size();
}