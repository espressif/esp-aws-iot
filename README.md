# ESP-AWS-IoT

> **IMPORTANT**: If you are using the older (3.x.x) `aws-iot-device-sdk-embedded-C` release, please checkout the [`release/v3.1.x`](https://github.com/espressif/esp-aws-iot/tree/release/v3.1.x) branch.

This framework enables AWS IoT cloud connectivity with ESP32 based platforms using [AWS IoT Device Embedded SDK](https://github.com/aws/aws-iot-device-sdk-embedded-C).

## Getting Started

- Please clone this repository using,
    ```
    git clone --recursive https://github.com/espressif/esp-aws-iot
    ```
- Please refer to https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html for setting ESP-IDF
  - ESP-IDF can be downloaded from https://github.com/espressif/esp-idf/
  - Please set your branch to `release/v4.3` and pull in the latest changes.
- Please refer to [example README](examples/README.md) for more information on setting up examples
- This framework supports multiple ways to securely store the PKI credentials.
  - The default method is to use PKI credentials which are embedded in the binary, using the certs from the `certs/` in every example. 
  - For using Secure Element (ATECC608A), you will need to use [esp-cryptoauthlib](https://github.com/espressif/esp-cryptoauthlib#how-to-use-esp-cryptoauthlib-with-esp-idf).
  - For using Digial Signature Peripheral (Available on the ESP32-C3, ESP32-S2 and ESP32-S3), you will need to [provision it](https://github.com/espressif/esp-idf/tree/master/examples/protocols/mqtt/ssl_ds#configure_dspy) first.
