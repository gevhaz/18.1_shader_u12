#include "MusicPlayer.h"

void MusicPlayer::load(string songPath, float audioVolume) {
	player.load(songPath);
	player.setVolume(audioVolume);
	player.play();
	player.setPaused(true);
}

void MusicPlayer::keyPressed(int key) {
	switch (key) {
		case ' ':
			player.setPaused(player.isPlaying());
			break;
		case OF_KEY_RIGHT: {
			int playhead = player.getPositionMS();
			player.setPositionMS(playhead + 15000);
			break; 
		}
		case OF_KEY_LEFT: {
			int playhead = player.getPositionMS();
			player.setPositionMS(playhead - 15000);
			break; 
		}
		case 's':
			player.stop();
			break;
		case 'f':
			ofToggleFullscreen();
			break;
	}
}

float MusicPlayer::getTime() {
	return player.getPositionMS();
}
