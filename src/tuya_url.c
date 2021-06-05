#include <stdint.h>
#include "tuya_url.h"
#include "tuya_log.h"
#include "tuya_error_code.h"
#include "storage_interface.h"

static const uint8_t default_tuya_cacert[] = {\
"-----BEGIN CERTIFICATE-----\r\n"\
"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\r\n"\
"-----END CERTIFICATE-----\r\n"};

extern int iotdns_cloud_endpoint_get(const char* region, const char* env, tuya_endpoint_t* endport);

typedef struct {
    char regist[MAX_LENGTH_REGIST + 1];
    const tuya_endpoint_t* endpoint;
    int endpoint_num;
} tuya_cloud_environment_t;

typedef struct {
    char region[MAX_LENGTH_REGION + 1];
    char regist_key[MAX_LENGTH_REGIST + 1];
    tuya_endpoint_t endpoint;
} endpoint_management_t;


const tuya_endpoint_t default_endpint_pro[] = {
    {.region = "AY", .atop = {"a2.tuyacn.com", 443}, .mqtt = {"m2.tuyacn.com", 8883}},
    {.region = "AZ", .atop = {"a2.tuyaus.com", 443}, .mqtt = {"m2.tuyaus.com", 8883}},
    {.region = "EU", .atop = {"a2.tuyaeu.com", 443}, .mqtt = {"m2.tuyaeu.com", 8883}},
    {.region = "IN", .atop = {"a2.tuyain.com", 443}, .mqtt = {"m2.tuyain.com", 8883}},
    {.region = "UE", .atop = {"a2-ueaz.tuyaus.com", 443}, .mqtt = {"m2-ueaz.tuyaus.com", 8883}},
};

const tuya_endpoint_t default_endpint_pr_0[] = {
    {.region = "AY", .atop = {"a2-cn.wgine.com", 443}, .mqtt = {"m2-cn.wgine.com", 8883}},
    {.region = "AZ", .atop = {"a2-us.wgine.com", 443}, .mqtt = {"m2-us.wgine.com", 8883}},
    {.region = "EU", .atop = {"a2-eu.wgine.com", 443}, .mqtt = {"m2-eu.wgine.com", 8883}},
    {.region = "IN", .atop = {"a2-ind.wgine.com", 443}, .mqtt = {"m2-ind.wgine.com", 8883}},
    {.region = "UE", .atop = {"a2-ueaz.wgine.com", 443}, .mqtt = {"m2-ueaz.wgine.com", 8883}},
};

const tuya_cloud_environment_t default_env_list[] = {
    {.regist = "pro",  .endpoint = default_endpint_pro,  .endpoint_num = sizeof(default_endpint_pro)/sizeof(tuya_endpoint_t)},
    {.regist = "pr_0", .endpoint = default_endpint_pr_0, .endpoint_num = sizeof(default_endpint_pr_0)/sizeof(tuya_endpoint_t)},
};
 
static endpoint_management_t endpoint_mgr;


static int tuya_region_regist_key_write( const char* region, const char* regist_key )
{
    if ( NULL == region || NULL == regist_key ) {
        TY_LOGE( "Invalid param" );
        return OPRT_INVALID_PARM;
    }

    /* Write kv storage */
    local_storage_set("binding.region", region, strlen(region) + 1);
    local_storage_set("binding.regist_key", regist_key, strlen(regist_key) + 1);

    return OPRT_OK;
}

static int tuya_region_regist_key_read( char* region, char* regist_key )
{
    if ( NULL == region || NULL == regist_key ) {
        TY_LOGE( "Invalid param" );
        return OPRT_INVALID_PARM;
    }

    /* Read the region&env from kv storage */
    size_t len = 0;
    len = MAX_LENGTH_REGION;
    if (local_storage_get("binding.region", region, &len) != OPRT_OK) {
        return OPRT_KVS_RD_FAIL;
    }
    
    len = MAX_LENGTH_REGIST;
    if (local_storage_get("binding.regist_key", regist_key, &len) != OPRT_OK) {
        return OPRT_KVS_RD_FAIL;
    }

    return OPRT_OK;
}

int tuya_region_regist_key_remove()
{
    local_storage_del("binding.region");
    local_storage_del("binding.regist_key");
    return OPRT_OK;
}

int tuya_region_regist_key_set( const char* region, const char* regist_key )
{
    if (tuya_region_regist_key_write( region, regist_key ) != OPRT_OK) {
        return OPRT_KVS_WR_FAIL;
    }
    return tuya_region_regist_key_load();
}

static int default_endpoint_get( const char* region, const char* regist_key, tuya_endpoint_t* endpoint )
{
    int i;

    /* find the defalut regist key */
    tuya_cloud_environment_t* env = (tuya_cloud_environment_t*)&default_env_list[0]; // defalut
    for (i = 0; i < sizeof(default_env_list)/sizeof(tuya_cloud_environment_t); i++) {
        if (memcmp(regist_key, default_env_list[i].regist, strlen(regist_key)) == 0) {
            env = (tuya_cloud_environment_t*)&default_env_list[i];
            TY_LOGI("Environment:%s", default_env_list[i].regist);
            break;
        }
    }
    
    /* find the default region */
    *endpoint = env->endpoint[0]; // defalut
    for (i = 0; i < env->endpoint_num; i++) {
        if (memcmp(region, env->endpoint[i].region, strlen(region)) == 0) {
            *endpoint = env->endpoint[i];
            TY_LOGI("Host region:%s", env->endpoint[i].region);
            break;
        }
    }

    /* default CA cert*/
    endpoint->atop.cert = (uint8_t*)default_tuya_cacert;
    endpoint->atop.cert_len = sizeof(default_tuya_cacert);
    endpoint->mqtt.cert = (uint8_t*)default_tuya_cacert;
    endpoint->mqtt.cert_len = sizeof(default_tuya_cacert);
    return OPRT_OK;
}

int tuya_region_regist_key_load()
{
    int ret = tuya_region_regist_key_read(endpoint_mgr.region, endpoint_mgr.regist_key);
    
    ret = iotdns_cloud_endpoint_get(endpoint_mgr.region, 
                                    endpoint_mgr.regist_key, 
                                    &endpoint_mgr.endpoint);
    if (ret == OPRT_OK) {
        return OPRT_OK;
    }

    /* Load default domain */
    return default_endpoint_get((const char*)endpoint_mgr.region, 
                                (const char*)endpoint_mgr.regist_key, 
                                &endpoint_mgr.endpoint);
}

const char* tuya_atop_server_host_get()
{
    return endpoint_mgr.endpoint.atop.host;
}

uint16_t tuya_atop_server_port_get()
{
    return endpoint_mgr.endpoint.atop.port;
}

const uint8_t* tuya_atop_server_cacert_get()
{
    return (const uint8_t*)endpoint_mgr.endpoint.atop.cert;
}

size_t tuya_atop_server_cacert_length_get()
{
    return endpoint_mgr.endpoint.atop.cert_len;
}

const char* tuya_mqtt_server_host_get()
{
    return endpoint_mgr.endpoint.mqtt.host;
}

uint16_t tuya_mqtt_server_port_get()
{
    return endpoint_mgr.endpoint.mqtt.port;
}

const uint8_t* tuya_mqtt_server_cacert_get()
{
    return (const uint8_t*)endpoint_mgr.endpoint.mqtt.cert;
}

size_t tuya_mqtt_server_cacert_length_get()
{
    return endpoint_mgr.endpoint.mqtt.cert_len;
}