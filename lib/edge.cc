/*
 * hyp2mat - convert hyperlynx files to matlab scripts
 * Copyright 2012 Koen De Vleeschauwer.
 *
 * This file is part of hyp2mat.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <algorithm>
#include "hyperlynx.h"

using namespace Hyp2Mat;

Edge::Edge()
{
  is_hole = false;
  width = 0;
  nesting_level = 0;
}

/*
 * Remove repeating vertices from a polygon
 */

Edge unique(Edge p) {
  Edge q = p;
  q.vertex.clear();
 
  if (p.vertex.empty()) return q; 

  /* create list of non-repeating vertices */
  q.vertex.push_back(p.vertex.front());

  for (PointList::iterator i = p.vertex.begin(); i != p.vertex.end(); ++i)
    if (*i != q.vertex.back()) q.vertex.push_back(*i);
  
  /* Remove last vertex if first and last vertex same */ 
  while ((q.vertex.size() > 1) && (q.vertex.front() == q.vertex.back()))
    q.vertex.pop_back();
    
  return q; 
}


/*
 * Calculate if a polygon is clockwise or not.
 */

bool Edge::is_clockwise() {
  Edge q = unique(*this);
    
  /* A polygon has three or more vertices. Arbitrarily choose clockwise if not a polygon. */
  if (q.vertex.size() <= 2) 
    return true;

  /* calculate area of polygon */
  double area = 0;
  for (PointList::iterator i = q.vertex.begin(); i != q.vertex.end(); ++i) {
    /* i points to current element; j points to next element */
    PointList::iterator j = i;
    ++j;
    /* polygon is closed */
    if (j == q.vertex.end()) j = q.vertex.begin();
    area += i->x * j->y - j->x * i->y;
    }

  /* positive area is clockwise. Arbitrarily choose clockwise if area 0 */
  return area >= 0; 
  
}

/*
 *  Reverse order of vertices.
 */

void Edge::reverse()
{
  std::reverse(this->vertex.begin(), this->vertex.end());
}

/* not truncated */
