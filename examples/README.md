# Amazon Web Services IoT Examples

These examples are adaptations of some of the [AWS IoT C SDK](https://github.com/aws/aws-iot-device-sdk-embedded-C) examples.

The provisioning/configuration steps for these examples are the same, and are given in this README.

This README also contains some troubleshooting information for common problems found when connecting to AWS IoT.

# Provisioning/Configuration

There are some additional steps that need to be run before you can build this example.

The [Getting Started section of the AWS IoT Developer Guide](http://docs.aws.amazon.com/iot/latest/developerguide/iot-gs.html) lays out the steps to get started with AWS IoT.

To build and use this example, follow all the AWS IoT Getting Started steps from the beginning ("Sign in to the AWS Iot Console") up until "Configuring Your Device". For configuring the device, these are the steps:

# Authentication (Based on X.509 certificates)

### Device Authentication

AWS IoT can use AWS IoT-generated certificates or certificates signed by a CA certificate for device authentication. To use a certificate that is not created by AWS IoT, you must register a CA certificate. All device certificates must be signed by the CA certificate you register. Please refer to guide at https://docs.aws.amazon.com/iot/latest/developerguide/device-certs-your-own.html for step-by-step instructions to register custom X.509 certificates.

### Server Authentication

Server certificates allow devices to verify that they're communicating with AWS IoT and not another server impersonating AWS IoT. By default [Amazon Root CA 1](https://www.amazontrust.com/repository/AmazonRootCA1.pem) (signed by Amazon Trust Services Endpoints CA) is embedded in applications, for more information please refer to https://docs.aws.amazon.com/iot/latest/developerguide/managing-device-certs.html#server-authentication

## Configuring Your Device

### Installing Private Key & Certificate

As part of creating a device certificate, you downloaded a Private Key (`xxx-private.pem.key`) and a Certificate file (`xxx-certificate.pem.crt`). These keys need to be loaded by the ESP32 to identify itself.

There is currently only one option for how to load the key & cert.

* Embed the files into the app binary (default)

### Embedded Key & Cert into App Binary

Copy the `.pem.key` and `.pem.crt` files to the `main/certs` subdirectory of the example. Rename them by removing the device-specific prefix - the new names are `client.key` and `client.crt`.

As these files are bound to your AWS IoT account, take care not to accidentally commit them to public source control. In a commercial IoT device these files would be flashed to the device via a provisioning step, but for these examples they are compiled in.

# Troubleshooting

## Tips

* Raise the ESP debug log level to Debug in order to see messages about the connection to AWS, certificate contents, etc.

* Enable mbedTLS debugging (under Components -> mbedTLS -> mbedTLS Debug) in order to see even more low-level debug output from the mbedTLS layer.

* To create a successful AWS IoT connection, the following factors must all be present:
  - Endpoint hostname is correct for your AWS account.
  - Certificate & private key are both attached to correct Thing in AWS IoT Console.
  - Certificate is activated.
  - Policy is attached to the Certificate in AWS IoT Console.
  - Policy contains sufficient permissions to authorize AWS IoT connection.

## TLS connection fails

If connecting fails entirely (handshake doesn't complete), this usually indicates a problem with certification configuration. The error usually looks like this:

```
failed! mbedtls_ssl_handshake returned -0x7780
```

(0x7780 is the mbedTLS error code when the server sends an alert message and closes the connection.)

* Check your client private key and certificate file match a Certificate registered and **activated** in AWS IoT console. You can find the Certificate in IoT Console in one of two ways, via the Thing or via Certificates:
  - To find the Certificate directly, click on "Registry" -> "Security Certificates". Then click on the Certificate itself to view it.
  - To find the Certificate via the Thing, click on "Registry" -> "Things", then click on the particular Thing you are using. Click "Certificates" in the sidebar to view all Certificates attached to that Thing. Then click on the Certificate itself to view it.

Verify the Certificate is activated (when viewing the Certificate, it will say "ACTIVE" or "INACTIVE" near the top under the certificate name).

If the Certificate appears correct and activated, verify that you are connecting to the correct AWS IoT endpoint (see above.)

## TLS connection closes immediately

Sometimes connecting is successful (the handshake completes) but as soon as the client sends its `MQTT CONNECT` message the server sends back a TLS alert and closes the connection, without anything else happening.

The error returned from AWS IoT is usually -28 (`MQTT_REQUEST_TIMEOUT_ERROR`). You may also see mbedtls error `-0x7780` (server alert), although if this error comes during `mbedtls_ssl_handshake` then it's usually a different problem (see above).

In the subscribe_publish example, the error may look like this in the log:

```
MQTT_DEMO: Error(-28) connecting to (endpoint)...
```

In the thing_shadow example, the error may look like this in the log:

```
ShadowDemo: aws_iot_shadow_connect returned error -28, aborting...
```

This error implies the Certificate is recognised, but the Certificate is either missing the correct Thing or the correct Policy attached to it.

* Check in the AWS IoT console that your certificate is activated and has both a **security policy** and a **Thing** attached to it. You can find this in IoT Console by clicking "Registry" -> "Security Certificates", then click the Certificate. Once viewing the Certificate, you can click the "Policies" and "Things" links in the sidebar.
