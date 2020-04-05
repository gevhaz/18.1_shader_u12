#include "ofApp.h"
using namespace glm;

void ofApp::setup() {
	//MUSIC
	string songpath = "music/U12.mp3";
	audioVolume = 0.5;
    player.load(songpath, audioVolume);

	previousSpectrum.fill(0.0);

    // temporal smoothing of the FFT
	smoothing = 0.8;

	// onset detection
	decayRate = 0.05;
	minimumThreshold = 0.1;
	threshold = minimumThreshold;

	//BACKGROUND
	backgroundShader.load("shaders/background");
	backgroundAlpha = 0.1;
	backgroundMesh = getBackgroundMesh();
	ofEnableAlphaBlending();
	ofSetBackgroundAuto(false);
	ofBackground(0);

	//CENTRAL CIRCLE
	centerCircleShader.load("shaders/shader");
	centerCircleLineLength = 1000.0;
	centerCircleLineThickness = 1.0;
	centerCircleRadius = 100; // arbitrary
	int nCentralCircleLines = 200;
	centerCircleMesh = getCenterCircleMesh(nCentralCircleLines,
			centerCircleLineLength, centerCircleLineThickness);
	centerCircleAngleDiff = 0.0;
	centerCircleIntensity = 0;

	//SATELLITES
	satelliteShader.load("shaders/sides");
	satelliteGlowWidth = 500;
	satelliteGlowHeight = 1000;
	satelliteInnerRadius = 650;
	satellitesMesh = getSattelliteMesh((float)satelliteGlowWidth,
			(float)satelliteGlowHeight, (float)satelliteInnerRadius);
	satelliteAngle = 0;
}

void ofApp::update() {
	//FFT
	ofSoundUpdate();
	spectrum = ofSoundGetSpectrum(FFTBANDS);
	for (int i = 0; i < FFTBANDS; ++i) {
		spectrum[i] = ofLerp(spectrum[i], previousSpectrum[i], smoothing);
		previousSpectrum[i] = spectrum[i];
	}

    // The bin ranges to use for each frequency group
    float lims [3] = { 2, 10, 120 };
	bass = getMeanValue(spectrum, 0, lims[0]);
	mid = getMeanValue(spectrum, lims[0], lims[1]);
	high = getMeanValue(spectrum, lims[1], lims[2]);
	bass = ofMap(bass, 0.0, 0.9 * audioVolume, 0.0, 1.0);
	mid = ofMap(mid, 0.0, 0.24 * audioVolume, 0.0, 1.0);
	high = ofMap(high, 0.0, 0.06 * audioVolume, 0.0, 1.0);
	centroid = getCentroid(spectrum);

	// onset detection
	threshold = ofLerp(threshold, minimumThreshold, decayRate);
	if (bass > threshold) {
		threshold = bass;
		ofBackground(0);
	}

	//CENTER CIRCLE
	centerCircleAngleDiff = ofMap(high, 0.0, 1.0, 1.0, 0.05, true);
	centerCircleIntensity = ofClamp(mid, 0.0, 1.0); // should be between 0 and 1
	centerCircleRadius = ofMap(centroid, 0.0, 10000, 40, 450);

	//SATELLITES
	satelliteDampening = ofClamp(bass, 0.01, 1.0); //shouldn't be 0
	satelliteAngle = ofMap(mid, 0.0, 1.0, 0.0, PI, true); //shouldn't be 0
}

void ofApp::draw() {
	//BACKGROUND
	backgroundShader.begin();
		backgroundShader.setUniform1f("alpha", backgroundAlpha);
		backgroundMesh.draw();
	backgroundShader.end();

	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

	//CENTER CIRCLE
	centerCircleShader.begin();
		centerCircleShader.setUniform1f("angleDiff", centerCircleAngleDiff);
		centerCircleShader.setUniform1f("intensity", centerCircleIntensity);
		centerCircleShader.setUniform1i("radius", centerCircleRadius);
		centerCircleShader.setUniform1i("lineLength", centerCircleLineLength);
		centerCircleMesh.draw();
	centerCircleShader.end();

	//SATELLITES
	satelliteShader.begin();
		satelliteShader.setUniform1i("width", satelliteGlowWidth);
		satelliteShader.setUniform1i("height", satelliteGlowHeight);
		satelliteShader.setUniform1f("dampening", satelliteDampening);
		satelliteShader.setUniform1f("angle", satelliteAngle);
		satellitesMesh.draw();
	satelliteShader.end();
}

