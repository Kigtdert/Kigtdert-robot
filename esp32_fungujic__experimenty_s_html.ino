#include "esp_http_server.h"
#include "esp_timer.h"
#include "esp_camera.h"
#include "img_converters.h"
#include "Arduino.h"
#include "esp_camera.h"
#include "esp32-hal-ledc.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <WiFi.h>
#include <ESP32Servo.h>
 
Servo myservo;  
int servoPin = 12;

#define CAMERA_MODEL_AI_THINKER

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

/* Wifi Crdentials */ /* Replace your SSID and Password */
const char* ssid = "Kalhoty283_2G";
const char* password = "vkalhotach283";
// adresa: 192.168.0.113

const int FlashPin = 4;

#define PART_BOUNDARY "123456789000000000000987654321"
static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";
httpd_handle_t stream_httpd = NULL;
httpd_handle_t camera_httpd = NULL;

/* Stream handler */
static esp_err_t stream_handler(httpd_req_t *req) {
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  char * part_buf[64];

  static int64_t last_frame = 0;
  if (!last_frame) {
    last_frame = esp_timer_get_time();
  }

  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if (res != ESP_OK) {
    return res;
  }

  while (true) {
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      res = ESP_FAIL;
    } else {
      {
        if (fb->format != PIXFORMAT_JPEG) {
          bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
          esp_camera_fb_return(fb);
          fb = NULL;
          if (!jpeg_converted) {
            Serial.println("JPEG compression failed");
            res = ESP_FAIL;
          }
        } else {
          _jpg_buf_len = fb->len;
          _jpg_buf = fb->buf;
        }
      }
    }
    if (res == ESP_OK) {
      size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if (res == ESP_OK) {
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    if (res == ESP_OK) {
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }
    if (fb) {
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } else if (_jpg_buf) {
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    if (res != ESP_OK) {
      break;
    }
    int64_t fr_end = esp_timer_get_time();
    int64_t frame_time = fr_end - last_frame;
    last_frame = fr_end;
    frame_time /= 1000;
    /*Serial.printf("MJPG: %uB %ums (%.1ffps)\n",
                  (uint32_t)(_jpg_buf_len),
                  (uint32_t)frame_time, 1000.0 / (uint32_t)frame_time
                 );*/
  }

  last_frame = 0;
  return res;
}

/* Command handler */
static esp_err_t cmd_handler(httpd_req_t *req)
{
  char*  buf;
  size_t buf_len;
  char variable[32] = {0,};
  char value[32] = {0,};

  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
    buf = (char*)malloc(buf_len);
    if (!buf) {
      httpd_resp_send_500(req);
      return ESP_FAIL;
    }
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
      if (httpd_query_key_value(buf, "var", variable, sizeof(variable)) == ESP_OK &&
          httpd_query_key_value(buf, "val", value, sizeof(value)) == ESP_OK) {
      } else {
        free(buf);
        httpd_resp_send_404(req);
        return ESP_FAIL;
      }
    } else {
      free(buf);
      httpd_resp_send_404(req);
      return ESP_FAIL;
    }
    free(buf);
  } else {
    httpd_resp_send_404(req);
    return ESP_FAIL;
  }

  int val = atoi(value);
  sensor_t * s = esp_camera_sensor_get();
  int res = 0;
  /* Flash LED control */
  if (!strcmp(variable, "servo"))
  {
    int servopos=val;
  }
  /* Robot direction control */
  else if (!strcmp(variable, "car")) {
    if (val == 1) {
      //Serial.println("Forward");
      Serial.println("7");
    }
    else if (val == 2) {
      //Serial.println("Turn Left");
      Serial.println("2");
    }
    else if (val == 3) {
      //Serial.println("Stop");
      Serial.println("3");
    }
    else if (val == 4) {
      //Serial.println("Turn Right");
     Serial.println("4");
    }
    else if (val == 5) {
      //Serial.println("Backward");
      Serial.println("5");
    }
  }
  /* Pan and Tilt servo control */
  else if (!strcmp(variable, "pantilt")) {
    if (val == 1) {
      digitalWrite(13,HIGH);
    }
    else if (val == 4) {
      digitalWrite(13,LOW); 
    }
    else if (val == 5) {
      Serial.print("9"); 
    }
    else if (val == 6) {
      Serial.print("901");  
    }
    else if (val == 7) {
      Serial.print("902");  
    }
    else if (val == 8) {
      Serial.print("903");  
    }
    else if (val == 2) {
      ledcWrite(3, 10); 
    }
    else if (val == 3) {
      ledcWrite(3, 0);
    }
  }
  else
  {
    myservo.write(val);
    res = -1;
  }

  if (res) {
    return httpd_resp_send_500(req);
  }

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
}


