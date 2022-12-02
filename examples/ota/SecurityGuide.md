# Getting Started with Security Features

## 1 Pre-requisites

Once you have set up and run the OTA update examples successfully, you can follow the steps given in this guide to make use of the Security Features available on the Espressif chips for development and production scenarios.

> **Note**: The guide and linked references are specific to the ESP32-C3.  Please change the SoC in the documentation pointer to the one that you are using.

## 2 Securing Device Identity

Espressif provides `esp_secure_cert_mgr`, a simplified interface to access the PKI credentials of a device pre-provisioned with the Espressif Provisioning Service or provisioned by the user.  
It provides the set of APIs that are required to access the contents of the `esp_secure_cert` partition.

SoCs that are equipped with the Digital Signature Peripheral produce hardware accelerated digital signatures (with assistance of HMAC), without the private key being accessible by software.  
This allows the private key to be kept securely on the device without anyone other than the device hardware being able to access it.  
More details about the Digital Signature Peripheral can be found [here](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-reference/peripherals/ds.html). 

On SoCs without the digital signature peripheral (ESP32 and ESP32-C2), the private key must be protected by enabling secure boot and flash encryption after the `esp_secure_cert` partition is created and written.

More information on the `esp_secure_cert_mgr` and the corresponding example application can be found [here](https://github.com/espressif/esp_secure_cert_mgr/).

The tool to generate the `esp_secure_cert` partition and configure the digital signature peripheral on supported SoCs can be installed using:
```sh
pip install esp-secure-cert-tool
```

### Configuring esp-secure-cert-tool

1) Generate Root CA and key pair:
```sh
openssl req -newkey rsa:2048 -nodes -keyout prvtkey.pem \
-x509 -days 3650 -out cacert.pem -subj "/CN=Test CA"
```

2) Generate client private key:
```sh
openssl genrsa -out client.key
```

3) Generate device cert:
```sh
openssl req -out client.csr -key client.key -new

openssl x509 -req -days 365 -in client.csr -CA cacert.pem \
-CAkey prvtkey.pem  -sha256 -CAcreateserial -out client.crt
```

In the next section, the script will generate a partition named `esp_secure_cert` on host machine, that contains the parameters required by the DS peripheral.  
It automatically flashes the partition to the ESP platform which has been connected and specified by the port.

