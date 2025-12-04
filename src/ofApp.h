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
		ofMesh m_mansion, m_battery, * m_hearts, m_gun, m_itemSlot, m_item;
		float m_attenuationFactor;

		// Player / camera movement
		float m_camRotFactor;
		bool m_camIsMoving;

		// Resources
		ofTexture m_batterySprites[7], m_heartSmallSprites, m_heartLargeSprites;
		float m_batteryPercentage;
		int m_batteryState;
		float m_healthPercentage;
		float m_uiRot;

		bool m_isSucking = false;

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
