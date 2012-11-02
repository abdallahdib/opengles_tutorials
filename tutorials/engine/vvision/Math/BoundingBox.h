/* BoundingBox
 *
 * Virtual Vision Engine . Copyright (C) 2012 Abdallah DIB.
 * All rights reserved. Email: Abdallah.dib@virtual-vison.net
 * Web: <http://www.virutal-vision.net/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H


#include "vmath.h"
#include <float.h>

namespace vvision
{
    class CBoundingBox
    {
        public :
        
        /** default constructor*/
        CBoundingBox()
        {
            Reset();
        }
        
        /** construct with min and max*/
        CBoundingBox(vec3f _min,vec3f _max)
        : m_vMin(_min),m_vMax(_max)
        {
        }
        
        /** copy reconstrutor*/
        CBoundingBox(const CBoundingBox& box)
        {
            m_vMax = box.m_vMax;
            m_vMin = box.m_vMin;
        }
        
        /***operator =*/
        CBoundingBox& operator=(const CBoundingBox& box)
        {
            m_vMax = box.m_vMax;
            m_vMin = box.m_vMin;
            return *this;
        }
        
        /** add vertex to the bounding box*/
        inline void Add(const vec3f& _NewVertexToTest)
        {
            
            if(_NewVertexToTest.x>m_vMax.x) m_vMax.x=_NewVertexToTest.x;
            if(_NewVertexToTest.y>m_vMax.y) m_vMax.y=_NewVertexToTest.y;
            if(_NewVertexToTest.z>m_vMax.z) m_vMax.z=_NewVertexToTest.z;
            
            if(_NewVertexToTest.x<m_vMin.x) m_vMin.x=_NewVertexToTest.x;
            if(_NewVertexToTest.y<m_vMin.y) m_vMin.y=_NewVertexToTest.y;
            if(_NewVertexToTest.z<m_vMin.z) m_vMin.z=_NewVertexToTest.z;
        }
        
        /** get the center of the bounding box*/
        inline vec3f GetCenter() const { return (m_vMin+m_vMax)/2.0f;}
        
        /** get the length of the bounding box*/
        inline vec3f GetLengthFromOrigin() const
        {
            vec3f l;
            l.x=(m_vMax.x-m_vMin.x)/1.0f;
            l.y=(m_vMax.y-m_vMin.y)/1.0f;
            l.z=(m_vMax.z-m_vMin.z)/1.0f;
            return l;
        }
        
        /**reset*/
        inline void Reset()
        {
            m_vMin.x = FLT_MAX;
            m_vMin.y = FLT_MAX;
            m_vMin.z = FLT_MAX;
            
            m_vMax.x = -FLT_MAX;
            m_vMax.y = -FLT_MAX;
            m_vMax.z = -FLT_MAX;
        }
        
        /** get m_vMax*/
        inline vec3f GetMax() const {return m_vMax;}
        
        /** get m_vMin*/
        inline vec3f GetMin() const {return m_vMin;}
        
        /** manually set max*/
        void SetMax(vec3f _max) {m_vMax=_max;}
        
        /** and min*/
        void SetMin(vec3f _min) {m_vMin=_min;}
        
        /** test if a point is inside bounding box,
         * @return true if inside
         */
        bool PointIsInside(vec3f &point)
        {
            return (
                    ( point.x >= m_vMin.x && point.x<=m_vMax.x ) &&
                    ( point.y >= m_vMin.y && point.y<=m_vMax.y ) &&
                    ( point.z >= m_vMin.z && point.z<=m_vMax.z )
                    );
        }
    private:
        /**
         * the minimum
         */
        vec3f m_vMin;
        /**
         * the maximum
         */
        vec3f m_vMax;
        
    };
}

#endif
