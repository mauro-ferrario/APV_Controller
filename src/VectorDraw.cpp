//
//  VectorDraw.cpp
//  APV_Controller
//
//  Created by Mauro Ferrario on 08/02/15.
//
//

#include "VectorDraw.h"
#include "MainGUI.h"

void VectorDraw::loadFromSVG(MainGUI* mainGUI, int id, string path)
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