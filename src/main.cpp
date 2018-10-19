#include <MsTimer2.h>
#include <L298N.h>
#include <RobotDefinitions.h>

//create a Left motor instance
L298N motorLeft(L298N_ENA, L298N_IN1, L298N_IN2);
//create a Right motor instance
L298N motorRight(L298N_ENB, L298N_IN3, L298N_IN4);

int nav_speed = NAV_SPEED;

char APP_STATE = APP_STATE_MANUAL;
char followerEnable = 0;

char chassisBusy = 0;
volatile int chassisNavTime = 0;

char chassisProgram = 0;
int progIt = 0;

char nav_program[NAV_PROGRAM_SIZE] = "FLFLFLFL";
char log_program[NAV_PROGRAM_SIZE] = {""};
char log_program_it = 0;

struct TaskContext_str
{
    int REQURENCY;
    int OFFSET;
    int ReqCnt;
    void (*execute)(void);
};

struct digital_sensor_str
{
    uint16_t pin;
    uint16_t dataRaw;
};

struct digital_sensor_str sensorLine[LINE_SENSOR_NR_OF];

struct distance_sensor_str
{
    uint16_t trigPin;
    uint16_t echoPin;
    uint32_t duration;
    uint32_t distance;
    uint16_t distanceBuffer[DIST_BUFF_SIZE];
    uint16_t buffPos;
};

struct distance_sensor_str sensorDistance[DISTANCE_SENSOR_NR_OF];

struct binary_sensor_str
{
    uint8_t pin;
    uint8_t dataRaw;

    int8_t abcCnt;
    int8_t abcMax;
    int8_t abcMin;

    uint8_t dataFilt;
};

struct binary_sensor_str sensorSoundDet;

struct binary_sensor_str sensorBumper[BUMPER_NR_OF] = {
    // pin / dataRaw / abcCnt / abcMax / abcMin / dataFilt
    {SENS_BUMPER_LEFT_PIN, 0, 0, 4, 0, 0},
    {SENS_BUMPER_RIGHT_PIN, 0, 0, 4, 0, 0}};

char morse_Codes[40][10] =
    {
        ".-",
        "-...",
        "-.-.",
        "-..",
        ".",
        "..-.",
        "--.",
        "....",
        "..",
        ".---",
        ".-.-",
        ".-..",
        "--",
        "-.",
        "---",
        ".--.",
        "--.-",
        ".-.",
        "...",
        "-",
        "..-",
        "...-",
        ".--",
        "-..-",
        "-.--",
        "--..",
        " "};
int buzzerPin = 13;

void ChassisNavSpeedUp(void)
{
    if (++nav_speed > NAV_SPEED_MAX)
        nav_speed = NAV_SPEED_MAX;
}

void ChassisNavSpeedInit(void)
{
    nav_speed = NAV_SPEED;
}

void ChassisStop(void)
{ // steps in ms
    // stop motors
    motorRight.stop();
    motorLeft.stop();
}

void ChassisStepFWD(int steps)
{ // steps in ms

    // set the speed
    motorRight.setSpeed(NAV_SPEED + NAV_SPEED_COR_R);
    motorLeft.setSpeed(NAV_SPEED + NAV_SPEED_COR_L);

    // stert mottors
    motorRight.forward();
    motorLeft.forward();

    // running for
    chassisNavTime = steps;
}

void ChassisStepBWD(int steps)
{ // steps in ms

    // set the speed
    motorRight.setSpeed(NAV_SPEED + NAV_SPEED_COR_R);
    motorLeft.setSpeed(NAV_SPEED + NAV_SPEED_COR_L);

    // stert mottors
    motorRight.backward();
    motorLeft.backward();

    // running for
    chassisNavTime = steps;
}

void ChassisTurnLeft(int angle)
{ // angle in ms

    // set the speed
    motorRight.setSpeed(NAV_SPEED + NAV_SPEED_COR_R);
    motorLeft.setSpeed(NAV_SPEED + NAV_SPEED_COR_L);

    // stert mottors
    motorRight.forward();
    motorLeft.backward();

    // running for
    chassisNavTime = angle;
}

void ChassisTurnRight(int angle)
{ // angle in ms

    // set the speed
    motorRight.setSpeed(NAV_SPEED + NAV_SPEED_COR_R);
    motorLeft.setSpeed(NAV_SPEED + NAV_SPEED_COR_L);

    // stert mottors
    motorRight.backward();
    motorLeft.forward();

    // running for
    chassisNavTime = angle;
}

