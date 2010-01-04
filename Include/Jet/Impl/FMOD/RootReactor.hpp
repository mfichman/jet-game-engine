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
#pragma once

#include <Jet/Root.hpp>
#include <Jet/Impl/FMOD/Types.hpp>
#include <string>
#include <map>

namespace Jet { namespace Impl { namespace FMOD {
using namespace std;
using namespace std::tr1;
using namespace boost;

class RootReactor : public Root::Observer, public Options::Observer, public Loader::Observer {
public:
    typedef intrusive_ptr<RootReactor> Ptr; 
    RootReactor(Root::Ptr e);
    ~RootReactor();
    
    void onSpeakerNew(Speaker::Ptr);
    void onMasterVolume();
    void onSoundNew(Sound::Ptr);

    inline FMOD_SYSTEM* system() const { return system_; }

private:
    Root::Ptr root_;
    map<string, SoundPtr> sound_;
    vector<Interface::Ptr> reactors_;
    FMOD_SYSTEM* system_;
};

}}}