ofMesh ofApp::getCenterCircleMesh(int n, float lineLength, float lineThickness) {
	ofMesh m;
	vec3 topleft(0.0, 0.0, 0.0);
	vec3 topright(0.0 + lineThickness, 0.0, 0.0);
	vec3 bottomleft(0.0, lineLength, 0.0);
	vec3 bottomright(0.0 + lineThickness, lineLength, 0.0);

	for (int i = 0; i < n; ++i) {
		m.addVertex(topleft);
		m.addVertex(topright);
		m.addVertex(bottomleft);
		m.addVertex(bottomright);

		m.addTexCoord(vec2(topleft.x, topleft.y));
		m.addTexCoord(vec2(topright.x, topright.y));
		m.addTexCoord(vec2(bottomleft.x, bottomleft.y));
		m.addTexCoord(vec2(bottomright.x, bottomright.y));

		m.addIndex(4 * i);
		m.addIndex(4 * i + 1);
		m.addIndex(4 * i + 2);
		m.addIndex(4 * i + 2);
		m.addIndex(4 * i + 3);
		m.addIndex(4 * i + 1);
	}
	return m;
}

ofMesh ofApp::getSattelliteMesh(float satW, float satH, float innerRadius) {
	// Pointy glow on the sides shader. It needs two squares.
	ofMesh m;
	vec3 topleft(-satW / 2, innerRadius, 0.0);
	vec3 topright(satW / 2, innerRadius, 0.0);
	vec3 bottomleft(-satW / 2, innerRadius + satH, 0.0);
	vec3 bottomright(satW / 2, innerRadius + satH, 0.0);
	for (int i = 0; i < 2; ++i) {
		m.addVertex(topleft);
		m.addVertex(topright);
		m.addVertex(bottomleft);
		m.addVertex(bottomright);

		m.addTexCoord(vec2(0.0, 0.0));
		m.addTexCoord(vec2(satW, 0.0));
		m.addTexCoord(vec2(0.0, satH));
		m.addTexCoord(vec2(satW, satH));

		m.addIndex(4 * i);
		m.addIndex(4 * i + 1);
		m.addIndex(4 * i + 2);
		m.addIndex(4 * i + 2);
		m.addIndex(4 * i + 3);
		m.addIndex(4 * i + 1);
	}
	return m;
}

ofMesh ofApp::getBackgroundMesh() {
	// Give backroundMesh 6 vertices to make two triangles to fill the background
	ofMesh m;
	for (int i = 0; i < 6; ++i) {
		m.addVertex(vec3(0.0, 0.0, 0.0));
	}
	return m;
}

float ofApp::getMeanValue(float * vals, int startIndex, int endIndex) {
	float meanValue = 0;
	for (int i = startIndex; i < endIndex; ++i) {
		meanValue += vals[i];
	}
	return meanValue /= (endIndex - startIndex);
}

void ofApp::keyPressed(int key) {
    player.keyPressed(key);
}

int ofApp::getCentroid(float* vals) {
	float cumulativeSum = 0;
	float centroidNormalization = 0;
	float nyquistFrequency = 22050; //assuming the sample rate is 44100
	for (int i = 0; i < FFTBANDS; ++i) {
		cumulativeSum += i * vals[i];
		centroidNormalization += vals[i];
	}
	int indexOfMeanFrequency = 0;
	if (centroidNormalization) {
		indexOfMeanFrequency = (int)(cumulativeSum / centroidNormalization);
	}
	float centroidFrequency = indexOfMeanFrequency * (nyquistFrequency / FFTBANDS);
	return centroidFrequency > 10000 ? 1 : centroidFrequency; //otherwise gigantic when volume is low 
}