void ChassisNavProgram(char program[])
{
    int i = 0;

    while (program[i] && i < NAV_PROGRAM_SIZE)
    {
        nav_program[i] = program[i];
        i++;
    }
    chassisProgram = 1;
    progIt = 0;

    MON_SERIAL.println(nav_program);
}

void ChassisTask_Run(void);
void SoundDetTask_Run(void);
void BumperTask_Run(void);
void LineSensorTask_Run(void);
void DistanceSensorTask_Run(void);

struct TaskContext_str TASK_List[NR_OF_TASKS] = {
    // REQURENCY / OFFSET / ReqCnt / runnable func;
    {CHASSIS_REQ, 3, 0, ChassisTask_Run},
    {1, 0, 0, SoundDetTask_Run},
    {10, 1, 0, BumperTask_Run},
    {100, 2, 0, LineSensorTask_Run}};

//===============================================
// LINE Sensor task

void AnalogSensorEval(struct digital_sensor_str &sensor)
{
    uint16_t dataRawLcl;
    // Acquisition
    dataRawLcl = analogRead(sensor.pin);
    sensor.dataRaw = dataRawLcl;
    // Evaluation
}

void LineSensorTask_Init(void)
{
    sensorLine[SENS_LINE_FAR_LEFT_ID].pin = SENS_LINE_FAR_LEFT;
    sensorLine[SENS_LINE_LEFT_ID].pin = SENS_LINE_LEFT;
    sensorLine[SENS_LINE_CENTER_ID].pin = SENS_LINE_CENTER;
    sensorLine[SENS_LINE_RIGHT_ID].pin = SENS_LINE_RIGHT;
    sensorLine[SENS_LINE_FAR_RIGHT_ID].pin = SENS_LINE_FAR_RIGHT;
}

void LineSensorTask_Run(void)
{
    for (int i = 0; i < LINE_SENSOR_NR_OF; i++)
    {
        AnalogSensorEval(sensorLine[i]);
    }
}

//============================
// Distance sensor

