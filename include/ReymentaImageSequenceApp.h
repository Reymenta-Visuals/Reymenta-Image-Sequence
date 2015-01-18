
#include "cinder/app/AppNative.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/Camera.h"
#include <vector>

// spout
#include "spout.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ReymentaImageSequenceApp : public AppNative {
public:
	void prepareSettings(Settings *settings);
	void setup();
	void update();
	void draw();
	void shutdown();
	void play();
	void pause();
	void stop();
	void setPlayheadPosition(int newPosition);
	void createFromDir(string filePath);
	void createFromPathList(vector<string> paths);
	void createFromTextureList(vector<ci::gl::Texture> textureList);

private:
	// spout

	// -------- SPOUT -------------
	SpoutSender spoutsender;                    // Create a Spout sender object
	bool bInitialized;                          // true if a sender initializes OK
	bool bMemoryMode;                           // tells us if texture share compatible
	unsigned int g_Width, g_Height;             // size of the texture being sent out
	char SenderName[256];                       // sender name 
	gl::Texture spoutTexture;                   // Local Cinder texture used for sharing
	// ----------------------------
	bool playing;
	bool paused;

};

