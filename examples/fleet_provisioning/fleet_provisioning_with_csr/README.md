# Amazon Web Services IoT MQTT (Fleet Provisioning With CSR) Example

This is an adaptation of the [AWS IoT C SDK](https://github.com/aws/aws-iot-device-sdk-embedded-C) "fleet_provisioning_with_csr" example for ESP-IDF.

> **Note**: This example is currently in release preview but supports the full functionality of the Fleet Provisioning library.
> It uses a SPIFFS partition to store the claim credentials and the received certificate and private key are stored in the NVS partition but are not encrypted by default.

# Provisioning/Configuration

## Cloud Configuration
To create a provisioning template and claim credentials, sign into your AWS account and visit [here](https://console.aws.amazon.com/iot/home#/provisioningtemplate/create/instruction).  
Once you have created the template and credentials, modify the claim certificate's policy to match the [sample policy](examples/fleet_provisioning/fleet_provisioning_with_csr/example_claim_policy.json).


## Device Configuration

* Set your custom endpoint under `Example Configuration` in `idf.py menuconfig`.  
The endpoint can found on the *Settings* page of the AWS IoT Console and has a format of `ABCDEFG1234567.iot.us-east-2.amazonaws.com`.
* Replace the `fleet_provisioning_with_csr/spiffs_image/certs/claim_cert.crt` and `claim_private.key` files with your claim certificate and claim private key.
* Set the name of the provisioning template that you have created.
* Set the device serial number to an arbitrary string representing a device identifier.

# Troubleshooting

If you're having problems with the AWS IoT connection itself, check the Troubleshooting section of the README in the parent directory.
