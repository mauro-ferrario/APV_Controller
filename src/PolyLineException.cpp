//
//  PolyLineException.cpp
//  APV_Controller
//
//  Created by XXX on 14/02/15.
//
//

#include "PolyLineException.h"

void PolyLineException::applyException(ofPolyline& poly, string url)
{
  if(url == "svg/0.svg" || url != "svg/1.svg")
  {
    
  }
  else
  {
//    newPoly.simplify();
  }
  if(url == "svg/1.svg") // Logo LPM 1
  {
    
  }
  else if(url == "svg/2.svg") // Logo LPM 2
  {
    
  }
  else if(url == "svg/3.svg") // Numero 7
  {
    poly = poly.getResampledBySpacing(25);
  }
  else
  {
//    newPoly = newPoly.getResampledByCount(150);
    poly = poly.getResampledBySpacing(13);
  }
}