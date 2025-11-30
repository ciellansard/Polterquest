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

	// Setup the camera
	m_cam.setAutoDistance(false);         //don't want camera moving around by itself
	m_cam.setPosition(0.0f, 2.0f, 0.0f); //set position of camera away from origin
	m_cam.setNearClip(1); // Eliminates camera clipping on nearby objects. Why does 1 disable it..?
	//camera.rotateDeg(0, 0, 0, 0);
	m_cam.disableMouseInput();
	m_camRotFactor = 0;
	m_camIsMoving = false;

	ofSetBackgroundColor(ofColor::black);


	// Setup a light
	m_camLight.setPointLight();
	m_camLight.setDiffuseColor(ofFloatColor(1.0f, 1.0f, 1.0f));
	m_camLight.setAttenuation(1.0f, 0.05f, 0.05f); // Dim the camera's attached light as it gets further away.  
	ofEnableLighting();
	m_camLight.enable();

	ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update(){
	m_camLight.setPosition(m_cam.getPosition());
	m_cam.rotateDeg(0.5f * m_camRotFactor, 0, 1, 0);
	m_cam.setPosition(m_cam.getPosition().x + 0.05f * m_camIsMoving * m_cam.getLookAtDir().x, m_cam.getPosition().y, m_cam.getPosition().z + 0.05f * m_camIsMoving * m_cam.getLookAtDir().z);
	//printf("%f, %f, %f\n", m_cam.getPitchDeg(), m_cam.getRollDeg(), m_cam.getHeadingDeg());
}

//--------------------------------------------------------------
void ofApp::draw(){
	//m_mansion.draw();
	m_camLight.enable();

	m_cam.begin();
	{
		m_tilesheet.bind();
		{
			ofPushMatrix();
			m_mansion.draw();
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
	/*
	if (key == 'a')
	{
		m_cam.setPosition(m_cam.getPosition().x - 0.1f * m_cam.getLookAtDir().x, m_cam.getPosition().y, m_cam.getPosition().z + 0.1f * m_cam.getLookAtDir().z);
	}
	if (key == 's')
	{
		m_cam.setPosition(m_cam.getPosition().x - 0.1f * m_cam.getLookAtDir().x, m_cam.getPosition().y, m_cam.getPosition().z - 0.1f * m_cam.getLookAtDir().z);
	}
	if (key == 'd')
	{
		m_cam.setPosition(m_cam.getPosition().x + 0.1f * m_cam.getLookAtDir().x, m_cam.getPosition().y, m_cam.getPosition().z - 0.1f * m_cam.getLookAtDir().z);
	}
	*/



	

	if (key == 32)
	{
		printf("(%f, %f)\n", m_cam.getHeadingRad(), m_cam.getLookAtDir().z);
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
