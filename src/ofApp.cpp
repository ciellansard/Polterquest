// -------------------------------------------------------------
//                       POLTERQUEST
// A ghost-hunting game created by Ciel Lansard and Shah Salter
// -------------------------------------------------------------

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetVerticalSync(true);
	ofSetBackgroundColor(ofColor::black);

	// Load 3D models
	ofxAssimpModelLoader loader;
	loader.loadModel("models/mansion.obj");
	m_mansion = loader.getMesh(0);
	loader.loadModel("models/battery.obj");
	m_battery = loader.getMesh(0);

	// Load textures
	ofDisableArbTex();
	ofLoadImage(m_tilesheet, "images/tilesheetLarge.png");
	for (int i = 0; i < 7; i++)
	{
		std::string path = "images/UI/battery" + ofToString(i) + ".png";
		ofLoadImage(m_batterySprites[i], "images/UI/battery" + ofToString(i) + ".png");
	}

	// Set up the camera / player info
	m_cam.setAutoDistance(false);
	m_cam.setPosition(1.0f, 1.8f, 0.0f);
	m_cam.setNearClip(0.3f); // Minimizes camera clipping on nearby objects.
	m_cam.rotateDeg(90.0f, 0, 1, 0);
	m_cam.disableMouseInput();
	m_camRotFactor = 0;
	m_camMoveFactor = 0;
	m_batteryPercentage = 1.0f;
	m_healthPercentage = 1.0f;

	// Set up controller
	m_arduino.connect(ARDUINO_DEVICE_NAME, ARDUINO_BAUD_RATE);
	ofAddListener(m_arduino.EInitialized, this, &ofApp::setupArduino);
	m_bSetup = false;

	m_joystickVal = { 0.0f, 0.0f };
	m_buttonVal = 0;

	// Set up a light that follows the player
	m_camLight.setPointLight();
	m_camLight.setDiffuseColor(ofFloatColor(0.88f, 0.80f, 0.50f)); // Make the light slightly yellow
	m_camLight.setAttenuation(0.2f, 0.1f, 0.05f); // Dim the camera's attached light as it shines further away.  
	ofEnableLighting();
	m_camLight.enable();
	
	// Create mobs
	mobs.push_back(Mob(0, NULL, {-8, 0, -3}, 0, &m_cam));
	mobs.push_back(Mob(0, NULL, { -8, 0, 3 }, 0, &m_cam));
	
	// Create doors
	doors.push_back(Door({  -5, 0,  -5 }, -180.0f,      true));
	doors.push_back(Door({ -10, 0,   0 }, -90.0f, false));
	doors.push_back(Door({  -5, 0, -15 }, 0,      false));
	doors.push_back(Door({ -12, 0, -19 }, -90.0f, false));
	doors.push_back(Door({   2, 0, -19 }, -90.0f, false));
	doors.push_back(Door({  -5, 0, -23 }, -180.0f,      false));
	doors.push_back(Door({ 20, 0, -50 }, -90.0f, false));

	// Create sectors
	sectors.push_back(Sector(0, 5, -10, -5));

	m_sector = sectors.at(0);

	ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update()
{
	updateArduino();
	//joystickControl();  // Remove slashes to play with custom controller
	//buttonControl();

	m_camLight.setPosition(m_cam.getPosition());
	m_cam.rotateDeg(0.75f * m_camRotFactor, 0, 1, 0);
	m_cam.setPosition(m_cam.getPosition().x + 0.05f * m_camMoveFactor * m_cam.getLookAtDir().x, 
		              m_cam.getPosition().y, 
		              m_cam.getPosition().z + 0.05f * m_camMoveFactor * m_cam.getLookAtDir().z);


	//fitToSector(); // Collision check
	

	for (int i = 0; i < doors.size(); i++)
	{
		if (doors.at(i).isClosed && abs(m_cam.getPosition().x - doors.at(i).pos.x) <= 1 && abs(m_cam.getPosition().z - doors.at(i).pos.z) <= 1) doors.at(i).unlock();
	}

	


	// Dim light as battery gets low.
	m_attenuationFactor = ofMap(m_batteryPercentage, 0.5f, 0.0f, 0.0f, 0.5f);

	if (m_isSucking)
	{
		// Player glow is a bright, unnatural teal.
		m_camLight.setDiffuseColor(ofFloatColor(0.53f, 0.90f, 0.64));
		m_camLight.setAttenuation(0.05f + m_attenuationFactor, 0.05f + m_attenuationFactor, 0.01f + m_attenuationFactor);

		// Drain the gun's battery.
		m_batteryPercentage -= 0.0002f;
		if (m_batteryPercentage < 0) m_batteryPercentage = 0;

		// Camera shake
		m_cam.setPosition(m_cam.getPosition().x, 1.8f + (sin(ofGetElapsedTimeMillis() / 20.0f) - 1.0f) / 60.0f, m_cam.getPosition().z);
	}

	else
	{
		// Player glow is a soft warm yellow.
		m_camLight.setDiffuseColor(ofFloatColor(0.88f, 0.80f, 0.50f));
		m_camLight.setAttenuation(0.2f + m_attenuationFactor, 0.1f + m_attenuationFactor, 0.05f + m_attenuationFactor);
		
		m_cam.setPosition(m_cam.getPosition().x, ofLerp(m_cam.getPosition().y, 1.8f, 0.1f), m_cam.getPosition().z);
	}

	for (int i = 0; i < mobs.size(); i++)
	{
		mobs.at(i).update();
	}

	float sign = (m_cam.getOrientationEulerDeg().x == -180.0f || m_cam.getOrientationEulerDeg().x == 180.0f) ? -1.0f : 1.0f;
	m_uiRot = m_cam.getOrientationEulerDeg().y * sign + ((sign == -1.0f) ? 180.0f : 0);
}

