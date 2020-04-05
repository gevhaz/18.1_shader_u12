#pragma once

#include "ofMain.h"

class MusicPlayer {
	public:
		void load(string songName, float audioVolume);
		void keyPressed(int key);
		float getTime();

	private:
		ofSoundPlayer player;
};
