#include "esphome.h"
#include <IRrecv.h>
#include <IRutils.h>
#include <IRac.h>
#include <IRtext.h>
#include <sstream>
#include <iomanip>

class IRLogger : public Component {
public:
  
  IRLogger(uint8_t pin) : irrecv_(pin, 4096, 50, true) {
    pin_ = pin;
  };


  void setup() override {
    pinMode(pin_, INPUT);
    irrecv_.enableIRIn();
  }

  void loop() override {
    decode_results results_;
    if (irrecv_.decode(&results_)) {
      ESP_LOGI("IRLogger", "==================================================================");
      ESP_LOGI("IRLogger", "Protocol:       %s", typeToString(results_.decode_type, results_.repeat).c_str());
      ESP_LOGI("IRLogger", "Decoded:        %s (%d bits)", resultToHexidecimal(&results_).c_str(), results_.bits);
      ESP_LOGI("IRLogger", "Interpretation: %s", IRAcUtils::resultAcToString(&results_).c_str());
      uint16_t* raw_ = resultToRawArray(&results_);
      uint16_t raw_len = getCorrectedRawLength(&results_);
      ESP_LOGI("IRLogger", "Raw data (len: %d):", raw_len);
      bool data_to_print = false;
      std::stringstream sstr("");
      for(int i = 0; i < raw_len; i++) {
        sstr << std::setw(4) << raw_[i] << " ";
        data_to_print = true;
        if ((i+1) % 40 == 0) {
          ESP_LOGI("IRLogger", "%s", sstr.str().c_str());
          sstr.str("");
          data_to_print = false;
        }          
      }
      if (data_to_print) {
        ESP_LOGI("IRLogger", "%s", sstr.str().c_str());
      }
      delete[] raw_;
    }
  }

protected:
  uint8_t pin_;
  IRrecv irrecv_;
};