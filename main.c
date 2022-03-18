//https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json Это для подключения библиотеки, чтоб плата прошивалась
#define OE 18 // orange
#define LATCH 5 // green
#define CLK 17 // white
#define SBROS 16 // blue
#define DATA 4 // yellow
#define BUTTON_1 34//Кнопка сверху
#define BUTTON_2 35//Кнопка снизу
#define DHTPIN 19 //DHT22 - датчик влажности
#define DISP_BRIGHTNESS 23 //PWM контакт для изменения яркости дисплея
#define RED_LED 27
#define GREEN_LED 33
#define BLUE_LED 32

#include <iarduino_RTC.h>
#include <iarduino_DHT.h>
iarduino_RTC rtc(RTC_DS3231);
iarduino_DHT dht(DHTPIN); //Инициация датчика

bool symbols[80][8] = {
{0,1,0,1,1,1,1,1}, //A
{0,1,1,1,1,1,1,1}, //B
{0,0,1,1,0,1,1,0}, //C
{0,1,1,1,1,1,1,0}, //D
{0,0,1,1,0,1,1,1}, //E
{0,0,0,1,0,1,1,1}, //F
{0,1,1,1,0,1,1,1}, //G
{0,1,0,1,1,0,1,1}, //H
{0,0,0,1,0,0,1,0}, //I
{0,1,1,1,1,1,0,0}, //J
{0,0,0,0,0,0,0,0}, //K-----
{0,0,1,1,0,0,1,0}, //L
{0,0,0,0,0,0,0,0}, //M-----
{0,1,0,1,1,1,1,0}, //N
{0,1,1,1,1,1,1,0}, //O
{0,0,0,1,1,1,1,1}, //P
{0,0,0,0,0,0,0,0}, //Q
{0,0,0,1,0,1,1,0}, //R
{0,1,1,0,0,1,1,1}, //S
{0,0,1,1,0,0,1,1}, //T
{0,1,1,1,1,0,1,0}, //U
{0,0,0,0,0,0,0,0}, //V-----
{0,0,0,0,0,0,0,0}, //W-----
{0,0,0,0,0,0,0,0}, //X-----
{0,1,1,0,1,0,1,1}, //Y
{0,0,0,0,0,0,0,0}, //Z-----
{0,0,1,1,0,1,1,0}, //[
{0,0,0,1,1,0,0,0}, /*\*/
{0,1,1,0,1,1,0,0}, //]
{0,0,0,0,1,1,1,0}, //^
{0,0,1,0,0,0,0,0}, //_
{0,0,0,0,0,0,0,0}, //`-----
{0,1,0,1,1,1,1,1}, //a
{0,1,1,1,0,0,1,1}, //b
{0,0,1,1,0,0,0,1}, //c
{0,1,1,1,1,0,0,1}, //d
{0,0,1,1,0,1,1,1}, //e
{0,0,0,1,0,1,1,1}, //f
{0,1,1,0,1,1,1,1}, //g
{0,1,0,1,0,0,1,1}, //h
{0,0,0,1,0,0,0,0}, //i
{0,1,1,1,1,1,0,0}, //j
{0,0,0,0,0,0,0,0}, //k-----
{0,0,0,1,0,0,1,0}, //l
{0,0,0,0,0,0,0,0}, //m-----
{0,1,0,1,0,0,0,1}, //n
{0,1,1,1,0,0,0,1}, //o
{0,0,0,1,1,1,1,1}, //p
{0,1,0,0,1,1,1,1}, //q
{0,0,0,1,0,0,0,1}, //r
{0,1,1,0,0,1,1,1}, //s
{0,0,1,1,0,0,1,1}, //t
{0,1,1,1,0,0,0,0}, //u
{0,0,0,0,0,0,0,0}, //v-----
{0,0,0,0,0,0,0,0}, //w-----
{0,0,0,0,0,0,0,0}, //x-----
{0,1,1,0,1,0,1,1}, //y
{0,0,0,0,0,0,0,0}, //z-----
{0,0,0,0,0,0,0,0}, //{-----
{0,0,0,1,0,0,1,0}, //|
{0,0,0,0,0,0,0,0}, //}----- №60
//Числа
{0,1,1,1,1,1,1,0}, //0
{0,1,0,0,1,0,0,0}, //1
{0,0,1,1,1,1,0,1}, //2
{0,1,1,0,1,1,0,1}, //3
{0,1,0,0,1,0,1,1}, //4
{0,1,1,0,0,1,1,1}, //5
{0,1,1,1,0,1,1,1}, //6
{0,1,0,0,1,1,0,0}, //7
{0,1,1,1,1,1,1,1}, //8
{0,1,1,0,1,1,1,1}, //9
//Дальше идут отдельные символы
{0,0,0,0,0,0,0,0}, //Пробел №71
{0,0,0,0,1,1,1,1}, ////* (Градус) №72
{0,0,0,0,0,0,0,1}, //- №73
{0,0,0,0,1,0,1,0}, //" №74
{0,1,0,0,1,0,0,0}, /// №75
{0,0,0,0,0,1,0,1}, //= №76
{0,0,0,0,1,1,1,1}, //Первая половина процента №77
{0,1,1,1,0,0,1,1}  //Вторая половина процента №78
};
/*A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,[,\,],^,_,',a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,{,|,}*/
volatile int regime = 0; //Режим
volatile bool onflag = 1; //Состояние включения/выключения
String str_old;
const int len = 64;
volatile bool tem_flag = 0;
volatile bool hum_flag = 0;
const int disp_pwm_channel = 0;
const int red_led_pwm_channel = 1;
const int green_led_pwm_channel = 2;
const int blue_led_pwm_channel = 3;
enum regimes //Перечисляем режимы
{
  CLOCK,
  DATE,
  TEM,
  HUM
};
class Timer //Класс таймера
{
  public:
    const unsigned long time_max = 4294967295;
    volatile bool active_flag = 0;
    volatile bool overflow_flag = 0;
    volatile bool done_flag = 0;
    volatile unsigned long time_1 = 0;
    volatile unsigned long time_2 = 0;
    void clean() //Функция очистки
    {
      active_flag = 0;
      overflow_flag = 0;
      done_flag = 0;
    };
    void start(volatile unsigned long interval = 0) //Функция старта таймера
    {
      if(active_flag == 0) //Если таймер еще не запущен
      {
        clean();
        time_1 = millis();
        time_2 = time_1 + interval; //Тут либо все прибавится нормально, либо мы сдвинемся в начало и начнем отсчет сначала
        active_flag = 1; //Таймер активирован
        if(time_2 < time_1) overflow_flag = 1; //Если было переполнение, то надо об этом знать
      }
    };
    bool check() //Функция проверки достижения нужного времени
    {
      volatile unsigned long time_now = millis(); //Считываем текущее время
      switch(overflow_flag)
      {
        case 0: //Если не было переполнения
          if((time_now >= time_2)||(time_now < time_1)) //Если наше время дошло до или перешло за time2 (конец отсчёта) или мы как-то заступили за верхнюю границу, вследствие чего оказались в начале отсчета
          {
            //Действия при окончании таймера
            active_flag = 0;
            overflow_flag = 0;
            done_flag = 1;
            return 1;
          }else return 0;
          break;
        case 1: //Если было переполнение
          if((time_now >= time_2)&&(time_now < time_1)) //Если наше время дошло до или перешло за time2 (конец отсчёта)
          {
            //Действия при окончании таймера
            active_flag = 0;
            overflow_flag = 0;
            done_flag = 1;
            return 1;
          }else return 0;
          break;
      }
    };
};
Timer timer_chatter; //Создаем таймер для контроля дребезга
Timer timer_dht; //Создаем таймер для контроля DHT22

