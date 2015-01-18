/*
                
        Reymenta Spout sender for Cinder

        Search for "SPOUT" to see what is required
        Uses the Spout dll

        Based on the RotatingBox CINDER example without much modification
        Nothing fancy about this, just the basics.

        Search for "SPOUT" to see what is required

    ==========================================================================
    Copyright (C) 2014 Lynn Jarvis.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    ==========================================================================

*/
#include "ReymentaImageSequenceApp.h"
// -------- SPOUT -------------
void ReymentaImageSequenceApp::prepareSettings(Settings *settings)
{
        g_Width  = 640;
        g_Height = 512;
        settings->setWindowSize( g_Width, g_Height );
        settings->setFullScreen( false );
        settings->setResizable( false ); // keep the screen size constant for a sender
        settings->setFrameRate( 60.0f );
}
// ----------------------------

void ReymentaImageSequenceApp::setup()
{
    glEnable( GL_TEXTURE_2D );
    gl::enableDepthRead();
    gl::enableDepthWrite(); 

	playing = false;
	paused = false;

    // -------- SPOUT -------------
    // Set up the texture we will use to send out
    // We grab the screen so it has to be the same size
    spoutTexture =  gl::Texture(g_Width, g_Height);
    strcpy_s(SenderName, "Reymenta Spout Sender"); // we have to set a sender name first
    // Optionally test for texture share compatibility
    // bMemoryMode informs us whether Spout initialized for texture share or memory share
    bMemoryMode = spoutsender.GetMemoryShareMode();
    // Initialize a sender
    bInitialized = spoutsender.CreateSender(SenderName, g_Width, g_Height);
    // ----------------------------
}

void ReymentaImageSequenceApp::update()
{

}

void ReymentaImageSequenceApp::draw()
{
    gl::clear( Color( 0.39f, 0.025f, 0.0f ) ); // red/brown to be different
    

    // -------- SPOUT -------------
    if(bInitialized) {

        // Grab the screen (current read buffer) into the local spout texture
        spoutTexture.bind();
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, g_Width, g_Height);
        spoutTexture.unbind();

        // Send the texture for all receivers to use
        // NOTE : if SendTexture is called with a framebuffer object bound, that binding will be lost
        // and has to be restored afterwards because Spout uses an fbo for intermediate rendering
        spoutsender.SendTexture(spoutTexture.getId(), spoutTexture.getTarget(), g_Width, g_Height);

    }

}
// -------- SPOUT -------------
void ReymentaImageSequenceApp::shutdown()
{
    spoutsender.ReleaseSender();
}

// sequence
//Begins playback of sequence
void ReymentaImageSequenceApp::play()
{
paused = false;
playing = true;
}

// Pauses playback
void ReymentaImageSequenceApp::pause()
{
paused = true;
playing = false;
}

// Stops playback and resets the playhead to zero
void ReymentaImageSequenceApp::stop()
{
playheadPosition = 0;
playing = false;
paused = false;
}
// Seek to a new position in the sequence
void ReymentaImageSequenceApp::setPlayheadPosition(int newPosition)
{
playheadPosition = max(0, min(newPosition, totalFrames));
}

/**
*  -- Loads all files contained in the supplied folder and creates Textures from them
*/
void ReymentaImageSequenceApp::createFromDir(string filePath)
{
bool noValidFile = true; // if no valid files in the folder, we keep existing vector
int i = 0;
string ext = "";
fs::path p(filePath);
for (fs::directory_iterator it(p); it != fs::directory_iterator(); ++it)
{
if (fs::is_regular_file(*it))
{
string fileName = it->path().filename().string();
if (fileName.find_last_of(".") != std::string::npos) ext = fileName.substr(fileName.find_last_of(".") + 1);
if (ext == "png" || ext == "jpg")
{
if (noValidFile)
{
// we found a valid file
noValidFile = false;
sequenceTextures.clear();
// reset playhead to the start
playheadPosition = 0;
}
sequenceTextures.push_back(ci::gl::Texture(loadImage(filePath + fileName)));
}
}
}

totalFrames = sequenceTextures.size();
}

// Loads all of the images in the supplied list of file paths

void ReymentaImageSequenceApp::createFromPathList(vector<string> paths)
{
sequenceTextures.clear();
for (int i = 0; i < paths.size(); ++i)
{
sequenceTextures.push_back(ci::gl::Texture(loadImage(paths[i])));
}
totalFrames = sequenceTextures.size();
}

void ReymentaImageSequenceApp::createFromTextureList(vector<ci::gl::Texture> textureList)
{
sequenceTextures.clear();
sequenceTextures = textureList;
totalFrames = sequenceTextures.size();
}
// This line tells Cinder to actually create the application
CINDER_APP_NATIVE( ReymentaImageSequenceApp, RendererGl )
