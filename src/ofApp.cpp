// -------------------------------------------------------------
//                       POLTERQUEST
// A ghost-hunting game created by Ciel Lansard and Shah Salter
// -------------------------------------------------------------

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
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
	m_cam.setPosition(0.0f, 1.8f, 0.0f);
	m_cam.setNearClip(1); // Eliminates camera clipping on nearby objects. Why does 1 disable it..?
	m_cam.rotateDeg(90.0f, 0, 1, 0);
	m_cam.disableMouseInput();
	m_camRotFactor = 0;
	m_camIsMoving = false;
	m_batteryPercentage = 1.0f;
	m_healthPercentage = 1.0f;

	ofSetBackgroundColor(ofColor::black);

	// Set up a light that follows the player
	m_camLight.setPointLight();
	m_camLight.setDiffuseColor(ofFloatColor(0.88f, 0.80f, 0.50f)); // Make the light slightly yellow
	m_camLight.setAttenuation(0.2f, 0.1f, 0.05f); // Dim the camera's attached light as it shines further away.  
	ofEnableLighting();
	m_camLight.enable();

	ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update()
{
	m_camLight.setPosition(m_cam.getPosition());
	m_cam.rotateDeg(0.5f * m_camRotFactor, 0, 1, 0);
	m_cam.setPosition(m_cam.getPosition().x + 0.05f * m_camIsMoving * m_cam.getLookAtDir().x, 
		              m_cam.getPosition().y, 
		              m_cam.getPosition().z + 0.05f * m_camIsMoving * m_cam.getLookAtDir().z);
	
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
		m_cam.setPosition(m_cam.getPosition().x, 1.8f + (sin(ofGetElapsedTimeMillis() / 100.0f) - 1.0f) / 20.0f, m_cam.getPosition().z);

		printf("\%: %f   | state: %i\n", m_batteryPercentage, m_batteryState);
	}
	else
	{
		// Player glow is a soft warm yellow.
		m_camLight.setDiffuseColor(ofFloatColor(0.88f, 0.80f, 0.50f));
		m_camLight.setAttenuation(0.2f + m_attenuationFactor, 0.1f + m_attenuationFactor, 0.05f + m_attenuationFactor);
		
		m_cam.setPosition(m_cam.getPosition().x, ofLerp(m_cam.getPosition().y, 1.8f, 0.1f), m_cam.getPosition().z);
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
				}
				m_tilesheet.unbind();

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
	if (key == 'w' || key == 'W')
	{
		m_camIsMoving = true;
	}

	// Suck
	if (key == 32)
	{
		m_isSucking = true;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
	if (key == 'w' || key == 'W') m_camIsMoving = false;
	if (key == 32) m_isSucking = false;
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