//--------------------------------------------------------------
void ofApp::draw(){
	m_camLight.enable();

	m_cam.begin();
	{
		ofPushMatrix();
		{
			m_tilesheet.bind();
			{
				m_mansion.draw();
				for (int i = 0; i < doors.size(); i++)
				{
					doors.at(i).draw();
				}
			}
			m_tilesheet.unbind();

			for (int i = 0; i < mobs.size(); i++)
			{
				mobs.at(i).draw();
			}



			// Set up UI
			ofTranslate(m_cam.getPosition()); // Follow the player/cam's position
			ofTranslate(-3.0f * m_cam.getZAxis().x, -1, -3.0f * m_cam.getZAxis().z); // Add an offset to avoid clipping
			ofRotate(m_uiRot - 90.0f, 0, 1, 0); // Rotate with camera
			ofDisableLighting(); // Keep UI the same brightness regardless of lighting
			ofDisableDepthTest(); // Always draw UI last
				
			// Draw battery gauge
			ofPushMatrix();
			{
				ofTranslate(0, -0.5f, 2.0f);
				ofScale(10.0f/40.0f);

				m_batteryState = ceil(m_batteryPercentage * 6.0f); // Take 0.0f - 1.0f range, convert to int between 0 - 6

				m_batterySprites[m_batteryState].bind();
				{
					m_battery.draw();
				}
				m_batterySprites[m_batteryState].unbind();
			}
			ofPopMatrix();
			ofEnableLighting();
			ofEnableDepthTest();
		}
	}
	m_cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	// Move forward
	if (key == 'w' || key == 'W') m_camMoveFactor = 1;
	else if (key == 's' || key == 'S') m_camMoveFactor = -1;

	// Suck
	if (key == 32)
	{
		m_isSucking = true;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
	if (key == 'w' || key == 'W' || key == 's' || key == 'S') m_camMoveFactor = 0;
	if (key == 32) m_isSucking = false;
}

//--------------------------------------------------------------
void ofApp::setupArduino(const int& version) 
{
	// remove listener because we don't need it anymore
	ofRemoveListener(m_arduino.EInitialized, this, &ofApp::setupArduino);

	// it is now safe to send commands to the Arduino
	m_bSetup = true;

	m_arduino.sendDigitalPinMode(PIN_BUTTON_INPUT, ARD_INPUT);
	m_arduino.sendAnalogPinReporting(PIN_JOYSTICK_X_INPUT, ARD_ANALOG);
	m_arduino.sendAnalogPinReporting(PIN_JOYSTICK_Y_INPUT, ARD_ANALOG);
}

//--------------------------------------------------------------
void ofApp::updateArduino() 
{
	m_arduino.update();

	if (m_bSetup) {
		m_joystickVal.x = m_arduino.getAnalog(PIN_JOYSTICK_X_INPUT);
		m_joystickVal.y = m_arduino.getAnalog(PIN_JOYSTICK_Y_INPUT);
		m_buttonVal = m_arduino.getDigital(PIN_BUTTON_INPUT);
		// cout << "(" << m_joystickVal.x << ", " << m_joystickVal.y << ")   |   (" << m_accelerometerVal.x << ", " << m_accelerometerVal.y << ")   |   " << m_buttonVal << "\n";
	}
}


void ofApp::joystickControl() 
{
	int driftTolerance = 16;

	// Rotate camera left/right
	if (m_joystickVal.y < 512 - driftTolerance) m_camRotFactor = 1;
	else if (m_joystickVal.y > 512 + driftTolerance) m_camRotFactor = -1;
	else m_camRotFactor = 0;

	// Forward/backward movement
	if (m_joystickVal.x > 512 + driftTolerance) m_camMoveFactor = -1;
	else if (m_joystickVal.x < 512 - driftTolerance) m_camMoveFactor = 1;
	else m_camMoveFactor = 0;
}

void ofApp::buttonControl() 
{
	if (m_buttonVal == 0) m_isSucking = true;
	else m_isSucking = false;
}

void ofApp::fitToSector()
{
	//Constrain player to a sector
	
	float sectorMargin = 0.5f;
	float fitTally;
	bool sectorFound = false;

	for (int i = 0; i < sectors.size(); i++)
	{
		fitTally = 0;
		if (m_cam.getPosition().x >= sectors.at(i).minX + sectorMargin) fitTally++;
		if (m_cam.getPosition().x <= sectors.at(i).maxX - sectorMargin) fitTally++;
		if (m_cam.getPosition().z >= sectors.at(i).minZ + sectorMargin) fitTally++;
		if (m_cam.getPosition().z <= sectors.at(i).maxZ - sectorMargin) fitTally++;

		if (fitTally == 4)
		{
			m_sector = sectors.at(i);
			sectorFound = true;
			break;
		}
	}

	if (!sectorFound)
	{
		if (m_cam.getPosition().x < m_sector.minX + sectorMargin) m_cam.setPosition(m_sector.minX + sectorMargin, m_cam.getPosition().y, m_cam.getPosition().z);
		if (m_cam.getPosition().x > m_sector.maxX - sectorMargin) m_cam.setPosition(m_sector.maxX - sectorMargin, m_cam.getPosition().y, m_cam.getPosition().z);
		if (m_cam.getPosition().z < m_sector.minZ + sectorMargin) m_cam.setPosition(m_cam.getPosition().x, m_cam.getPosition().y, m_sector.minZ + sectorMargin);
		if (m_cam.getPosition().z > m_sector.maxZ - sectorMargin) m_cam.setPosition(m_cam.getPosition().x, m_cam.getPosition().y, m_sector.maxZ - sectorMargin);
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (button == 0)
	{
		m_camRotFactor = 1;
	}
	if (button == 2)
	{
		m_camRotFactor = -1;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
	m_camRotFactor = 0;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
