#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxImGui.h"

class ofApp : public ofBaseApp{

	public:
		// 3D rendering
		ofEasyCam m_cam;
		ofTexture m_tilesheet;
		ofLight m_camLight;
		ofMesh m_mansion;

		// Player / camera movement
		float m_camRotFactor;
		bool m_camIsMoving;

		// Resources
		float m_batteryPercentage, m_healthPercentage;

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
};
