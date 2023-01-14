# ESP-AWS-IoT

> **IMPORTANT**: Please choose the branch of this repo, based on the [FreeRTOS-LTS release](https://github.com/FreeRTOS/FreeRTOS-LTS/releases) that you would like to base your application on.

If you are using the older (3.x.x) `aws-iot-device-sdk-embedded-C` release, please checkout the [`release/v3.1.x`](https://github.com/espressif/esp-aws-iot/tree/release/v3.1.x) branch.

This SDK enables AWS IoT cloud connectivity with ESP32-based platforms using the libraries provided as a part of the [AWS IoT Device Embedded C SDK](https://github.com/aws/aws-iot-device-sdk-embedded-C).

## Getting Started

- Please clone this branch of the repository using
    ```
    git clone -b "<name_of_the_release_branch>" --recursive https://github.com/espressif/esp-aws-iot
    ```
  For example: To clone just `release/202210.01-LTS`, you may run:
  ```
  git clone -b "release/202210.01-LTS" --recursive https://github.com/espressif/esp-aws-iot
  ```
- Please refer to https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html for setting up ESP-IDF
  - ESP-IDF can be downloaded from https://github.com/espressif/esp-idf/
  - This SDK supports `release/v5.0`, `release/v4.4` and `release/v4.3` of ESP-IDF.
  - Please set your branch to the IDF release that you would like to use, and pull in the latest changes.
  > **Note**: Please read [Support Policy](#support-policy) below for more details.
- Please refer to the [example README](examples/README.md) for more information on setting up examples.
- For a production-ready starting point for making applications that connect to AWS IoT Core using `esp-aws-iot`, refer to the [reference example](https://github.com/FreeRTOS/iot-reference-esp32c3).
- Please refer to the [Security Guide](examples/ota/SecurityGuide.md) for steps to enable Security Features on your Espressif chip. 
  - This SDK supports multiple ways to securely store the PKI credentials.
  - The default method is to use PKI credentials which are embedded in the binary, using the certs from the `certs/` in every example. 
  - For using Secure Element (ATECC608A), you will need to use [esp-cryptoauthlib](https://github.com/espressif/esp-cryptoauthlib#how-to-use-esp-cryptoauthlib-with-esp-idf).
  - The Security Guide includes detailed steps and pointers to configure and use the Digital Signature Peripheral on supported chips, using [esp_secure_cert_mgr](https://components.espressif.com/components/espressif/esp_secure_cert_mgr).

## Supported SoCs

The following table shows esp-aws-iot support with the ESP-IDF support of Espressif SoCs:  
![alt text][supported] and ![alt text][unsupported] denote supported and unsupported status, respectively.  

<table>
<thead>
  <tr align="center">
    <th rowspan="2">SoC</th>
    <th rowspan="2">Supported IDF Version</th>
    <th colspan="2">esp-aws-iot Release</th>
  </tr>
  <tr align="center">
    <th>202012.04-LTS</th>
    <th>202210.01-LTS</th>
  </tr>
</thead>
<tbody>
  <tr align="center">
    <td rowspan="3">ESP32</td>
    <td>IDF v4.3</td>
     <td><img src="https://img.shields.io/badge/-supported-green"/></a></td>
     <td><img src="https://img.shields.io/badge/-supported-green"/></a></td>
  </tr>
  <tr align="center">
    <td>IDF v4.4</td>
     <td><img src="https://img.shields.io/badge/-supported-green"/></a></td>
     <td><img src="https://img.shields.io/badge/-supported-green"/></a></td>
  </tr>
  <tr align="center">
    <td>IDF v5.0</td>
    <td></td>
     <td><img src="https://img.shields.io/badge/-supported-green"/></a></td>
  </tr>
  <tr align="center">
    <td rowspan="3">ESP32-S2</td>
    <td>IDF v4.3</td>
     <td><img src="https://img.shields.io/badge/-supported-green"/></a></td>
     <td><img src="https://img.shields.io/badge/-supported-green"/></a></td>
  </tr>
  <tr align="center">
    <td>IDF v4.4</td>
     <td><img src="https://img.shields.io/badge/-supported-green"/></a></td>
     <td><img src="https://img.shields.io/badge/-supported-green"/></a></td>
  </tr>
  <tr align="center">
    <td>IDF v5.0</td>
    <td></td>
     <td><img src="https://img.shields.io/badge/-supported-green"/></a></td>
  </tr>
  <tr align="center">
    <td rowspan="3">ESP32-C3</td>
    <td>IDF v4.3</td>
     <td><img src="https://img.shields.io/badge/-supported-green"/></a></td>
     <td><img src="https://img.shields.io/badge/-supported-green"/></a></td>
  </tr>
  <tr align="center">
    <td>IDF v4.4</td>
     <td><img src="https://img.shields.io/badge/-supported-green"/></a></td>
     <td><img src="https://img.shields.io/badge/-supported-green"/></a></td>
  </tr>
  <tr align="center">
    <td>IDF v5.0</td>
    <td></td>
     <td><img src="https://img.shields.io/badge/-supported-green"/></a></td>
  </tr>
  <tr align="center">
    <td rowspan="2">ESP32-S3</td>
    <td>IDF v4.4</td>
     <td><img src="https://img.shields.io/badge/-supported-green"/></a></td>
     <td><img src="https://img.shields.io/badge/-supported-green"/></a></td>
  </tr>
  <tr align="center">
    <td>IDF v5.0</td>
    <td></td>
     <td><img src="https://img.shields.io/badge/-supported-green"/></a></td>
  </tr>
  <tr align="center">
    <td>ESP32-C2</td>
    <td>IDF v5.0</td>
    <td></td>
     <td><img src="https://img.shields.io/badge/-supported-green"/></a></td>
  </tr>
</tbody>
</table>

[supported]: https://img.shields.io/badge/-supported-green "supported"
[unsupported]: https://img.shields.io/badge/-unsupported-red "unsupported"

## Support Policy

IDF version support for esp-aws-iot releases is based on [IDF Release Support Schedule](https://github.com/espressif/esp-idf#esp-idf-release-support-schedule).  
For example, support for IDF v4.3 for Release 202210.01-LTS will expire on 15th Dec 2023.