static esp_err_t status_handler(httpd_req_t *req) {
  static char json_response[1024];

  sensor_t * s = esp_camera_sensor_get();
  char * p = json_response;
  *p++ = '{';

  p += sprintf(p, "\"framesize\":%u,", s->status.framesize);
  p += sprintf(p, "\"quality\":%u,", s->status.quality);
  *p++ = '}';
  *p++ = 0;
  httpd_resp_set_type(req, "application/json");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, json_response, strlen(json_response));
}
/* Index HTML page design 
 *  1) fancy efekty teploty atd. - kruhy s progress barem (gaugemeter)
 *  2) propojení s klávesnicí - ovládání motorů
 *  3) upozornění když bude napětí baterie pod 6V
*/
static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!doctype html>
<html>
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Kigtdert Robot</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            background-color: #f2f2f2;
        }
        .container {
            max-width: 1500px;
            margin: 0 auto;
            padding: 20px;
            background-image: url(https://upload.wikimedia.org/wikipedia/commons/0/03/WEB_Technology.jpg);
            background-size: cover;   
            background-position: center; 
            background-repeat: no-repeat;
            border-radius: 5px;
            box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
            
        }
        .container1 {
            
            margin: 0 auto;
            padding: 20px;
            background-color: #fff;
            border-radius: 5px;
            margin-right: 200px;
            margin-left: 30px;
            box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
            display: inline-block;
            vertical-align: top;
        }
        .title {
            font-size: 32px;
            text-align: center;
            margin-bottom: 20px;
            color: #0097b5;
        }
        .image-container {
            text-align: center;
            margin-bottom: 20px;
            display: inline-block;
            vertical-align: top;
        }
        #camstream {
            width: 800px;
            max-width: 800px;
            display: inline-block;
            vertical-align: top;
            text-align: center;
        }
        .control-group {
            text-align: center;
            max-width: 350px;
            padding: 10px;
            background-color:  #fff;
            border-radius: 5px;
            box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
            display: inline-block;
            vertical-align: center;
            margin-right: 10px;
            margin-left: 20px;
        }
        .mid-group {
            text-align: center;
            max-width: 800px;
            margin: 0 auto;
            padding: 10px;
            background-color:  #fff;
            border-radius: 5px;
            box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
            display: inline-block;
        }
        .control-group3 {
            text-align: center;
            width: 750px;
            margin: 0 auto;
            padding: 10px;
            background-color:  #fff;
            display: inline-block;
        }
        .control-group2 {
            text-align: left;
            max-width: 100px;
            padding: 20px;
            background-color: #fff;
            border-radius: 5px;
            box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
            display: inline-block;
            vertical-align: top;
            margin-top:70px;        
            margin-right: 10px;
            margin-left: 10px;
            
        }
        .data1-group {
            
            width: 350px;
            height: 445px;
            margin: 0 auto;
            margin-bottom:50px;
            margin-left:10px;      
            background-color: #fff;
            border-radius: 5px;
            box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);  
            display: inline-block;
            vertical-align: top;         
        }
        .info1 {
            padding: 10px;
            margin: 0 auto;    
            background-color: #0097b5;
            color:white;
            border-radius: 5px;
            margin:25px;
            box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);  
            vertical-align: top;         
        }
                .data2-group { 
            text-align: center;          
            width: 400px;
            margin: 0 auto; 
            display: inline-block;         
        }
        .label { 
            padding:10px;
            font-size: 20px;       
        }
        .label2 { 
            padding:20px;
            font-size: 25px; 
            background-color: #0097b5;;
            border-radius: 5px;      
        }
        .slider {
            width: 80%;
            height: 15px;
            border-radius: 10px;
            background: #fff;
            outline: none;
        }
        .slider::-webkit-slider-thumb {
            width: 30px;
            height: 30px;
            border-radius: 50%;
            background: #fff;
            -webkit-appearance: none;
            appearance: none;
        }
        .button {
            background-color: #0097b5;
            width:100px;
            height:50px;
            border: none;
            color:white;
            border-radius: 4px;
            padding: 10px 25px;
            font-size: 16px;
            margin: 4px 2px;
            cursor: pointer;
        }
        .button1 {
            background-color: #0097b5;
            border: none;
            width:100px;
            height:50px;
            border-radius: 4px;
            color: white;
            padding: 10px 25px;
            font-size: 16px;
            margin: 4px 2px;
            cursor: pointer;
        }
    </style>
