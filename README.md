# ESP-AWS-IoT

This framework enables AWS IoT cloud connectivity with ESP32 based platforms using [AWS IoT Device Embedded SDK](https://github.com/aws/aws-iot-device-sdk-embedded-C).

## Getting Started

- Please clone this repository using,
    ```
    git clone --recursive https://github.com/espressif/esp-aws-iot
    ```
- Please refer to https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html for setting ESP-IDF
  - ESP-IDF can be downloaded from https://github.com/espressif/esp-idf/
  - ESP-IDF v3.1 and above is recommended version
- Please refer to [example README](examples/README.md) for more information on setting up examples

## Using this library with an ATECC608a
This port contains support for using a secure element chip from microchip.  This will store the private key used for TLS communication in the secure element.  To use this function, perform the following tasks:

1. Add [esp-cryptoauthlib](https://github.com/espressif/esp-cryptoauthlib) to your project's components, and make sure it is configured
1. Turn on AWS_IOT_USE_HARDWARE_SECURE_ELEMENT in the sdkconfig of your project.
1. Ensure that you call `atcab_init` to initialise the microchip library before you use the AWS IoT library
1. When connecting to AWS IoT Core, change your connection configuration to;
  1. If you wish to use the certificate stored in the ATECC608a, set `mqttInitParams.pDeviceCertLocation = "#"`.  You may still use a certificate stored in code or on the filesystem, if you wish
  1. Tell the IoT library to find the Private key in a slot by setting `mqttInitParams.pDevicePrivateKeyLocation = “#0”`, where the digit 0 indicates the slot in the ATECC608a that should be used to sign requests.  In most circumstances, the key will be stored in slot 0
