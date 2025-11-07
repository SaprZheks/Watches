#include "main.h"

void setup()
{
  pinMode(BUTTON_1,INPUT);
  pinMode(BUTTON_2,INPUT);
  pinMode(DHTPIN, INPUT);
  pinMode(OE,OUTPUT);
  pinMode(LATCH,OUTPUT);
  pinMode(CLK,OUTPUT);
  pinMode(SBROS,OUTPUT);
  pinMode(DATA,OUTPUT);
  pinMode(DISP_BRIGHTNESS,OUTPUT);
  pinMode(RED_LED,OUTPUT);
  pinMode(GREEN_LED,OUTPUT);
  pinMode(BLUE_LED,OUTPUT);
  digitalWrite(OE,LOW);
  digitalWrite(SBROS,HIGH);
  digitalWrite(LATCH,HIGH);
  attachInterrupt(digitalPinToInterrupt(BUTTON_1),buttons_interrupt1,RISING); //Настройка аппаратного прерывания для кнопок
  attachInterrupt(digitalPinToInterrupt(BUTTON_2),buttons_interrupt2,RISING);

  Serial.begin(9600); //Максимальная скорость 9600 бод, если поставить больше, будут иероглифы
  rtc.begin();
  timer_dht.loop(TIMER_DHT); //Начинаем таймер
  if(!SPIFFS.begin(true)) //Запуск SPIFFS
  {
    Serial.println("Произошла ошибка при монтаже SPIFFS");
    return;
  }
  WiFi.mode(WIFI_STA); //Установка режима "станция" 
  WiFi.setHostname("Sapr_Wathes"); //Установка названия станции
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) //Установка наших параметров
  {
    Serial.println("STA не установлен");
  }
  WiFi.begin(ssid, password); //Начинаем подключение к сети
  ////////////////////////////////////////////////////////////////
  ///////////////////Настройки сервера////////////////////////////
  ////////////////////////////////////////////////////////////////
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false);
  });
  server.on("/f", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/config.json", "json");
  });
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/favicon.png", "image/png");
  });
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    if(request->hasParam("output") && request->hasParam("state")){ //Если есть параметры output и state
      const AsyncWebParameter* p1 = request->getParam("output");
      const AsyncWebParameter* p2 = request->getParam("state");
      inputMessage1 = p1->value();
      inputMessage2 = p2->value();
      if(inputMessage1 == "onflag_toggle"){ //Если мы ткнули по ползунку
        onflag = inputMessage2.toInt();
      }else if(inputMessage1 == "mode_button"){ //Если ткнули по кнопке "изменить режим"
        regime++;
        if(regime == MAX_REGIME){
          regime = 0;
        }
      }else if(inputMessage1 == "set_time_button") //Если ткнули на "Установить дату и время" (Или на "Синхронизировать дату и время")
      {
        String data[7] = {"  ","  ","  ","  ","  ","  ","  "}; //h,m,s,y,m,d,day of week
        int j = 0; //Счётчик символов строки InputMessage2
        int k = 0; //Номер элемента в строке 1-й переменной (секунд, минут, ...)
        bool error_set_time_flag = 0;
        for(int i=0;i<7;i++)
        {
          k = 0;
          while((inputMessage2[j] != ':')&&(inputMessage2[j] != '-')&&(inputMessage2[j] != ';')) //Пока не наткнёмся на : - ;
          {
            data[i][k] = inputMessage2[j]; //Запись данных в соответствующие ячейки масссива для последующего преобразования в число
            k++;
            j++;
          }
          if(k == 0){ //Если между разделительными символами не было ни одного символа, то установить флаг ошибки в true и выйти из цикла
              error_set_time_flag = 1;
              break; //Выходим из цикла for, как только замечаем ошибку
          }
          if(inputMessage2[j] == ';') //Если наткнулись на ; следом будет год, т.е. лишние 2 символа
            j+=2; 
          j++;
        }
        if(error_set_time_flag == 0) //Если нет ошибок чтения, запись чисел в соответствующие переменные и установка флага для установки времени
        {
          h = data[0].toInt();
          m = data[1].toInt();
          s = data[2].toInt();
          y = data[3].toInt();
          mon = data[4].toInt();
          d = data[5].toInt();
          dofw = data[6].toInt(); //День недели
          rtc_set_flag = 1; //Установка флага для установки даты и времени
        }
      }else if(inputMessage1 == "brightness_display_slider_range"){ //Если ткнули на ползунок яркости дисплея
        brightness_display = inputMessage2.toInt();
        set_pwm_display = 1;
      }else if(inputMessage1 == "brightness_lighting_slider_range"){ //Если ткнули на ползунок яркости подсветки
        brightness_lighting = inputMessage2.toInt();
        set_pwm_lighting = 1;
      }else if(inputMessage1 == "color_lighting"){ //Если ткнули на изменение цвета подсветки
        String data[3] = {"   ","   ","   "}; //r,g,b
        int j = 0; //Счётчик символов строки InputMessage2
        int k = 0; //Номер элемента в строке переменной
        bool error_set_color_flag = 0;
        for(int i=0;i<3;i++)
        {
          k = 0;
          while(inputMessage2[j] != ';') //Пока не наткнёмся на ,
          {
            data[i][k] = inputMessage2[j]; //Запись данных в соответствующие ячейки масссива для последующего преобразования в число
            k++;
            j++;
          }
          if(k == 0){ //Если между разделительными символами не было ни одного символа, то установить флаг ошибки в true и выйти из цикла
              error_set_color_flag = 1;
              break; //Выходим из цикла for, как только замечаем ошибку
          }
          j++;
        }
        if(error_set_color_flag == 0) //Если нет ошибок чтения, запись чисел в соответствующие переменные и установка флага для установки цвета
        {
          red_led_value = data[0].toInt();
          green_led_value = data[1].toInt();
          blue_led_value = data[2].toInt();
          set_pwm_lighting = 1; //Установка флага для установки даты и времени
        }
      }
    }else{
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    AsyncWebServerResponse *response = request->beginResponse(200); 
    response->addHeader("Hum",(String)hum);
    response->addHeader("Tem",(String)tem);
    response->addHeader("Onflag",(String)onflag);
    response->addHeader("Mode",(String)regime);
    response->addHeader("RedLedValue",(String)red_led_value);
    response->addHeader("GreenLedValue",(String)green_led_value);
    response->addHeader("BlueLedValue",(String)blue_led_value);
    response->addHeader("BrightnessDisplay",(String)brightness_display);
    response->addHeader("BrightnessLighting",(String)brightness_lighting);

    request->send(response);
  });
  server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse(200); //Sends 404 File Not Found
    response->addHeader("Hum",(String)hum);
    response->addHeader("Tem",(String)tem);
    response->addHeader("Onflag",(String)onflag);
    response->addHeader("Mode",(String)regime);
    request->send(response);
    //Список всех заголовков
    int headers = request->headers();
    int i;
    for(i=0;i<headers;i++)
    {
      const AsyncWebHeader* h = request->getHeader(i);
      Serial.printf("HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
    }
    //Список всех параметров
    int params = request->params();
    for(int i=0;i<params;i++){
      const AsyncWebParameter* p = request->getParam(i);
      if(p->isFile()){ //p->isPost() is also true
      Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
      } else if(p->isPost()){
        Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
      } else {
        Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
    }
  });
  server.begin();

  read_config();//Чтение данных с файла config.json
  //Установка ШИМ
  ledcSetup(disp_pwm_channel, 5000, 8);
  ledcAttachPin(DISP_BRIGHTNESS, disp_pwm_channel);
  ledcWrite(disp_pwm_channel, brightness_display);

  ledcSetup(red_led_pwm_channel, 5000, 8);
  ledcAttachPin(RED_LED, red_led_pwm_channel);
  ledcWrite(red_led_pwm_channel, (int)((float)red_led_value * (float)brightness_lighting/100));
  
  ledcSetup(green_led_pwm_channel, 5000, 8);
  ledcAttachPin(GREEN_LED, green_led_pwm_channel);
  ledcWrite(green_led_pwm_channel, (int)((float)green_led_value * (float)brightness_lighting/100));
  
  ledcSetup(blue_led_pwm_channel, 5000, 8);
  ledcAttachPin(BLUE_LED, blue_led_pwm_channel);
  ledcWrite(blue_led_pwm_channel, (int)((float)blue_led_value * (float)brightness_lighting/100));
  Serial.println("brightness_display = " + (String)brightness_display + "\n red_led_value = " + (String)red_led_value + "\n green_led_value = " + (String)green_led_value + "\n blue_led_value = " + (String)blue_led_value);
  timer_wifi.loop(TIMER_WIFI); //Начинаем таймер
}