</head>
<body>
<div class="container">

      <div class="control-group2">
          <button class="button" id="laser" onclick="fetch(document.location.origin+'/control?var=pantilt&val=1');">Laser</button>
          <button class="button" id="laseroff" onclick="fetch(document.location.origin+'/control?var=pantilt&val=4');">Laser off</button>
          <button class="button" id="blesk" onclick="fetch(document.location.origin+'/control?var=pantilt&val=2');">Blesk</button>
          <button class="button" id="bleskoff" onclick="fetch(document.location.origin+'/control?var=pantilt&val=3');">Blesk off</button>
          <button class="button" id="radio" onclick="fetch(document.location.origin+'/control?var=pantilt&val=5');">Radio</button>
          <button class="button" id="radio1" onclick="fetch(document.location.origin+'/control?var=pantilt&val=6');">Radio 1</button>
          <button class="button" id="radio2" onclick="fetch(document.location.origin+'/control?var=pantilt&val=7');">Radio 2</button>
          <button class="button" id="radio3" onclick="fetch(document.location.origin+'/control?var=pantilt&val=8');">Radio 3</button>
          </div>

    <div class="mid-group">
      <div class="image-container">
          <img id="camstream" src="" alt="Camera Stream">
      </div>
      <div class="control-group3">
        <label for="servo" class="label">Servo Control</label><br>
        <input type="range" class="slider" id="servo" min="10" max="170" value="90" onchange="try{fetch(document.location.origin+'/control?var=servo&val='+this.value);}catch(e){}">
    </div>
    </div>
    <div class="data2-group">
      <div class="data1-group">
          <div class="label2">
          LIVE DATA:
          </div>
          <div class="info1">
            <div class="label">Teplota: 23°C</div>
          </div>
          <div class="info1">
            <div class="label">Vlhkost: 48%</div>
          </div>
          <div class="info1">
            <div class="label">Napětí baterie: 7,4V</div>
          </div>
          <div class="info1">
            <div class="label">Doba provozu: 5 minut</div>
          </div>
      </div>
      <div class="control-group">
        <div>
            <button class="button1" id="forward" onclick="fetch(document.location.origin+'/control?var=car&val=1');">↑</button>
        </div>
        <div>
            <button class="button1" id="turnleft" onclick="fetch(document.location.origin+'/control?var=car&val=2');">←</button>
            <button class="button1" id="stop" onclick="fetch(document.location.origin+'/control?var=car&val=3');">Stop</button>
            <button class="button1" id="turnright" onclick="fetch(document.location.origin+'/control?var=car&val=4');">→</button>
        </div>
          <div>
          <button class="button1" id="backward" onclick="fetch(document.location.origin+'/control?var=car&val=5');">↓</button>
          </div>
      </div>
      </div>
</div>


<script>
    window.onload = function() {
        document.getElementById("camstream").src = window.location.origin + ":81/stream";
    }
</script>
</body>
</html>

)rawliteral";

static esp_err_t index_handler(httpd_req_t *req){
    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, (const char *)INDEX_HTML, strlen(INDEX_HTML));
}

void startCameraServer()
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    httpd_uri_t index_uri = {
        .uri       = "/",
        .method    = HTTP_GET,
        .handler   = index_handler,
        .user_ctx  = NULL
    };

    httpd_uri_t status_uri = {
        .uri       = "/status",
        .method    = HTTP_GET,
        .handler   = status_handler,
        .user_ctx  = NULL
    };

    httpd_uri_t cmd_uri = {
        .uri       = "/control",
        .method    = HTTP_GET,
        .handler   = cmd_handler,
        .user_ctx  = NULL
    };

   httpd_uri_t stream_uri = {
        .uri       = "/stream",
        .method    = HTTP_GET,
        .handler   = stream_handler,
        .user_ctx  = NULL
    };
    
    if (httpd_start(&camera_httpd, &config) == ESP_OK) {
        httpd_register_uri_handler(camera_httpd, &index_uri);
        httpd_register_uri_handler(camera_httpd, &cmd_uri);
        httpd_register_uri_handler(camera_httpd, &status_uri);
    }

    config.server_port += 1;
    config.ctrl_port += 1;
    if (httpd_start(&stream_httpd, &config) == ESP_OK) {
        httpd_register_uri_handler(stream_httpd, &stream_uri);
    }
}


void initFlash() {
  /* Configuring PWM channels for Falsh LED */
  ledcSetup(3, 5000, 8); /* 5000 hz PWM, 8-bit resolution and range from 0 to 255 */
  ledcAttachPin(FlashPin, 3);
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  Serial.begin(57600);
  Serial.setDebugOutput(true);
  pinMode(13,OUTPUT);
  initFlash();
  myservo.setPeriodHertz(50); 
  myservo.attach(servoPin);
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  if (psramFound()) {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_QVGA);
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);

  /* Connecting to WiFi */
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  startCameraServer();
  Serial.println(WiFi.localIP());

  for (int i = 0; i < 5; i++) {
    ledcWrite(3, 10);
    delay(50);
    ledcWrite(3, 0);
    delay(50);
  }
}

void loop() {

}
