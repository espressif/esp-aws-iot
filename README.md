# ESP-AWS-IoT

> **IMPORTANT**: Please choose the branch of this repo, based on the [FreeRTOS-LTS release](https://github.com/FreeRTOS/FreeRTOS-LTS/releases) that you would like to base your application on.

If you are using the older (3.x.x) `aws-iot-device-sdk-embedded-C` release, please checkout the [`release/v3.1.x`](https://github.com/espressif/esp-aws-iot/tree/release/v3.1.x) branch.

This SDK enables AWS IoT cloud connectivity with ESP32-based platforms using the libraries provided as a part of the [AWS IoT Device Embedded C SDK](https://github.com/aws/aws-iot-device-sdk-embedded-C).

## Getting Started

- Please clone this branch of the repository using
    ```
    git clone -b "<name_of_the_release_branch>" --recursive https://github.com/espressif/esp-aws-iot
    ```
  For example: To clone just `release/202406.05-LTS`, you may run:
  ```
  git clone -b "release/202406.05-LTS" --recursive https://github.com/espressif/esp-aws-iot
  ```
- Please refer to [https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) for setting up ESP-IDF
  - ESP-IDF can be downloaded from [https://github.com/espressif/esp-idf/](https://github.com/espressif/esp-idf/)
  - This SDK supports `release/v6.0`, `release/v5.5`, `release/v5.4`, `release/v5.3` and `release/v5.2` of ESP-IDF.
  - The `corePKCS11` library is **not** supported on `release/v6.0` as support for [mbedTLS v4.x](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/migration-guides/release-6.x/6.0/security.html#mbed-tls-v4-0-migration) is [not available](https://github.com/FreeRTOS/corePKCS11/issues/213) in `corePKCS11` yet.  
  Consequently, the `fleet_provisioning_with_csr` example, which depends on corePKCS11 is incompatible with `release/v6.0`.
  - Please set your branch to the IDF release that you would like to use, and pull in the latest changes.
    > **Note**: Please read [Support Policy](#support-policy) below for more details.
- Please refer to the [example README](examples/README.md) for more information on setting up examples.
- For a production-ready starting point for making applications that connect to AWS IoT Core using `esp-aws-iot`, refer to the [reference example](https://github.com/FreeRTOS/iot-reference-esp32).
- Please refer to the [Security Guide](https://github.com/espressif/esp-aws-iot/blob/release/202210.01-LTS/examples/ota/SecurityGuide.md) for steps to enable Security Features on your Espressif chip.
  - This SDK supports multiple ways to securely store the PKI credentials.
  - The default method is to use PKI credentials which are embedded in the binary, using the certs from the `certs/` in every example. 
  - ATECC608A credentials use the legacy mbedTLS ALT integration on ESP-IDF v5.x and the unified key interface with a PSA opaque-key driver on supported ESP-IDF v6.x checkouts.
  - The Security Guide includes detailed steps and pointers to configure and use the Digital Signature Peripheral on supported chips, using [esp_secure_cert_mgr](https://components.espressif.com/components/espressif/esp_secure_cert_mgr).

## Supported SoCs

The following table shows esp-aws-iot support with the ESP-IDF support of Espressif SoCs:  
Cell marked as ![supported][supported] denotes supported combination.  
Cell marked as ![expired][expired] denotes expired combination.  
Cell marked as `-` denotes a release/IDF combination that was not available in that release.

<table>
    <thead>
        <tr align="center">
            <th rowspan="2">SoC</th>
            <th rowspan="2">Supported IDF Version</th>
            <th colspan="3">esp-aws-iot Release</th>
        </tr>
        <tr align="center">
            <th><a href="https://github.com/espressif/esp-aws-iot/tree/master">master</a></th>
            <th><a href="https://github.com/espressif/esp-aws-iot/tree/release/202406.05-LTS">202406.05-LTS</a></th>
            <th><a href="https://github.com/espressif/esp-aws-iot/tree/release/202406.03-LTS">202406.03-LTS</a><br/><a href="https://github.com/espressif/esp-aws-iot/tree/release/202406.01-LTS">202406.01-LTS</a></th>
        </tr>
    </thead>
    <tbody>
        <tr align="center">
            <td rowspan="6">ESP32</td>
            <td>IDF v5.1</td>
            <td><img src="https://img.shields.io/badge/-expired-grey" alt="expired" width="50" height="20"></td>
            <td><img src="https://img.shields.io/badge/-expired-grey" alt="expired" width="50" height="20"></td>
            <td><img src="https://img.shields.io/badge/-expired-grey" alt="expired" width="50" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.2</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.3</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.4</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.5</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v6.0</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td>-</td>
        </tr>
        <tr align="center">
            <td rowspan="6">ESP32-S2</td>
            <td>IDF v5.1</td>
            <td><img src="https://img.shields.io/badge/-expired-grey" alt="expired" width="50" height="20"></td>
            <td><img src="https://img.shields.io/badge/-expired-grey" alt="expired" width="50" height="20"></td>
            <td><img src="https://img.shields.io/badge/-expired-grey" alt="expired" width="50" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.2</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.3</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.4</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.5</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v6.0</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td>-</td>
        </tr>
        <tr align="center">
            <td rowspan="6">ESP32-C3</td>
            <td>IDF v5.1</td>
            <td><img src="https://img.shields.io/badge/-expired-grey" alt="expired" width="50" height="20"></td>
            <td><img src="https://img.shields.io/badge/-expired-grey" alt="expired" width="50" height="20"></td>
            <td><img src="https://img.shields.io/badge/-expired-grey" alt="expired" width="50" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.2</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.3</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.4</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.5</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v6.0</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td>-</td>
        </tr>
        <tr align="center">
            <td rowspan="6">ESP32-S3</td>
            <td>IDF v5.1</td>
            <td><img src="https://img.shields.io/badge/-expired-grey" alt="expired" width="50" height="20"></td>
            <td><img src="https://img.shields.io/badge/-expired-grey" alt="expired" width="50" height="20"></td>
            <td><img src="https://img.shields.io/badge/-expired-grey" alt="expired" width="50" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.2</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.3</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.4</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.5</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v6.0</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td>-</td>
        </tr>
        <tr align="center">
            <td rowspan="6">ESP32-C2</td>
            <td>IDF v5.1</td>
            <td><img src="https://img.shields.io/badge/-expired-grey" alt="expired" width="50" height="20"></td>
            <td><img src="https://img.shields.io/badge/-expired-grey" alt="expired" width="50" height="20"></td>
            <td><img src="https://img.shields.io/badge/-expired-grey" alt="expired" width="50" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.2</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.3</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.4</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.5</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v6.0</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td>-</td>
        </tr>
        <tr align="center">
            <td rowspan="6">ESP32-C6</td>
            <td>IDF v5.1</td>
            <td><img src="https://img.shields.io/badge/-expired-grey" alt="expired" width="50" height="20"></td>
            <td><img src="https://img.shields.io/badge/-expired-grey" alt="expired" width="50" height="20"></td>
            <td><img src="https://img.shields.io/badge/-expired-grey" alt="expired" width="50" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.2</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.3</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.4</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v5.5</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v6.0</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td>-</td>
        </tr>
        <tr align="center">
            <td rowspan="2">ESP32-C5</td>
            <td>IDF v5.5</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
        </tr>
        <tr align="center">
            <td>IDF v6.0</td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td><img src="https://img.shields.io/badge/-supported-green" alt="supported" width="65" height="20"></td>
            <td>-</td>
        </tr>
    </tbody>
</table>

[supported]: https://img.shields.io/badge/-supported-green "supported"
[expired]: https://img.shields.io/badge/-expired-grey "expired"

## Support Policy


Support for `esp-aws-iot` release branches is based on a combination of the [IDF Release Support Schedule](https://github.com/espressif/esp-idf#esp-idf-release-support-schedule) and the FreeRTOS Long Term Support [Maintenance Period](https://github.com/FreeRTOS/FreeRTOS-LTS#freertosfreertos-long-term-support).


**Examples:**

1)  IDF v5.2 in esp-aws-iot 202406.05-LTS → expires 16 August 2026, when IDF v5.2 reaches End-of-Life.

2) IDF v5.1 in esp-aws-iot 202406.03-LTS → expired December 2025, when IDF v5.1 [reached End-of-Life](https://documentation.espressif.com/AR2025-007_End-of-Life_Advisory_for_ESP-IDF_v5.1_Release_Branch_EN.pdf).

3) Complete support for esp-aws-iot 202210.01-LTS → expired 31st October 2024, when FreeRTOS-LTS 202210.01-LTS [reached End-of-Life](https://github.com/FreeRTOS/FreeRTOS-LTS/tree/202210.01-LTS#freertosfreertos-long-term-support).
