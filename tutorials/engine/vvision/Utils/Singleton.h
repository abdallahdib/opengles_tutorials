/* Singleton.h
 *
 * Virtual Vision Engine . Copyright (C) 2012 Abdallah DIB.
 * All rights reserved. Email: Abdallah.dib@virtual-vison.net
 * Web: <http://www.virutal-vision.net/>
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#ifndef SINGLETON_H
#define SINGLETON_H

#include <cstdlib>

namespace vvision
{
    
    /** \class Singleton
     * Template singleton class
     *
     * \author Abdallah DIB
     */
    template<class T>
    class  Singleton
    {
        public :
        
        /** return a unique instance
         */
        static T& Instance()
        {
            if(!m_instance)
                m_instance=new T;
            return *m_instance;
        }
        
        /** destory the unique instance
         */
        static void Destroy()
        {
            if(m_instance)
            {
                delete m_instance;
                m_instance=NULL;
            }
        }
    protected:
        
        /** the private constructor
         */
        Singleton() {}
        
        /** the private destructor
         */
        ~Singleton() {}
        
    private:
        
        /** the instance
         */
        static T* m_instance;
        
        /** no copy allowed
         */
        Singleton(const Singleton&);
        /** no copy allowed
         */
        void operator=(Singleton&);
        
    };
    
    /** the only instance
     */
    template<class T>T* Singleton<T>::m_instance=NULL;
#define CREATE_SINGLETON(class_name) \
    class class_name: public Singleton<class_name> { \
        friend class Singleton<class_name>;
}

#endif
