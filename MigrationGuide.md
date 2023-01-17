# Migration Guide for esp-aws-iot Releases

## esp-aws-iot Release 202012.04-LTS to esp-aws-iot Release 202210.01-LTS

1. Change in `NetworkContext` struct

- "Pem" has been removed from member names, the new member names are as follows: `pcServerRootCA`, `pcClientCert`, `pcClientKey`.
- `pcServerRootCASize`, `pcClientCertSize`, `pcClientKeySize` members have been added.
- It is now required to set these `*Size` members in addition to the pointers to the certificates.

**Example:**

**Old Code Snippet**:
```c
pNetworkContext->pcClientCertPem = client_cert_pem_start;
```

**New Code Snippet**:
```c
pNetworkContext->pcClientCert = client_cert_start;
pNetworkContext->pcClientCertSize = client_cert_end - client_cert_start;
```

2. This releases includes a version >=v2.0.0 of the `coreMQTT` library.  
Please refer to the [coreMQTT Migration Guide](https://github.com/FreeRTOS/coreMQTT/blob/main/MigrationGuide.md) for more details.

3. The `*_USE_DS_PERIPHERAL` and `*_USE_SECURE_ELEMENT` config options have been removed and no longer need to be set while using `coreMQTT` and `coreHTTP` libraries.
