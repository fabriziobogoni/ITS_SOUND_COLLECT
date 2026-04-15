/* Defines  ----------------------------------------------------------*/


#define SENSOR_SAMPLES 1024  //buffer size

#define AXIS 1        //microphone is 1 axis
#define DOWNSAMPLE 8  //microphone as a very high data rate, we downsample it
/* Prototypes ----------------------------------------------------------*/
void get_microphone_data();  //function to collect buffer of sound
/* Global variables ----------------------------------------------------------*/
static uint16_t neai_ptr = 0;                               //pointers to fill for sound buffer
static float neai_buffer[SENSOR_SAMPLES * AXIS] = { 0.0 };  //souhnd buffer
int const AMP_PIN = A0;                                     // Preamp output pin connected to A0
int state = LOW;
int myloop = 0;
/* Setup function ----------------------------------------------------------*/
void setup() {
	pinMode(D7, OUTPUT);

	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(115200);
	while(!Serial);
	delay(10);
}
/* Infinite loop ----------------------------------------------------------*/
void loop() {
	get_microphone_data();
	delay(200);
}
/* Functions declaration ----------------------------------------------------------*/
void get_microphone_data() {
	static uint16_t temp = 0;  //stock values
	int sub = 0;               //increment to downsample
	digitalWrite(LED_BUILTIN, HIGH);
	//while the buffer is not full
	while (neai_ptr < SENSOR_SAMPLES) {
		//we only get a value every DOWNSAMPLE (32 in this case)
		if (sub > DOWNSAMPLE) {
			/* Fill neai buffer with new accel data */
			neai_buffer[neai_ptr] = analogRead(AMP_PIN);
			/* Increment neai pointer */
			neai_ptr++;
			sub = 0;  //reset increment
			digitalWrite(D7, state);		//per debug, capire la velocità di campinamento
			state = !state;
		} else {
			//we read the sample even if we don't use it
			//else it is instantaneous and we don't downsample
			temp = analogRead(AMP_PIN);
		}
		sub++;
	}
	digitalWrite(LED_BUILTIN, LOW);
	//print the buffer values to send them via serial
	for (uint16_t i = 0; i < SENSOR_SAMPLES; i++) {
		Serial.print(neai_buffer[i]);
		Serial.print(" ");
	}
	Serial.print("\n");
	neai_ptr = 0;  //reset the beginning position
}
