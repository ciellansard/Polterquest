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

	// Load textures
	ofDisableArbTex();
	ofLoadImage(m_tilesheet, "images/tilesheetLarge.png");

	// Set up the camera / player info
	m_cam.setAutoDistance(false);
	m_cam.setPosition(0.0f, 2.0f, 0.0f);
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
	
	//if (m_batteryPercentage > 0) m_batteryPercentage -= 0.01f;

	// Dim player's light as their battery depletes. If it's near 0, dim the light substantially. If above 0.5, do not affect.
	//m_camLight.setAttenuation(0.2f + ofMap(m_batteryPercentage, 0.5f, 0.0f, 0.0f, 100.0f), 0.1f, 0.05f);
}

//--------------------------------------------------------------
void ofApp::draw(){
	m_camLight.enable();

	m_cam.begin();
	{
		m_tilesheet.bind();
		{
			ofPushMatrix();
			{
				m_mansion.draw();
			}
			ofPopMatrix();
		}
		m_tilesheet.unbind();
	}
	m_cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == 'w')
	{
		m_camIsMoving = true;
	}
	if (key == 32)
	{
		printf("(%f, %f)\n", m_cam.getGlobalPosition().x, m_cam.getGlobalPosition().z);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
	if (key == 'w') m_camIsMoving = false;
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
