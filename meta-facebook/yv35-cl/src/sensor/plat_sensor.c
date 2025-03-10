#include <stdio.h>
#include <string.h>
#include "sdr.h"
#include "sensor.h"
#include "sensor_def.h"
#include "hal_i2c.h"
#include "plat_i2c.h"
#include "plat_func.h"
#include "pal.h"

bool stby_access(uint8_t snr_num);
bool DC_access(uint8_t snr_num);
bool post_access(uint8_t snr_num);

snr_cfg plat_sensor_config[] = {
  /* number,                           type,            port,           address,                  offset,             access check       arg0,   arg1,   cache,   cache_status */

  // temperature
  {SENSOR_NUM_TEMP_TMP75_IN          , type_tmp75     , i2c_bus2      , tmp75_in_addr           , tmp75_tmp_offset  , stby_access      , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_TEMP_TMP75_OUT         , type_tmp75     , i2c_bus2      , tmp75_out_addr          , tmp75_tmp_offset  , stby_access      , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_TEMP_TMP75_FIO         , type_tmp75     , i2c_bus2      , tmp75_fio_addr          , tmp75_tmp_offset  , stby_access      , 0     , 0     , 0      , SNR_INIT_STATUS},

  // NVME
  {SENSOR_NUM_TEMP_SSD0              , type_nvme      , i2c_bus2      , SSD0_addr               , SSD0_offset       , DC_access        , 0     , 0     , 0      , SNR_INIT_STATUS},
                                                                                                                                                                 
  // PECI                                                                                                                                                        
  {SENSOR_NUM_TEMP_CPU               , type_peci      , NULL          , CPU_PECI_addr           , NULL              , post_access      , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_TEMP_CPU_MARGIN        , type_peci      , NULL          , CPU_PECI_addr           , NULL              , post_access      , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_TEMP_CPU_TJMAX         , type_peci      , NULL          , CPU_PECI_addr           , NULL              , post_access      , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_TEMP_DIMM_A            , type_peci      , NULL          , CPU_PECI_addr           , NULL              , post_access      , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_TEMP_DIMM_C            , type_peci      , NULL          , CPU_PECI_addr           , NULL              , post_access      , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_TEMP_DIMM_D            , type_peci      , NULL          , CPU_PECI_addr           , NULL              , post_access      , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_TEMP_DIMM_E            , type_peci      , NULL          , CPU_PECI_addr           , NULL              , post_access      , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_TEMP_DIMM_G            , type_peci      , NULL          , CPU_PECI_addr           , NULL              , post_access      , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_TEMP_DIMM_H            , type_peci      , NULL          , CPU_PECI_addr           , NULL              , post_access      , 0     , 0     , 0      , SNR_INIT_STATUS},
                                                                                                                                                                 
  // adc voltage                                                                                                                                                 
  {SENSOR_NUM_VOL_STBY12V            , type_adc       , adc_port0     , NULL                    , NULL              , stby_access      , 667   , 100   , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_VOL_STBY3V             , type_adc       , adc_port2     , NULL                    , NULL              , stby_access      , 2     , 1     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_VOL_STBY1V05           , type_adc       , adc_port3     , NULL                    , NULL              , stby_access      , 1     , 1     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_VOL_BAT3V              , type_adc       , adc_port4     , NULL                    , NULL              , stby_access      , 3     , 1     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_VOL_STBY1V8            , type_adc       , adc_port15    , NULL                    , NULL              , stby_access      , 1     , 1     , 0      , SNR_INIT_STATUS},

  // VR voltage
  {SENSOR_NUM_VOL_PVCCD_HV           , type_vr        , i2c_bus5      , PVCCD_HV_addr           , VR_VOL_CMD        , DC_access        , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_VOL_PVCCINFAON         , type_vr        , i2c_bus5      , PVCCINFAON_addr         , VR_VOL_CMD        , DC_access        , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_VOL_PVCCFA_EHV         , type_vr        , i2c_bus5      , PVCCFA_EHV_addr         , VR_VOL_CMD        , DC_access        , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_VOL_PVCCIN             , type_vr        , i2c_bus5      , PVCCIN_addr             , VR_VOL_CMD        , DC_access        , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_VOL_PVCCFA_EHV_FIVRA   , type_vr        , i2c_bus5      , PVCCFA_EHV_FIVRA_addr   , VR_VOL_CMD        , DC_access        , 0     , 0     , 0      , SNR_INIT_STATUS},
  
  // VR current
  {SENSOR_NUM_CUR_PVCCD_HV           , type_vr        , i2c_bus5      , PVCCD_HV_addr           , VR_CUR_CMD        , DC_access        , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_CUR_PVCCINFAON         , type_vr        , i2c_bus5      , PVCCINFAON_addr         , VR_CUR_CMD        , DC_access        , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_CUR_PVCCFA_EHV         , type_vr        , i2c_bus5      , PVCCFA_EHV_addr         , VR_CUR_CMD        , DC_access        , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_CUR_PVCCIN             , type_vr        , i2c_bus5      , PVCCIN_addr             , VR_CUR_CMD        , DC_access        , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_CUR_PVCCFA_EHV_FIVRA   , type_vr        , i2c_bus5      , PVCCFA_EHV_FIVRA_addr   , VR_CUR_CMD        , DC_access        , 0     , 0     , 0      , SNR_INIT_STATUS},
  
  // VR temperature
  {SENSOR_NUM_TEMP_PVCCD_HV          , type_vr        , i2c_bus5      , PVCCD_HV_addr           , VR_TEMP_CMD       , DC_access        , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_TEMP_PVCCINFAON        , type_vr        , i2c_bus5      , PVCCINFAON_addr         , VR_TEMP_CMD       , DC_access        , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_TEMP_PVCCFA_EHV        , type_vr        , i2c_bus5      , PVCCFA_EHV_addr         , VR_TEMP_CMD       , DC_access        , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_TEMP_PVCCIN            , type_vr        , i2c_bus5      , PVCCIN_addr             , VR_TEMP_CMD       , DC_access        , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_TEMP_PVCCFA_EHV_FIVRA  , type_vr        , i2c_bus5      , PVCCFA_EHV_FIVRA_addr   , VR_TEMP_CMD       , DC_access        , 0     , 0     , 0      , SNR_INIT_STATUS},
  
  // VR power 
  {SENSOR_NUM_PWR_PVCCD_HV           , type_vr        , i2c_bus5      , PVCCD_HV_addr           , VR_PWR_CMD        , DC_access        , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_PWR_PVCCINFAON         , type_vr        , i2c_bus5      , PVCCINFAON_addr         , VR_PWR_CMD        , DC_access        , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_PWR_PVCCFA_EHV         , type_vr        , i2c_bus5      , PVCCFA_EHV_addr         , VR_PWR_CMD        , DC_access        , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_PWR_PVCCIN             , type_vr        , i2c_bus5      , PVCCIN_addr             , VR_PWR_CMD        , DC_access        , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_PWR_PVCCFA_EHV_FIVRA   , type_vr        , i2c_bus5      , PVCCFA_EHV_FIVRA_addr   , VR_PWR_CMD        , DC_access        , 0     , 0     , 0      , SNR_INIT_STATUS},
  
  // ME
  {SENSOR_NUM_TEMP_PCH               , type_pch       , i2c_bus3      , PCH_addr                , NULL              , stby_access      , 0     , 0     , 0      , SNR_INIT_STATUS},
  
  // HSC
  {SENSOR_NUM_TEMP_HSC               , type_hsc       , i2c_bus2      , HSC_addr                , HSC_TEMP_CMD      , stby_access      , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_VOL_HSCIN              , type_hsc       , i2c_bus2      , HSC_addr                , HSC_VOL_CMD       , stby_access      , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_CUR_HSCOUT             , type_hsc       , i2c_bus2      , HSC_addr                , HSC_CUR_CMD       , stby_access      , 0     , 0     , 0      , SNR_INIT_STATUS},
  {SENSOR_NUM_PWR_HSCIN              , type_hsc       , i2c_bus2      , HSC_addr                , HSC_PWR_CMD       , stby_access      , 0     , 0     , 0      , SNR_INIT_STATUS},
};

bool stby_access(uint8_t snr_num) {
  return 1;
}

bool DC_access(uint8_t snr_num) {
  return get_DC_status();
}

bool post_access(uint8_t snr_num) {
  return get_post_status();
}

bool pal_load_snr_config(void) {
  memcpy(&sensor_config[0], &plat_sensor_config[0], sizeof(plat_sensor_config));
  return 1;
};

