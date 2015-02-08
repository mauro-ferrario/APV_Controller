//
//  GeometricDraw.h
//  APV_Controller
//
//  Created by Mauro Ferrario on 08/02/15.
//
//

#ifndef __APV_Controller__GeometricDraw__
#define __APV_Controller__GeometricDraw__

#include "ofMain.h"

class MainGUI;

class GeometricDraw
{
public:
  static void loadGeometric(MainGUI* mainGUI, int geomId, float geomParam1, float geomParam2);
};

#endif /* defined(__APV_Controller__GeometricDraw__) */
