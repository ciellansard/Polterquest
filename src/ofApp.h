#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
//#include "ofxImGui.h"
#include "Mob.h"
#include "Door.h"
#include "ofArduino.h"

class ofApp : public ofBaseApp
{
	public:
		// Controller
		ofArduino m_arduino;
		const std::string ARDUINO_DEVICE_NAME = "COM6";
		static const int ARDUINO_BAUD_RATE = 57600;
		static const int PIN_BUTTON_INPUT = 8;
		static const int PIN_JOYSTICK_X_INPUT = 14;
		static const int PIN_JOYSTICK_Y_INPUT = 15;
		static const int PIN_ACCELEROMETER_X_INPUT = 18;
		static const int PIN_ACCELEROMETER_Y_INPUT = 19;
		bool m_buttonVal;
		ofVec2f m_joystickVal;
		ofVec3f m_accelerometerVal;
		bool m_bSetup;
		void setupArduino(const int& _version);
		void updateArduino();
		void digitalPinChanged(const int& pinNum);
		void analogPinChanged(const int& pinNum);

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

		// Doors
		std::vector<Door> doors;

		// Mobs
		std::vector<Mob> mobs;

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