void convert_str(String str, int l, byte converted_str[]);
void IRAM_ATTR buttons_interrupt1(); //Функция для обработки кнопки 1 после срабатывания прерывания
void IRAM_ATTR buttons_interrupt2(); //Функция для обработки кнопки 2 после срабатывания прерывания
void push(byte str_b[],int len); //Функция вывода на дисплей байт-кода
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
  attachInterrupt(digitalPinToInterrupt(BUTTON_1),buttons_interrupt1,RISING); //Настройка аппаратного прерывания для кнопок
  attachInterrupt(digitalPinToInterrupt(BUTTON_2),buttons_interrupt2,RISING);
  ledcSetup(disp_pwm_channel, 5000, 8);
  ledcAttachPin(DISP_BRIGHTNESS, disp_pwm_channel);
  ledcWrite(disp_pwm_channel, 200);
  
  ledcSetup(red_led_pwm_channel, 5000, 8);
  ledcAttachPin(RED_LED, red_led_pwm_channel);
  ledcWrite(red_led_pwm_channel, 0);
  
  ledcSetup(green_led_pwm_channel, 5000, 8);
  ledcAttachPin(GREEN_LED, green_led_pwm_channel);
  ledcWrite(green_led_pwm_channel, 255);
  
  ledcSetup(blue_led_pwm_channel, 5000, 8);
  ledcAttachPin(BLUE_LED, blue_led_pwm_channel);
  ledcWrite(blue_led_pwm_channel, 255);
  Serial.begin(115200);
  rtc.begin();
  timer_dht.start(2000);
  digitalWrite(OE,LOW);
  digitalWrite(SBROS,HIGH);
  digitalWrite(LATCH,HIGH);
}