void loop() //Бесконечный цикл
{
  byte str_b[len];
  String str = "";
  update(); //Вызываем функцию обновления
  if(onflag == 1) //Если часы включены
  {
    switch (regime) //Выбор режима
    {
      case CLOCK: //Режим часов
        str = rtc.gettime("H:i:s"); //Получаем строку из RTC модуля
        if(str != str_old) //Если строка не совпадает с предыдущей
        {
          convert_str(str,str.length(),str_b); //Заполняем байт-кодом строку str_b
          push(str_b,len); //Выводим содержимое str_b
          str_old = str; //Обновляем старую строку
        }
        break;
      case DATE: //Режим даты
        str = rtc.gettime("d.m.20y"); //Получаем строку из RTC модуля
        if(str != str_old) //Если строка не совпадает с предыдущей
        {
          convert_str(str,str.length(),str_b); //Заполняем байт-кодом строку str_b
          push(str_b,len); //Выводим содержимое str_b
          str_old = str; //Обновляем старую строку
        }
        break;
      case TEM: //Режим температуры
        str = "t=";
        if(dht_error_flag == 0) //Если ошибок нет
        {
          str += (String)tem + "*C";
        }else{
          str += "ERROR";
        }
        if(str != str_old) //Если строка не совпадает с предыдущей
        {
          convert_str(str,str.length(),str_b); //Заполняем байт-кодом строку str_b
          push(str_b,len); //Выводим содержимое str_b
          str_old = str; //Обновляем старую строку
        }
        break;
      case HUM: //Режим влажности
        str = "h=";
        if(dht_error_flag == 0) //Если ошибок нет
        {
          str += (String)hum + "%";
        }else{
          str += "ERROR";
        }
        if(str != str_old) //Если строка не совпадает с предыдущей
        {
          convert_str(str,str.length(),str_b); //Заполняем байт-кодом строку str_b
          push(str_b,len); //Выводим содержимое str_b
          str_old = str; //Обновляем старую строку
        }
        break;
    }
  }else //Иначе выводим пустую строку
  {
    if(str != str_old) //Если строка не совпадает с предыдущей
    {
      convert_str(str,str.length(),str_b); //Заполняем байт-кодом строку str_b
      push(str_b,len); //Выводим содержимое str_b
      str_old = str; //Обновляем старую строку
    }
  }
}