# Steps to run OTA over HTTP Demo

1. Ensure that you have followed all the prerequisites for OTA Update from the [Amazon FreeRTOS Docs](https://docs.aws.amazon.com/freertos/latest/userguide/ota-prereqs.html).

2. Create a code signing certificate for ESP32, as shown [here](https://docs.aws.amazon.com/freertos/latest/userguide/ota-code-sign-cert-esp.html).  
Ensure that keep the `ecdsasigner.crt` file handy, it will be needed in subsequent steps.

3. Grant access to code signing, for AWS IoT, as shown [here](https://docs.aws.amazon.com/freertos/latest/userguide/code-sign-policy.html).

4. Now, to create an OTA update job, using the AWS IoT console, follow the steps mentioned [here](https://docs.aws.amazon.com/freertos/latest/userguide/ota-console-workflow.html).  
For **"Path name of code signing certificate on device"**, put the following value:
```
Code Verify Key
```  
This corresponds to `pkcs11configLABEL_CODE_VERIFICATION_KEY` in the `core_pkcs11_config.h` file.  

For **"Path name of file on device"**, put an arbitrary value such as:
```
/
```

5. Replace the contents of the file `main/certs/P11_CSK.crt` with the content in `ecdsasigner.crt`  
Run the following command to generate the NVS certificate image:  
```sh
python $IDF_PATH/components/nvs_flash/nvs_partition_generator/nvs_partition_gen.py generate ./ota_http_demo.csv ./ota_http_demo.bin 0x4000
```  
Run the following command to program the NVS certificate image:   
```sh
python $IDF_PATH/components/esptool_py/esptool/esptool.py -p <UART port> -b 921600 --before default_reset --after hard_reset write_flash --flash_mode dio --flash_size detect --flash_freq 40m 0x310000 ./ota_http_demo.bin
```

6. `idf.py menuconfig` and set MQTT endpoint.

7. `idf.py build flash monitor`