# ESP-AWS-IoT

> **IMPORTANT**: Please choose the branch of this repo, based on the [FreeRTOS-LTS release](https://github.com/FreeRTOS/FreeRTOS-LTS/releases) that you would like to base your application on.

If you are using the older (3.x.x) `aws-iot-device-sdk-embedded-C` release, please checkout the [`release/v3.1.x`](https://github.com/espressif/esp-aws-iot/tree/release/v3.1.x) branch.

This framework enables AWS IoT cloud connectivity with ESP32 based platforms using [AWS IoT Device Embedded C SDK](https://github.com/aws/aws-iot-device-sdk-embedded-C).

## Getting Started

- Please clone this branch of the repository using
    ```
    git clone -b "<name_of_the_release_branch>" --recursive https://github.com/espressif/esp-aws-iot
    ```
  For example: To clone just `release/202012.04-LTS`, you may run:
  ```
  git clone -b "release/202012.04-LTS" --recursive https://github.com/espressif/esp-aws-iot
  ```
- Please refer to https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html for setting up ESP-IDF
  - ESP-IDF can be downloaded from https://github.com/espressif/esp-idf/
  - Please set your branch to `release/v4.3` or `release/v4.4` and pull in the latest changes.
  - IDF `release/v5.0` is not supported.
- Please refer to [example README](examples/README.md) for more information on setting up examples.
- For a production-ready starting point for making applications that connect to AWS IoT Core using esp-aws-iot, refer to the [reference example](https://github.com/FreeRTOS/iot-reference-esp32c3).
- This framework supports multiple ways to securely store the PKI credentials.
  - The default method is to use PKI credentials which are embedded in the binary, using the certs from the `certs/` in every example. 
  - For using Secure Element (ATECC608A), you will need to use [esp-cryptoauthlib](https://github.com/espressif/esp-cryptoauthlib#how-to-use-esp-cryptoauthlib-with-esp-idf).
  - For using Digial Signature Peripheral (Available on the ESP32-C3, ESP32-S2 and ESP32-S3), you will need to use [esp_secure_cert_mgr](https://github.com/espressif/esp_secure_cert_mgr).
