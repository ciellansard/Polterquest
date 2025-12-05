// -------------------------------------------------------------
//                       POLTERQUEST
// A ghost-hunting game created by Ciel Lansard and Shah Salter
// -------------------------------------------------------------

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
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
	m_cam.setPosition(0.0f, 1.8f, 0.0f);
	m_cam.setNearClip(1); // Minimizes camera clipping on nearby objects.
	m_cam.rotateDeg(90.0f, 0, 1, 0);
	m_cam.disableMouseInput();
	m_camRotFactor = 0;
	m_camIsMoving = false;
	m_batteryPercentage = 1.0f;
	m_healthPercentage = 1.0f;


	// Set up controller
	//m_serial.enumerateDevices();
	//m_serial.setup(0, 9600);
	m_arduino.connect("COM6", 9600);

	// Listen for EInitialized notification. this indicates that
	// the arduino is ready to receive commands and it is safe to
	// call setupArduino()
	ofAddListener(m_arduino.EInitialized, this, &ofApp::setupArduino);
	m_bSetup = false;

	m_joystickVal = { 0.0f, 0.0f };
	m_accelerometerVal = { 0.0f, 0.0f, 0.0f };

	// Set up a light that follows the player
	m_camLight.setPointLight();
	m_camLight.setDiffuseColor(ofFloatColor(0.88f, 0.80f, 0.50f)); // Make the light slightly yellow
	m_camLight.setAttenuation(0.2f, 0.1f, 0.05f); // Dim the camera's attached light as it shines further away.  
	ofEnableLighting();
	m_camLight.enable();
	
	// Create mobs
	mobs.push_back(Mob(0, {2, 0, 10}, {0, 0, 0}));


	ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update()
{
	updateArduino();

	//

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

		//printf("\%: %f   | state: %i\n", m_batteryPercentage, m_batteryState);
	}
	else
	{
		// Player glow is a soft warm yellow.
		m_camLight.setDiffuseColor(ofFloatColor(0.88f, 0.80f, 0.50f));
		m_camLight.setAttenuation(0.2f + m_attenuationFactor, 0.1f + m_attenuationFactor, 0.05f + m_attenuationFactor);
		
		m_cam.setPosition(m_cam.getPosition().x, ofLerp(m_cam.getPosition().y, 1.8f, 0.1f), m_cam.getPosition().z);
	}

	//for (int i = 0; i < mobs.size(); i++)
	{
		//mobs.at(i).update();
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

			//for (int i = 0; i < mobs.size(); i++)
			{
				//mobs.at(i).draw();
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
void ofApp::setupArduino(const int& version) {

	// remove listener because we don't need it anymore
	ofRemoveListener(m_arduino.EInitialized, this, &ofApp::setupArduino);

	// it is now safe to send commands to the Arduino
	m_bSetup = true;

	// print firmware name and version to the console
	ofLogNotice() << m_arduino.getFirmwareName();
	ofLogNotice() << "firmata v" << m_arduino.getMajorFirmwareVersion() << "." << m_arduino.getMinorFirmwareVersion();

	// Note: pins A0 - A5 can be used as digital input and output.
	// Refer to them as pins 14 - 19 if using StandardFirmata from Arduino 1.0.
	// If using Arduino 0022 or older, then use 16 - 21.
	// Firmata pin numbering changed in version 2.3 (which is included in Arduino 1.0)

	

	// set pins D2 and A5 to digital input
	m_arduino.sendDigitalPinMode(2, ARD_INPUT);
	m_arduino.sendDigitalPinMode(19, ARD_INPUT);  // pin 21 if using StandardFirmata from Arduino 0022 or older

	// set pin A0 to analog input
	m_arduino.sendAnalogPinReporting(0, ARD_ANALOG);

	// set pin D13 as digital output
	m_arduino.sendDigitalPinMode(13, ARD_OUTPUT);
	// set pin A4 as digital output
	m_arduino.sendDigitalPinMode(18, ARD_OUTPUT);  // pin 20 if using StandardFirmata from Arduino 0022 or older

	// set pin D11 as PWM (analog output)
	m_arduino.sendDigitalPinMode(11, ARD_PWM);

	// attach a servo to pin D9
	// servo motors can only be attached to pin D3, D5, D6, D9, D10, or D11
	m_arduino.sendServoAttach(9);

	// Listen for changes on the digital and analog pins
	ofAddListener(m_arduino.EDigitalPinChanged, this, &ofApp::digitalPinChanged);
	ofAddListener(m_arduino.EAnalogPinChanged, this, &ofApp::analogPinChanged);
}

//--------------------------------------------------------------
void ofApp::updateArduino() {

	// update the arduino, get any data or messages.
	// the call to ard.update() is required
	m_arduino.update();
	printf("%i\n\n", m_arduino.getTotalPins());
	//m_buttonVal = m_arduino.getDigital(PIN_BUTTON_INPUT);
	//m_joystickVal.set(m_arduino.getAnalog(PIN_JOYSTICK_X_INPUT), m_arduino.getAnalog(PIN_JOYSTICK_Y_INPUT));
	//m_accelerometerVal.set(m_arduino.getAnalog(PIN_ACCELEROMETER_X_INPUT), m_arduino.getAnalog(PIN_ACCELEROMETER_Y_INPUT));

	//printf("%i     |     ( %f, %f )     |     ( %f, %f, %f )\n\n", (int)m_buttonVal, m_joystickVal.x, m_joystickVal.y, m_accelerometerVal.x, m_accelerometerVal.y, m_accelerometerVal.z);

	// do not send anything until the arduino has been set up
	if (m_bSetup) {
		// fade the led connected to pin D11
		m_arduino.sendPwm(11, (int)(128 + 128 * sin(ofGetElapsedTimef())));   // pwm...
	}

}

// digital pin event handler, called whenever a digital pin value has changed
// note: if an analog pin has been set as a digital pin, it will be handled
// by the digitalPinChanged function rather than the analogPinChanged function.

//--------------------------------------------------------------
void ofApp::digitalPinChanged(const int& pinNum) {
	// do something with the digital input. here we're simply going to print the pin number and
	// value to the screen each time it changes
	//buttonState = "digital pin: " + ofToString(pinNum) + " = " + ofToString(ard.getDigital(pinNum));
}

// analog pin event handler, called whenever an analog pin value has changed

//--------------------------------------------------------------
void ofApp::analogPinChanged(const int& pinNum) {
	// do something with the analog input. here we're simply going to print the pin number and
	// value to the screen each time it changes
	//potValue = "analog pin: " + ofToString(pinNum) + " = " + ofToString(ard.getAnalog(pinNum));
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
