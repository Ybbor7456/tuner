#include <driver/i2s.h>
#include <Arduino.h> 
#define SAMPLE_BUFFER_SIZE 512
#define SAMPLE_RATE 8000
// most microphones will probably default to left channel
#define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_LEFT
// either wire your microphone to the same pins or change these to match your wiring
#define I2S_MIC_SERIAL_CLOCK GPIO_NUM_32
#define I2S_MIC_LEFT_RIGHT_CLOCK GPIO_NUM_25
#define I2S_MIC_SERIAL_DATA GPIO_NUM_33

// don't mess around with this
i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 1024,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0};

// don't mess around with this
i2s_pin_config_t i2s_mic_pins = {
    .bck_io_num = I2S_MIC_SERIAL_CLOCK,
    .ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_MIC_SERIAL_DATA};



float estimateHz(const int32_t* buf32, size_t n32, float fs) {
  if (n32 < 4) return 0.0f;
  int32_t prev = buf32[0] >> 8;      // 24-bit signed
  unsigned crossings = 0;
  for (size_t i = 1; i < n32; ++i) {
    int32_t v = buf32[i] >> 8;
    // count sign changes
    if ((prev < 0 && v >= 0) || (prev > 0 && v <= 0)) crossings++;
    prev = v;
  }
  float periods = crossings * 0.5f;
  float seconds = n32 / fs;
  return (periods > 0 && seconds > 0) ? (periods / seconds) : 0.0f;
}

void setup()
{
  
  // we need serial output for the plotter
  Serial.begin(115200);
  Serial.print("starting");
  // start up the I2S peripheral
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &i2s_mic_pins);
}


int32_t raw[SAMPLE_BUFFER_SIZE];
void loop() {
  size_t br = 0;
  i2s_read(I2S_NUM_0, raw, sizeof(raw), &br, portMAX_DELAY);
  size_t n32 = br / sizeof(raw[0]);        // number of 32-bit samples

  // optional: quick level check
  long long sumAbs = 0;
  for (size_t i = 0; i < n32; ++i) {
    int32_t s = raw[i] >> 8;
    sumAbs += (s < 0 ? -s : s);
  }
  int avgAbs = n32 ? (int)(sumAbs / (long long)n32) : 0;

  float hz = estimateHz(raw, n32, SAMPLE_RATE);
  Serial.printf("Hz=%.2f  level=%d\n", hz, avgAbs);
}
