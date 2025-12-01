// [ES] Incluir las funciones de Arduino
#include <Arduino.h>

// ! [ES] Incluir la configuración de RemoteXY (necesita ser importada antes de MechaLib)
#include "./remotexy_conf.h"

// [ES] Incluir MechaLib
#include <MechaLib.h>

/**
 * [ES] Definir la clase Robot que extiende RobotBase para el código principal del robot
 * El manejo de RemoteXY se realiza automáticamente por MechaLib
 */
class Carro : public RobotBase {
    protected:
        //Motores
        MOTOR_CONTROLLER_L298N frontLeft{0, 0, 0};
        MOTOR_CONTROLLER_L298N frontRight{0, 0, 0};
        MOTOR_CONTROLLER_L298N backLeft{0, 0, 0};
        MOTOR_CONTROLLER_L298N backRight{0, 0, 0};

        float wheelSpeeds[4];
        MecanumDriveKinematics* kinematics = new MecanumDriveKinematics{30.0 / 1000.0, 20.0 / 1000.0, 20.0 / 1000.0};

    public:
        void robotSetup() override {
            Logger::getInstance().log("MechaLeague - RemoteXY Example");

            frontLeft.begin();
            frontRight.begin();
            backLeft.begin();
            backRight.begin();
        }

        void robotLoop() override {
            Logger::getInstance().log("Left Joystick: (" + String(RemoteXY.leftx) + ", " + String(RemoteXY.lefty) + ")");
            Logger::getInstance().log("Right Joystick: (" + String(RemoteXY.rightx) + ", " + String(RemoteXY.righty) + ")");

            kinematics->calculateWheelSpeeds(
                this->wheelSpeeds,
                RemoteXY.leftx / 100.0,
                RemoteXY.lefty / 100.0,
                RemoteXY.rightx / 100.0
            );
            MecanumDriveKinematics::normalizeWheelSpeeds(this->wheelSpeeds);

            frontLeft.set(this->wheelSpeeds[0]);
            frontRight.set(this->wheelSpeeds[1]);
            backLeft.set(this->wheelSpeeds[2]);
            backRight.set(this->wheelSpeeds[3]);
        }
};

// [ES] Crear instancia del robot
Carro carro;

//[ES] Registrar la instancia del robot para que MechaLib pueda gestionar las llamadas de setup y loop automáticamente

REGISTER_ROBOT(carro);