int distsort(const void *p, const void *q)
{
    int l = *(const int *)p;
    int r = *(const int *)q;

    if (l > r)
        return 1;
    if (l == r)
        return 0;
    if (l < r)
        return -1;
}
void DistanceSensorEval(struct distance_sensor_str &sensor)
{
    // Clears the trigPin
    long duration;
    int distance;
    digitalWrite(sensor.trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(sensor.trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(sensor.trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(sensor.echoPin, HIGH, MAX_DUR);
    // Calculating the distance

    if (duration <= 0)
        duration = MAX_DUR;
    distance = duration * 0.034 / 2;

    if (++sensor.buffPos >= DIST_BUFF_SIZE)
        sensor.buffPos = DIST_BUFF_SIZE;
    sensor.distanceBuffer[sensor.buffPos] = distance;

    // qsort(sensor.distanceBuffer, DIST_BUFF_SIZE, sizeof(sensor.distanceBuffer[0]), distsort);
    //
    // distance = sensor.distanceBuffer[DIST_BUFF_SIZE / 2 + 1];

    // int sum = 0;
    // for (int i = 0; i< DIST_BUFF_SIZE; i++){
    // sum+=sensor.distanceBuffer[i];
    // }
    //
    // distance = sum / DIST_BUFF_SIZE;
    //
    sensor.duration = duration;
    sensor.distance = distance;
    // Serial.print(sensor.trigPin);
    // Serial.print(" ");
    // Serial.print(sensor.echoPin);
    // Serial.print(" ");
    // Serial.print(sensor.distance);
    // Serial.print(" ");
}

// Distance sensor Pinmap

void DistanceSensorTask_Init(void)
{
    sensorDistance[SENS_US_LEFT_ID].trigPin = SENS_US_TRIG_LEFT_PIN;
    sensorDistance[SENS_US_LEFT_ID].echoPin = SENS_US_ECHO_LEFT_PIN;

    sensorDistance[SENS_US_CENTER_ID].trigPin = SENS_US_TRIG_CENTER_PIN;
    sensorDistance[SENS_US_CENTER_ID].echoPin = SENS_US_ECHO_CENTER_PIN;

    sensorDistance[SENS_US_RIGHT_ID].trigPin = SENS_US_TRIG_RIGHT_PIN;
    sensorDistance[SENS_US_RIGHT_ID].echoPin = SENS_US_ECHO_RIGHT_PIN;

    for (int i = 0; i < DISTANCE_SENSOR_NR_OF; i++)
    {
        pinMode(sensorDistance[i].trigPin, OUTPUT);
        pinMode(sensorDistance[i].echoPin, INPUT);
    }
}

void DistanceSensorTask_Run(void)
{
    for (int i = 0; i < DISTANCE_SENSOR_NR_OF; i++)
    {
        DistanceSensorEval(sensorDistance[i]);
    }
}

//===============================================
// TASK 1
int TestCnt = 0;
void ChassisTask_Run(void)
{
    if (chassisProgram && !chassisBusy)
    {

        if (nav_program[progIt])
        {
            switch (nav_program[progIt])
            {

            case 'F':
                CTRL_SERIAL.println("Am receptiont Comanda FWD");
                ChassisStepFWD(PROG_WALK_STEP);
                break;

            case 'B':
                CTRL_SERIAL.println("Am receptiont Comanda BWD");
                ChassisStepBWD(PROG_WALK_STEP);
                break;

            case 'L':
                CTRL_SERIAL.println("Am receptiont Comanda LEFT");
                ChassisTurnLeft(PROG_TURN_ANGLE);
                break;

            case 'R':
                CTRL_SERIAL.println("Am receptiont Comanda RIGHT");
                ChassisTurnRight(PROG_TURN_ANGLE);
                break;

            case 'S':
                CTRL_SERIAL.println("Am receptiont Comanda STOP");
                ChassisStop();
                break;
            }
            progIt++;
        }
        else
        {
            chassisProgram = 0;
            progIt = 0;
        }
    }
    //CTRL_SERIAL.println((int)chassisNavTime);

    if (chassisNavTime > 0)
    {
        chassisBusy = 1;
        chassisNavTime--;
    }
    else
    {
        chassisBusy = 0;
        ChassisStop();
    }
}
//===============================================
// SND DET TASK 2

char Get_SoundDetRaw(void)
{
    return sensorSoundDet.dataRaw;
}

void SoundDetTask_Init(void)
{
    sensorSoundDet.pin = SENS_SOUND_DET_PIN;
    pinMode(SENS_SOUND_DET_PIN, INPUT);
}

void SoundDetTask_Run(void)
{
    // Acquisition
    sensorSoundDet.dataRaw = digitalRead(SENS_SOUND_DET_PIN);

    // Evaluation
}

//===================================
// Bumper Task

char Get_BumperRaw(uint8_t sensorId)
{
    return sensorBumper[sensorId].dataRaw;
}
char Get_BumperAbcCnt(uint8_t sensorId)
{
    return sensorBumper[sensorId].abcCnt;
}
char Get_BumperFilt(uint8_t sensorId)
{
    return (sensorBumper[sensorId].dataFilt == 0);
}

void BumperTask_Init(void)
{
    sensorBumper[SENS_BUMPER_LEFT_ID].pin = SENS_BUMPER_LEFT_PIN;
    sensorBumper[SENS_BUMPER_RIGHT_ID].pin = SENS_BUMPER_RIGHT_PIN;

    for (int i = 0; i < BUMPER_NR_OF; i++)
    {
        pinMode(sensorBumper[i].pin, INPUT);
    }
}

void BinarySensorEval(struct binary_sensor_str &sensor)
{
    uint8_t dataRawLcl;
    // Acquisition
    dataRawLcl = digitalRead(sensor.pin);
    sensor.dataRaw = dataRawLcl;
    // Evaluation
    if (dataRawLcl > 0)
    {
        if (++sensor.abcCnt > sensor.abcMax)
        {
            sensor.abcCnt = sensor.abcMax; // saturation
        }
    }
    else
    {
        if (--sensor.abcCnt < sensor.abcMin)
        {
            sensor.abcCnt = sensor.abcMin; // saturation
        }
    }

    if (sensor.abcCnt <= sensor.abcMin)
    {
        sensor.dataFilt = 0;
    }
    else if (sensor.abcCnt >= sensor.abcMax)
    {
        sensor.dataFilt = 1;
    }
    else
    {
        // do nothing, keep the last value
    }
}

void BumperTask_Run(void)
{
    for (int i = 0; i < BUMPER_NR_OF; i++)
    {
        BinarySensorEval(sensorBumper[i]);
    }
}

//===============================================
void TaskScheduler(void)
{

    for (int taskNr = 0; taskNr < NR_OF_TASKS; taskNr++)
    {
        if (--TASK_List[taskNr].ReqCnt <= 0)
        {
            TASK_List[taskNr].execute();
            TASK_List[taskNr].ReqCnt = TASK_List[taskNr].REQURENCY;
        }
    }
}

// SYS TICK
void SysTick(void)
{
    // run task 1
    // Scheduler
    //CTRL_SERIAL.println("SYS TICK");

    TaskScheduler();
}
//===============================================
// MORSE Generator

void PlayMorse(char morse[])
{
    int i = 0;
    while (morse[i] != 0)
    {
        if (morse[i] == '.')
        {
            tone(buzzerPin, MORSE_FRQ, MORSE_DOT_DUR);
            delay(MORSE_DOT_DUR);
            noTone(buzzerPin);
        }
        else if (morse[i] == '-')
        {
            tone(buzzerPin, MORSE_FRQ, MORSE_DASH_DUR);
            delay(MORSE_DASH_DUR);
            noTone(buzzerPin);
        }
        else
        {
            //no tone
            delay(MORSE_PAUSE);
        }
        delay(MORSE_DOT_DUR);

        i++;
    }
    delay(MORSE_DOT_DUR);
}

void PlayMorseString(char morseStr[])
{
    int i = 0;
    while (morseStr[i] != 0)
    {
        PlayMorse(morse_Codes[morseStr[i] - 'A']);
        i++;
    }
}

void FollowerAppTask_Run(void)
{

    int minID = SENS_US_CENTER_ID;
    int minDist = sensorDistance[SENS_US_CENTER_ID].distance;

    if (minDist > sensorDistance[SENS_US_LEFT_ID].distance)
    {
        minID = SENS_US_LEFT_ID;
        minDist = sensorDistance[SENS_US_LEFT_ID].distance;
    }

    if (minDist > sensorDistance[SENS_US_RIGHT_ID].distance)
    {
        minID = SENS_US_RIGHT_ID;
        minDist = sensorDistance[SENS_US_RIGHT_ID].distance;
    }

    // Bumper diag
    if (!chassisBusy)
    {
        // if (Get_BumperFilt(SENS_BUMPER_LEFT_ID) ||
        // Get_BumperFilt(SENS_BUMPER_RIGHT_ID)) {
        // // bumper reaction
        // ChassisStepFWD(NAV_WALK_STEP);
        // } else
        if (minID == SENS_US_LEFT_ID)
        { // Angle Under
            ChassisNavSpeedInit();
            ChassisTurnLeft(NAV_TURN_ANGLE);
        }
        else if (minID == SENS_US_RIGHT_ID)
        { // Angle Over
            ChassisNavSpeedInit();
            ChassisTurnRight(NAV_TURN_ANGLE);
        }
        else if (minDist > DISTANCE_MAX)
        { // Distance Over
            ChassisStepFWD(NAV_WALK_STEP);
            ChassisNavSpeedUp();
        }
        else if (minDist < DISTANCE_MIN)
        { // Distance under
            ChassisNavSpeedInit();
            ChassisStepBWD(NAV_WALK_STEP);
        }
        else
        {
            ChassisStop();

            if (Get_SoundDetRaw())
            {
                PlayMorseString("SOS");
                ChassisNavProgram("LLFF");
            }
        }
    }
}
void TunelAppTask_Run(void)
{

    int maxID = SENS_US_CENTER_ID;
    int maxDist = sensorDistance[SENS_US_CENTER_ID].distance;

    if (maxDist < sensorDistance[SENS_US_LEFT_ID].distance)
    {
        maxID = SENS_US_LEFT_ID;
        maxDist = sensorDistance[SENS_US_LEFT_ID].distance;
    }

    if (maxDist < sensorDistance[SENS_US_RIGHT_ID].distance)
    {
        maxID = SENS_US_RIGHT_ID;
        maxDist = sensorDistance[SENS_US_RIGHT_ID].distance;
    }

    // Bumper diag
    if (!chassisBusy)
    {
        // if (Get_BumperFilt(SENS_BUMPER_LEFT_ID) ||
        // Get_BumperFilt(SENS_BUMPER_RIGHT_ID)) {
        // // bumper reaction
        // ChassisStepFWD(NAV_WALK_STEP);
        // } else
        if (maxID == SENS_US_LEFT_ID)
        { // Angle Under
            ChassisNavSpeedInit();
            ChassisTurnLeft(NAV_TURN_ANGLE);
        }
        else if (maxID == SENS_US_RIGHT_ID)
        { // Angle Over
            ChassisNavSpeedInit();
            ChassisTurnRight(NAV_TURN_ANGLE);
        }
        else if (maxDist > DISTANCE_MAX)
        { // Distance Over
            ChassisStepFWD(NAV_WALK_STEP);
            ChassisNavSpeedUp();
        }
        else if (maxDist < DISTANCE_MIN)
        { // Distance under
            ChassisStepBWD(NAV_WALK_STEP);
        }
        else
        {
            ChassisStop();

            if (Get_SoundDetRaw())
            {
                PlayMorseString("SOS");
                ChassisNavProgram("LLFF");
            }
        }
    }
}

void setup()
{
    CTRL_SERIAL.begin(9600);
    MON_SERIAL.begin(9600);

    pinMode(13, OUTPUT);

    SoundDetTask_Init();
    BumperTask_Init();
    LineSensorTask_Init();
    DistanceSensorTask_Init();

    MsTimer2::set(SYS_TICK_TIME, SysTick); // 1ms period
    MsTimer2::start();
}

void loop()
{

    DistanceSensorTask_Run();

    if (APP_STATE == APP_STATE_AUTO_FOLLOW)
    {
        FollowerAppTask_Run();
    }
    else if (APP_STATE == APP_STATE_AUTO_TUNEL)
    {
        TunelAppTask_Run();
    }

    if (CTRL_SERIAL.available())
    {
        char incommingByte = CTRL_SERIAL.read();
        switch (incommingByte)
        {

        case '0':
            CTRL_SERIAL.println("Am receptiont Comanda 0");
            PlayMorseString("SOS");
            break;

        case '1':
            CTRL_SERIAL.println("Am receptiont Comanda 1 - Manual Control");
            APP_STATE = APP_STATE_MANUAL;
            break;

        case '2':
            CTRL_SERIAL.println("Am receptiont Comanda 2 - Follower");
            APP_STATE = APP_STATE_AUTO_FOLLOW;
            break;

        case '3':
            CTRL_SERIAL.println("Am receptiont Comanda 3 - Tunel");
            APP_STATE = APP_STATE_AUTO_TUNEL;
            break;

        case 'P':
            CTRL_SERIAL.println("Am receptiont Comanda P - PROGRAM");
            ChassisNavProgram("FFFLRFFFLBBBBBB");
            break;

        case 'Y':
            CTRL_SERIAL.println("Am receptiont Comanda y - Play Log");
            ChassisNavProgram(log_program);
            break;

        case 'X':
            CTRL_SERIAL.println("Am receptiont Comanda x - clean");
            log_program[0] = 0;
            break;

        case 'C':
            CTRL_SERIAL.println("Am receptiont Comanda c - Copy Log");
            strcpy(nav_program, log_program);
            break;

        case 'F':
            CTRL_SERIAL.println("Am receptiont Comanda FWD");
            ChassisStepFWD(NAV_WALK_STEP);
            if (log_program_it < NAV_PROGRAM_SIZE)
            {
                log_program[log_program_it++] = incommingByte;
                log_program[log_program_it] = 0;
            }
            break;

        case 'B':
            CTRL_SERIAL.println("Am receptiont Comanda BWD");
            ChassisStepBWD(NAV_WALK_STEP);
            if (log_program_it < NAV_PROGRAM_SIZE)
            {
                log_program[log_program_it++] = incommingByte;
                log_program[log_program_it] = 0;
            }

            break;

        case 'L':
            CTRL_SERIAL.println("Am receptiont Comanda LEFT");
            ChassisTurnLeft(NAV_TURN_ANGLE);
            if (log_program_it < NAV_PROGRAM_SIZE)
            {
                log_program[log_program_it++] = incommingByte;
                log_program[log_program_it] = 0;
            }
            break;

        case 'R':
            CTRL_SERIAL.println("Am receptiont Comanda RIGHT");
            ChassisTurnRight(NAV_TURN_ANGLE);
            if (log_program_it < NAV_PROGRAM_SIZE)
            {
                log_program[log_program_it++] = incommingByte;
                log_program[log_program_it] = 0;
            }
            break;

        case 'S':
            CTRL_SERIAL.println("Am receptiont Comanda STOP");
            ChassisStop();
            if (log_program_it < NAV_PROGRAM_SIZE)
            {
                log_program[log_program_it++] = incommingByte;
                log_program[log_program_it] = 0;
            }
            break;

        default:
            CTRL_SERIAL.println("Comanda necunoscuta ");
        }
    }
}