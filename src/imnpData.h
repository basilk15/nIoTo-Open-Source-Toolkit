#include <driver/i2s.h>

#define I2S_SCK  14   
#define I2S_WS   15   
#define I2S_SD   32   

#define I2S_PORT I2S_NUM_0
#define SAMPLE_RATE 8000

void setupI2S() {
  const i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 512,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };

  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_PORT, &pin_config);
}


int32_t i2s_read_single_sample()
{
  uint8_t raw[4];
  size_t bytesRead = 0;
  if (i2s_read(I2S_PORT, raw, sizeof(raw), &bytesRead, portMAX_DELAY) != ESP_OK || bytesRead != 4) {
    return 0;
  }

  int16_t hi = (int16_t)((raw[3] << 8) | raw[2]);
  int32_t sample = (int32_t)hi;

  return sample;
}



void getImnpData() {
  doc['imnpSample'] = i2s_read_single_sample();
}
