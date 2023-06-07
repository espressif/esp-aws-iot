#!/usr/bin/bash
set -x

PORT="/dev/cu.usbmodem21201"

espefuse.py --port $PORT summary --format json --file efuses.json
MAC=$(jq '.MAC.value' efuses.json)
MAC=${MAC%(*}
MAC=${MAC//\"/}
MAC=${MAC//\ /}
MAC=${MAC//:/-}
rm -rf efuses.json

#idf.py clean
#idf.py fullclean 
rm -rf sdkconfig
rm -rf build
#idf.py set-target esp32c3

dev_id="cloudio-$MAC"

cp -rf sdkconfig2 sdkconfig

find . -type f -name 'sdkconfig' -exec sed -i '' s/"testClient"/$dev_id/g {} +

thing_type='prototype'
thing_group_name='cloudio_v1'
product_prj='cloudio'
policy_name='cloudio_v1_policy'
payload_shadow_name='payload-prod'
error_shadow_name='error-logs'

mkdir -p tmp/cert/$dev_id/AWS/
#erase_shadow = '1'

#cd ./fwcerts

thingexists=$(aws iot describe-thing --thing-name "$dev_id" | jq -r '.thingName')
if [ "$thingexists" = "$dev_id" ]; then
    read -p "Thing already exists on AWS press ctrl+c to exit, delete the aws iot thing and run the script again. Press any key to continue... " -n1 -s
    echo''; echo "\nDeleting AWS IoT Thing shadow"
     aws iot-data delete-thing-shadow \
    --thing-name  $dev_id \
    /dev/stdout
fi
echo''

echo''; echo "\nDeleting AWS IoT Thing payload shadow"
aws iot-data delete-thing-shadow \
    --thing-name  $dev_id \
    --shadow-name $payload_shadow_name

echo''; echo "\nDeleting AWS IoT Thing error shadow"
aws iot-data delete-thing-shadow \
    --thing-name  $dev_id \
    --shadow-name $payload_shadow_name

echo "\nCreate AWS IoT Thing"
# Create AWS IoT Thing
aws iot create-thing \
    --thing-name $dev_id \
    --thing-type-name $thing_type

#add to the group
echo''
echo "\nADD Thing to a group"
aws iot add-thing-to-thing-group \
    --thing-name $dev_id \
    --thing-group-name $thing_group_name

# Create keys and certificate
echo''
echo "\nCreate certificates"
arn=$(aws iot create-keys-and-certificate \
    --set-as-active \
    --certificate-pem-outfile tmp/cert/$dev_id/AWS/certificate.pem.crt \
    --public-key-outfile tmp/cert/$dev_id/AWS/public.pem.key \
    --private-key-outfile tmp/cert/$dev_id/AWS/private.pem.key |
    jq -r '.certificateArn')

# Attach the right policy
echo''
echo "\nAttach policies"
aws iot attach-policy \
    --target "$arn" \
    --policy-name "$policy_name" 

# Attach ARN
echo''
echo "\nAttach principal"
aws iot attach-thing-principal \
    --principal "$arn" \
    --thing-name "$dev_id"

# reinit payload shadow
echo''
echo "\nUpdate thing shadow"
aws iot-data update-thing-shadow \
    --thing-name "$dev_id" \
    --shadow-name $payload_shadow_name \
    --cli-binary-format raw-in-base64-out \
    --payload \
    '{
        "state": {
            "desired": null
        }
    }' \
    /dev/stdout

aws iot-data update-thing-shadow \
    --thing-name "$dev_id" \
    --shadow-name $payload_shadow_name \
    --cli-binary-format raw-in-base64-out \
    --payload \
    '{
        "state": {
            "reported": null
        }
    }' \
    /dev/stdout
    printf "\n***\nPayload shadow reinitialized for $dev_id \n\n"


# reinit error shadow
echo''
echo "\nUpdate thing shadow"
aws iot-data update-thing-shadow \
    --thing-name "$dev_id" \
    --shadow-name $error_shadow_name \
    --cli-binary-format raw-in-base64-out \
    --payload \
    '{
        "state": {
            "desired": null
        }
    }' \
    /dev/stdout

aws iot-data update-thing-shadow \
    --thing-name "$dev_id" \
    --shadow-name $error_shadow_name \
    --cli-binary-format raw-in-base64-out \
    --payload \
    '{
        "state": {
            "reported": null
        }
    }' \
    /dev/stdout
    printf "\n***\nError shadow reinitialized for $dev_id \n\n"

# Update device shadow
echo''
echo "\nUpdate thing shadow"
aws iot-data update-thing-shadow \
    --thing-name "$dev_id" \
    --cli-binary-format raw-in-base64-out \
    --payload \
    '{
        "state": {
            "desired": {
            "cfg": {
                "version":"v1.0.1",
                "publish_interval": 60,
                "facility": "testing",
                "led": 1,
                "dbg_mode": 1,
                "reset": 0
            }
            }
        }
    }' \
    /dev/stdout

    printf "\n***\nThing shadow created for $dev_id with default configuration\n\n"

#mkdir -p fwcerts
cp -rf tmp/cert/$dev_id/AWS/certificate.pem.crt main/certs/client.crt
cp -rf tmp/cert/$dev_id/AWS/private.pem.key main/certs/client.key



## AWS provision


idf.py --version 
#idf.py -p $PORT erase-otadata

esptool.py -p $PORT --chip esp32c3 erase_flash
#parttool.py -p $PORT erase_partition --partition-name=nvs

#idf.py clean
#python3 components/esp_secure_cert_mgr/tools/configure_esp_secure_cert.py -p $PORT --keep_ds_data_on_host --ca-cert main/certs/root_cert_auth.crt --device-cert main/certs/certificate.pem.crt --private-key main/certs/private.pem.key --target_chip esp32c3 --secure_cert_type cust_flash
#esptool.py --no-stub --port $PORT write_flash 0xD000 esp_secure_cert_data/esp_secure_cert.bin
# PORT="/dev/cu.usbmodem11101"
idf.py -p $PORT build flash monitor