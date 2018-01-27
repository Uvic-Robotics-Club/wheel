#ifndef motor_h
#define motor_h

#include <serial/serial.h>
#include <stdexcept>
#include <string>

class USBMotor{
  public:
    std::vector<std::string> split(const std::string& s, char delimiter);
    std::vector<uint8_t> data_to_write;
    double wheel_radius;
    double distance;
    double a_input;
    double a_setpoint;
    double a_output;
    std::string data_returned;		//recieve info
    std::string usb_port;

    USBMotor(std::string usb_port_input);
    USBMotor();
    bool checkData();
    void setVelocity(int vel);
    void update(int vel);
    void sendData();
    void init(std::string usb_port_input);

    serial::Serial ser;
};


#endif
