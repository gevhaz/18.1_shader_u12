#pragma once

#include "ofMain.h"
#include "MusicPlayer.h"

#define FFTBANDS 512

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);

    private:
		float getMeanValue(float * vals, int startIndex, int endIndex);
		int getCentroid(float* vals);

		MusicPlayer player;
		float audioVolume;
		float * spectrum;
		std::array<float, FFTBANDS> previousSpectrum;
		float smoothing;

		float bass;
		float mid;
		float high;
		float centroid;

		// onset detection
		float threshold;
		float minimumThreshold;
		float decayRate;

		// BACKGROUND
		ofMesh getBackgroundMesh();

		ofShader backgroundShader;
		ofMesh backgroundMesh;
		float backgroundAlpha;

		// CENTER CIRCLE
		ofMesh getCenterCircleMesh(int n, float lineLength, float centerCircleLineThickness);

		ofShader centerCircleShader;
		ofMesh centerCircleMesh;
		float centerCircleAngleDiff; //difference in angle between lines to be sent as uniform
		int centerCircleRadius;
		float centerCircleIntensity;
		float centerCircleLineLength;
		float centerCircleLineThickness;

		//SATELLITES
		ofMesh getSattelliteMesh(float glowWidth, float glowHeight, float satelliteInnerRadius);

		ofMesh satellitesMesh;
		ofShader satelliteShader;
		float sidesAngle;
		int satelliteGlowWidth;
		int satelliteGlowHeight;
		int satelliteInnerRadius;
		float satelliteDampening;
		float satelliteAngle;

};
