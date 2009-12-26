/*
 * Copyright (c) 2009 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <Jet/Root.hpp>

using namespace Jet;

//------------------------------------------------------------------------------
template <typename T>
typename T::Ptr Root::objectNew(
    const string& name, 
    map<string, typename T::Ptr>& m,
    void (Root::Observer::*fn)(typename T::Ptr)) {

    typename map<string, typename T::Ptr>::iterator i = m.find(name);
    if (i == m.end()) {
        typename T::Ptr t = new T;
        m[name] = t;
        publisher_.notify(fn, t);       
        return t;    
    } else {
        return i->second;
    }
}

//------------------------------------------------------------------------------
Model::Ptr              
Root::modelNew(const string& name) {
    return objectNew<Model>(name, model_, &Observer::onModelNew);
}

//------------------------------------------------------------------------------
Entity::Ptr             
Root::entityNew(const string& name) {
    return objectNew<Entity>(name, entity_, &Observer::onEntityNew);
}

//------------------------------------------------------------------------------
Camera::Ptr             
Root::cameraNew(const string& name) {
    return objectNew<Camera>(name, camera_, &Observer::onCameraNew);   
}

//------------------------------------------------------------------------------
Quad::Ptr               
Root::quadNew(const string& name) {
    return objectNew<Quad>(name, quad_, &Observer::onQuadNew);
}

//------------------------------------------------------------------------------
Speaker::Ptr            
Root::speakerNew(const string& name) {
    return objectNew<Speaker>(name, speaker_, &Observer::onSpeakerNew);
}
    
//------------------------------------------------------------------------------
Listener::Ptr           
Root::listenerNew(const string& name) {
    return objectNew<Listener>(name, listener_, &Observer::onListenerNew);
}
    
//------------------------------------------------------------------------------
Cloud::Ptr              
Root::cloudNew(const string& name) {
    return objectNew<Cloud>(name, cloud_, &Observer::onCloudNew);
}
    
//------------------------------------------------------------------------------
void                    
Root::activeCamera(Camera::Ptr c) {
    if (c != activeCamera_) {
        activeCamera_ = c;
    }
}
    
//------------------------------------------------------------------------------
void                    
Root::activeListener(Listener::Ptr l) {
    if (l != activeListener_) {
        activeListener_ = l;
    }
}
    
//------------------------------------------------------------------------------
void                    
Root::timeInc(float t) {
    time_ += t;    
}