The examples in this repository already include the APIs required to utilise the `esp_secure_cert` partition and fetch the `esp_secure_cert_mgr` component using [IDF Component Registry](https://components.espressif.com/components/espressif/esp_secure_cert_mgr) at build-time.  
It can be enabled for any example for SoCs that have the digital signature peripheral by following the following steps, after the `esp_secure_cert` partition has been flashed:
   1. Enter the menuconfig
   ```sh
   idf.py menuconfig
   ```
   2. Go into the `Example Configuration` section.
   3. Go into `Choose PKI credentials access method`
   4. Choose `Use ESP Secure Certificate Manager`
   5. Go into `(Top)` -> `Component config` -> `ESP Secure Cert Manager`
   6. Choose `Enable DS peripheral support` 

### Securing Device Identity Workflows
<details>
   <summary>Expand Development Workflow</summary>

   ###
   In the development workflow, read protection for the eFuse key block in which the DS key is programmed is not enabled when configuring the DS peripheral.  
   This is done to allow flexibility during development.

   The following command shall generate a binary partition containing the PKI credentials stored in the TLV format:
   ```sh
   configure_esp_secure_cert.py -p (PORT) --keep_ds_data_on_host --efuse_key_id 1 \
   --ca-cert cacert.pem --device-cert client.crt --private-key client.key \
   --target_chip (TARGET_CHIP) --secure_cert_type cust_flash_tlv --configure_ds
   ```
   If the DS Peripheral is disabled in the menuconfig, or if you are using this command for a non-DS SoC, please remove the `--configure_ds` argument from the command.
</details>

<details>
   <summary>Expand Production Workflow</summary>

   ###
   It is recommended to use modules pre-provisioned with the Espressif Provisioning Service for production scenario, because use of the following workflow for production can still lead to security compromised devices, if the private key (which is available in plaintext on the host) gets compromised.  
   In the production workflow, the read protection for the eFuse key block in which the DS key is programmed is enabled.
   ```sh
   configure_esp_secure_cert.py -p (PORT) --keep_ds_data_on_host --efuse_key_id 1 \
   --ca-cert cacert.pem --device-cert client.crt --private-key client.key \
   --target_chip (TARGET_CHIP) --secure_cert_type cust_flash_tlv --configure_ds --production
   ```
   If the DS Peripheral is disabled in the menuconfig, or if you are using this command for a non-DS SoC, please remove the `--configure_ds` argument from the command.
   > **IMPORTANT**: For non-DS SoCs, you will need to enable flash encryption and secure boot, to protect the private key.
   > The subsequent sections describe how this can be accomplished.
</details>


## 3 Enabling Flash Encryption

Flash encryption is intended for encrypting the contents of the device's off-chip flash memory and thus provides the "confidentiality" aspect to the software or data stored in the flash memory.  
Once this feature is enabled, firmware is flashed as plaintext, and then the data is encrypted in place on the first boot.  
As a result, physical readout of flash will not be sufficient to recover most flash contents.

More details on Flash Encryption for your SoC can be found [here](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/security/flash-encryption.html).

### Flash Encryption Workflows
<details>
   <summary>Expand Development Workflow</summary>

   ###
   1. Enter the menuconfig
   ```sh
   idf.py menuconfig
   ```
   2. Go into the `Security features` section and select `Enable flash encryption on boot`.
   3. Ensure that the usage mode is set to `Development (NOT SECURE)` (it will be selected by default)
   > In the next step upon resetting, the partitions marked `encrypted`, partitions of "app" type, the firmware bootloader and the partition table will be encrypted.
   4. Rebuild, re-flash and reset the board (Can be accomplished in one go by doing `idf.py build flash monitor`)
   5. Watch the serial console output to verify that Flash Encryption is enabled, and no errors have occurred.

</details>

<details>
   <summary>Expand Production Workflow</summary>

   ###
   1. Enter the menuconfig
   ```sh
   idf.py menuconfig
   ```
   2. Go into the `Security features` section and select `Enable flash encryption on boot`.
   3. Change usage mode to `Release`.  
   Setting the usage mode to `Release` ensures that all the eFuse bits that are recommended for production are set correctly.
   > In the next step upon resetting, the partitions marked `encrypted`, partitions of "app" type, the firmware bootloader and the partition table will be encrypted.
   4. Rebuild, re-flash and reset the board (Can be accomplished in one go by doing `idf.py build flash monitor`)
   5. Watch the serial console output to verify that Flash Encryption is enabled, and no errors have occurred.
   
   More details on `Release mode` for your SoC can be found [here](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/security/flash-encryption.html#release-mode).
</details>


## 4 Enabling Secure Boot

The Secure Boot feature ensures that only authenticated software can execute on the device. The secure boot process forms chain of trust by verifying all mutable software entities involved in the ESP-IDF boot process. Signature verification happens during both boot-up as well as OTA updates.

There is no separate `Development Workflow` for Secure Boot.

More details on Secure Boot can be found [here](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/security/secure-boot-v2.html).

### Secure Boot Production Workflow
<details open>
   <summary>Expand Production Workflow</summary>

   ###
   1. Open the Project Configuration Menu, in "Security features" set "Enable hardware Secure Boot in bootloader" to enable Secure Boot.
   2. The "Secure Boot V2" option will be selected, and the "App Signing Scheme" would be set to RSA by default.
   3. Specify the path to Secure Boot signing key, relative to the project directory.
   4. The first time you run `idf.py build`, if the signing key is not found then an error message will be printed with a command to generate a signing key via `espsecure.py generate_signing_key`.  
   For production environments, it is recommended to generate the key pair using OpenSSL or another industry standard encryption program.  
   See the [Generating Secure Boot Signing Key](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/security/secure-boot-v2.html#secure-boot-v2-generate-key) page for your SoC for more details.
   5. Run `idf.py bootloader` to build a Secure Boot enabled bootloader.
   The build output will include a prompt for a flashing command, using `esptool.py write_flash`.
   6. When you are ready to flash the bootloader, run the specified command (you must enter it yourself, this step is not performed by the build system) and then wait for flashing to complete.
   7. Run `idf.py flash` to build and flash the partition table and the just-built app image.  
   The app image will be signed using the signing key you generated in step 4.
   8. Reset the device, and it will boot the software bootloader you flashed which will enable Secure Boot on the chip, and then it verifies the app image signature and boots the app.
   9. Watch the serial console output to verify that Secure Boot is enabled, and no errors have occurred due to the build configuration.
</details>

> **Note**: Since the AWS OTA library implements its own signature verification of the OTA payload (based on ECDSA), there is a double  signature verification for the application that takes place during OTA update.
> After the OTA payload has been successfully downloaded, first the ECDSA signature (supplied by the `certs/aws_codesign.crt` file) is verified and then the Secure Boot V2 RSA signature gets verified.
