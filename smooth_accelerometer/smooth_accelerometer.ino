#define SAMPLES 10

class SmoothAccelerometer
{
  public:
    SmoothAccelerometer();

    void init();
    void process();

    int16_t xAxis;
    int16_t yAxis;
    int16_t zAxis;

    int16_t xRaw;
    int16_t yRaw;
    int16_t zRaw;

  private:
    int16_t _nX[SAMPLES];
    int16_t _nY[SAMPLES];
    int16_t _nZ[SAMPLES];
    int _nReadingIndex;

    int16_t xAvg();
    int16_t yAvg();
    int16_t zAvg();
};

SmoothAccelerometer accel;
unsigned long nLastOutput = 0;

void setup()
{
  // This is to extend start-up idle time in case needed for uploading sketches
  Bean.sleep(1000);

  // Initial Serial interface
  Serial.begin(57600);
  Serial.setTimeout(10);

  // Seed random function in case we need it
  randomSeed(analogRead(A1));

  accel.init();

  Serial.println( "setup() done." );
  delay(10);

    String strMsg = "";
    strMsg += "\r\n";
    strMsg += "millis,x-raw,x-smooth,y-raw,y-smooth,z-raw,z-smooth";
    strMsg += "\r\n";
    Serial.print( strMsg );
    delay(10);
}

void loop() 
{
  unsigned long nNow = millis();

  accel.process();

  // Output data every 100 millis
  if( (nLastOutput == 0) || (nNow < nLastOutput) || ( (nNow - nLastOutput) > 150) )
  {
    nLastOutput = nNow;

    // Output millis and raw and smoothed readings
    String strMsg = "";
    strMsg += nNow;
    strMsg += ",";
    strMsg += accel.xRaw;
    strMsg += ",";
    strMsg += accel.xAxis;
    strMsg += ",";
    strMsg += accel.yRaw;
    strMsg += ",";
    strMsg += accel.yAxis;
    strMsg += ",";
    strMsg += accel.zRaw;
    strMsg += ",";
    strMsg += accel.zAxis;
    strMsg += "\r\n";
    Serial.print( strMsg );
  }

  delay(5);
}

SmoothAccelerometer::SmoothAccelerometer()
{

}

void SmoothAccelerometer::init()
{
  // Initialize accelerometer reading arrays
  AccelerationReading accXYZ = Bean.getAcceleration();
  xRaw = accXYZ.xAxis;
  yRaw = accXYZ.yAxis;
  zRaw = accXYZ.zAxis;
  _nReadingIndex = 0;
  for(int i=0; i<SAMPLES; i++ ) { _nX[i] = xRaw; }
  for(int i=0; i<SAMPLES; i++ ) { _nY[i] = yRaw; }
  for(int i=0; i<SAMPLES; i++ ) { _nZ[i] = zRaw; }

  // Initialize axis values
  xAxis = xAvg();
  yAxis = yAvg();
  zAxis = zAvg();
}

void SmoothAccelerometer::process()
{
  // Get current accelerometer readings
  AccelerationReading accXYZ = Bean.getAcceleration();
  xRaw = accXYZ.xAxis;
  yRaw = accXYZ.yAxis;
  zRaw = accXYZ.zAxis;

  // Update reading arrays
  _nX[_nReadingIndex] = xRaw;
  _nY[_nReadingIndex] = yRaw;
  _nZ[_nReadingIndex] = zRaw;
  _nReadingIndex++;
  if( _nReadingIndex == SAMPLES ) _nReadingIndex = 0;

  // Update axis values
  xAxis = xAvg();
  yAxis = yAvg();
  zAxis = zAvg();
}

int16_t SmoothAccelerometer::xAvg()
{
  int32_t nX = 0;
  for( int i=0; i<SAMPLES; i++)
  {
    nX += _nX[i];
  }
  return (int16_t) (nX / SAMPLES);
}

int16_t SmoothAccelerometer::yAvg()
{
  int32_t nY = 0;
  for( int i=0; i<SAMPLES; i++)
  {
    nY += _nY[i];
  }
  return (int16_t) (nY / SAMPLES);
}

int16_t SmoothAccelerometer::zAvg()
{
  int32_t nZ = 0;
  for( int i=0; i<SAMPLES; i++)
  {
    nZ += _nZ[i];
  }
  return (int16_t) (nZ / SAMPLES);
}
