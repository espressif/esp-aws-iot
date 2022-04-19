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

5. Replace the contents of the file `main/certs/aws_codesign.crt` with the content from the file `ecdsasigner.crt` 

> The code-signing public key certificate will be used by the application binary i.e. the demo, to authenticate a binary that was downloaded for an update (this downloaded firmware would have been signed by the certificate's corresponding private key i.e. `ecdsasigner.key`). 

6. `idf.py menuconfig` and set MQTT endpoint.

7. `idf.py build flash monitor`