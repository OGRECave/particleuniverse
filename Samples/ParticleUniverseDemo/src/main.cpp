#include "ParticleUniverseDemo.h"

int main(int argc, char* argv[])
{	
	Sample_ParticleUniverseDemo app;
	app.initApp();
	app.createScene();
	app.getRoot()->startRendering();
	app.destroyScene();
}
