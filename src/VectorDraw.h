//
//  VectorDraw.h
//  APV_Controller
//
//  Created by Mauro Ferrario on 08/02/15.
//
//

#ifndef __APV_Controller__VectorDraw__
#define __APV_Controller__VectorDraw__

#include "ofMain.h"
#include "ofxSvg.h"
#include "PolyLineException.h"

class MainGUI;

class VectorDraw
{
public:
  static void                 loadFromSVG(MainGUI* mainGUI, int id, string path, ofVec2f outputSize, PolyLineException* exception, bool addDirtyPoints = true);
  static vector<string>       loadSVGAddress(string path);
  static int                  getTotSvgs(string path);
};

#endif /* defined(__APV_Controller__VectorDraw__) */