void loop()
{
  byte str_b[len];
  String str = "";
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
        if(timer_dht.check() == 1) //Если с частотой обращений к датчику все нормально
        {
          tem_flag = 0; //Зануляем флаг, чтоб обновились данные на дисплее
          switch(dht.read()) // Читаем показания датчика
          {  
            case DHT_OK: //Если все ок, выводим
              str += (String)dht.tem + "*C";
              break;
            case DHT_ERROR_CHECKSUM:
              str += "ERROR";
              timer_dht.start(2000); //Добавляем время для ликвидации ошибки
              break;
            case DHT_ERROR_DATA:
              str += "ERROR";
              timer_dht.start(2000); //Добавляем время для ликвидации ошибки
              break;
            case DHT_ERROR_NO_REPLY:
              str += "ERROR";
              timer_dht.start(2000); //Добавляем время для ликвидации ошибки
              break;
            default:
              str += "ERROR";
              timer_dht.start(2000); //Добавляем время для ликвидации ошибки
              break; 
          }
          timer_dht.start(2000); //Обновляем таймер
        }
        if((str != str_old)&&(tem_flag == 0)) //Если строка не совпадает с предыдущей и вывод в этом режиме производится впервые с момента его включения
        {
          convert_str(str,str.length(),str_b); //Заполняем байт-кодом строку str_b
          push(str_b,len); //Выводим содержимое str_b
          str_old = str; //Обновляем старую строку
        }
        tem_flag = 1; //Поднимаем флаг, чтоб не сбрасывалось текущее значение температуры
        break;
      case HUM: //Режим влажности
        str = "h=";
        if(timer_dht.check() == 1) //Если с частотой обращений к датчику все нормально
        {
          hum_flag = 0; //Зануляем флаг, чтоб обновились данные на дисплее
          switch(dht.read()) // Читаем показания датчика
          {  
            case DHT_OK: //Если все ок, выводим
              str += (String)dht.hum + "%";
              break;
            case DHT_ERROR_CHECKSUM:
              str += "ERROR";
              timer_dht.start(2000); //Добавляем время для ликвидации ошибки
              break;
            case DHT_ERROR_DATA:
              str += "ERROR";
              timer_dht.start(2000); //Добавляем время для ликвидации ошибки
              break;
            case DHT_ERROR_NO_REPLY:
              str += "ERROR";
              timer_dht.start(2000); //Добавляем время для ликвидации ошибки
              break;
            default:
              str += "ERROR";
              timer_dht.start(2000); //Добавляем время для ликвидации ошибки
              break; 
           }
         timer_dht.start(2000); //Обновляем таймер
        }
        if((str != str_old)&&(hum_flag == 0)) //Если строка не совпадает с предыдущей и вывод в этом режиме производится впервые с момента его включения
        {
          convert_str(str,str.length(),str_b); //Заполняем байт-кодом строку str_b
          push(str_b,len); //Выводим содержимое str_b
          str_old = str; //Обновляем старую строку
        }
        hum_flag = 1; //Поднимаем флаг, чтоб не сбрасывалось текущее значение влажности
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


void convert_str(String string, int l, byte converted_str[]) //Функция конвертации строки в байт-код для вывода на дисплей
{
  bool doubledot_flag = 0;
  bool set_flag = 0;
  bool dot_flag = 0;
  int n = 0;
  for (int i=0;i<l;i++) //Цикл по всем символам из строчки 
  {
    if(string[i]>=48 && string[i]<=57) //Автоматическая подстановка байт-кода для символов чисел
    {
      for(int j=0;j<8;j++)
      {
        converted_str[8*(i-n)+j] = symbols[(int)string[i]+13][j]; //До цифр идут 60 символов, и еще 0 — 48-й символ
      }
    }else if(string[i]>=65 && string[i]<=175) //Автоматическая подстановка байт-кода для символов из основного диапазона
    {
      for(int j=0;j<8;j++)
      {
        converted_str[8*(i-n)+j] = symbols[(int)string[i]-65][j];
      }
    }else switch (string[i]) //Распознавание других символов строки и дописывание байт-кода
    {
      case ' ':
        for(int j=0;j<8;j++)
        {
          converted_str[8*(i-n)+j] = symbols[71][j];
        }
        break;
      case '.': //Если точка, то не нужно занимать лишний символ
        n++;
        converted_str[8*(i-n)] = 1;
        dot_flag = 1; //Флаг того, что текущий символ — точка, нужен для того, чтоб корректно работала подстановка двоеточия
        break;
      case ':': //Если двоеточие, то флаг doubledot_flag поднимается для того, чтоб проставить точку в следующем символе
        n++;
        converted_str[8*(i-n)] = 1;
        set_flag  = 1;
        break;
      case '*':
        for(int j=0;j<8;j++)
        {
          converted_str[8*(i-n)+j] = symbols[72][j];
        }
        break;
      case '-':
        for(int j=0;j<8;j++)
        {
          converted_str[8*(i-n)+j] = symbols[73][j];
        }
        break;
      case '"':
        for(int j=0;j<8;j++)
        {
          converted_str[8*(i-n)+j] = symbols[74][j];
        }
        break;
      case '/':
        for(int j=0;j<8;j++)
        {
          converted_str[8*(i-n)+j] = symbols[75][j];
        }
        break;
      case '=':
        for(int j=0;j<8;j++)
        {
          converted_str[8*(i-n)+j] = symbols[76][j];
        }
        break;
      case '%': //процент занимает 2 индикатора
        n--;
        for(int j=0;j<8;j++)
        {
          converted_str[8*(i-n-1)+j] = symbols[77][j];
          converted_str[8*(i-n)+j] = symbols[78][j];
        }
        break;
      default: //Если символ не был распознан, он заменится на пробел
        for(int j=0;j<8;j++)
        {
          converted_str[8*(i-n)+j] = 0;
        }
        break;
    }
    if(doubledot_flag == 1 && dot_flag == 0) //Если было двоеточие до этого символа, точки на текущей позиции не было, то проставить точку в этом символе
    {
      converted_str[8*(i-n)] = 1;
      doubledot_flag = 0;
    }
    dot_flag = 0;
    if(set_flag == 1) //Дополнительный флаг, чтоб избежать срабатывания предыдущего условия
    {
      doubledot_flag = 1;
      set_flag = 0;
    }
  }
  //Далее мы делаем пустыми оставшиеся символы
  for(int i=l-n;i<len/8;i++) //len - количество бит, а нам нужно количество байт
    for(int j=0;j<8;j++)
        {
          converted_str[8*i+j] = 0;
        }
  if(doubledot_flag == 1) //Если было двоеточие до этого символа, то проставить точку в этом символе
    {
      converted_str[8*(l-n)] = 1;
      doubledot_flag = 0;
    }
}

void push(byte str_byte[],int length_of_str_byte) //Функция вывода на дисплей байт-кода
{
  digitalWrite(LATCH,LOW);
  digitalWrite(CLK,LOW);
  for(int i=0;i<length_of_str_byte;i++)
  {
    digitalWrite(DATA,str_byte[length_of_str_byte-i-1]);
    digitalWrite(CLK,HIGH);
    digitalWrite(CLK,LOW);
    digitalWrite(DATA,LOW);
  } 
  digitalWrite(LATCH,HIGH);
}

void IRAM_ATTR buttons_interrupt1() //Функция для обработки кнопки 1 после срабатывания прерывания 
{
  unsigned long time_interval = 170; //170мс
  if(onflag == 1) //Изменять режим, только если экран вкллючен
  {
    if(timer_chatter.check() == 1) //Если таймер не был запущен//
    {
      hum_flag = 0;
      tem_flag = 0;
      regime++;
      if(regime == 4) regime = CLOCK;
    }
    timer_chatter.start(time_interval); //Установка таймера на 170 мс, если он еще не был запущен
  }
}
void IRAM_ATTR buttons_interrupt2() //Функция для обработки кнопки 2 после срабатывания прерывания 
{
  unsigned long time_interval = 170; //170мс
  if(timer_chatter.check() == 1) //Если таймер не был запущен (у обоих кнопок общий таймер)
  {
    onflag = !onflag;
  }
  timer_chatter.start(time_interval); //Установка таймера на 170 мс, если он еще не был запущен
}
