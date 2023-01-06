#define OE 2 // orange
#define LATCH 15 // green
#define CLK 17 // white
#define SBROS 16 // blue
#define DATA 4 // yellow
#define BUTTON_1 34//Кнопка сверху
#define BUTTON_2 35//Кнопка снизу
#define DHTPIN 14 //DHT22 - датчик влажности
#define DISP_BRIGHTNESS 0 //PWM контакт для изменения яркости дисплея
#define RED_LED 27
#define GREEN_LED 33
#define BLUE_LED 32

#define SD_CS 5
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCK 18

#define I2S_DOUT 12
#define I2S_BCLK 26
#define I2S_LRC 25

#define TIMER_WIFI 3000
#define TIMER_DHT 1500
#define TIMER_ALARM 750
#define MAX_REGIME 4

#include <iostream>
#include <iarduino_RTC.h>
#include <iarduino_DHT.h>
#include "WiFi.h"
#include <WebServer.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>
#include "SD.h"
#include "SPI.h"
#include "FS.h"
#include "Audio.h"
using namespace std;
String logs = ""; .//Строка для логов
const char* ssid = ""; //SSID вставить свой
const char* password = ""; //Пароль вставить свой
iarduino_RTC rtc(RTC_DS3231); //Объявление RTC-модуля
iarduino_DHT dht(DHTPIN); //Объявление датчика
const bool symbols[80][8] = {
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

String str_old;
volatile int regime = 0; //Режим
volatile bool onflag = 1; //Состояние включения/выключения
const int len = 64; //Битовая длина строки вывода на экран
volatile bool rtc_set_flag = 0; //Флаг для установки даты и времени
volatile bool error_set_time_flag = 0; //Флаг ошибки при чтении информации с веб-страницы
volatile int s,m,h,d,mon,y,dofw; //Переменные для хранения данных для установки даты и времени в цикле loop
////////////////////////////////////////
const int disp_pwm_channel = 0;        //
volatile int brightness_display = 50;  //
volatile bool set_pwm_display = 0;     //
const int red_led_pwm_channel = 1;     //
const int green_led_pwm_channel = 2;   //
const int blue_led_pwm_channel = 3;    //ШИМ
volatile int red_led_value = 125;      //
volatile int green_led_value = 125;    //
volatile int blue_led_value = 125;     //
volatile int brightness_lighting = 75; //
volatile int set_pwm_lighting = 0;     //
////////////////////////////////////////
//////////////////////////////
float hum = 0;               //
float tem = 0;               // DHT22
bool dht_error_flag = 0;     //
//////////////////////////////
////////////////////////////////////////////////////////////////////////////
volatile bool alarm_flag = 0;      //Флаг для моргания (вспомогательный)                                         
volatile bool alarm_time_mode = 0; //Флаг для моргания (вкл/выкл дисплея)  
volatile bool alarm_stop_flag = 0; //Флаг для остановки будильника         
volatile bool alarm_shift_flag = 0;//Флаг для переноса будильника         
//String track_name_audio = "";
//bool track_set_flag = 0;
//bool track_play_flag = 0; //Резерв функии
////////////////////////////////////////////////////////////////////////////
Audio audio;

enum regimes //Перечисляем режимы
{
  CLOCK,
  DATE,
  TEM,
  HUM,
  ALARM
};
class Timer //Класс таймера
{
  private:
    //const unsigned long time_max = 4294967295;
    volatile unsigned long time_1 = 0;
    volatile unsigned long time_2 = 0;
    volatile bool active_flag = 0;
    volatile bool overflow_flag = 0;
  public:
    void clean() //Функция очистки
    {
      active_flag = 0;
      overflow_flag = 0;
    }
    void loop(volatile unsigned long interval = 0) //Циклическая функция таймера
    {
      if(active_flag == 0) //Если таймер еще не запущен
      {
        clean();
        time_1 = millis();
        time_2 = time_1 + interval; //Тут либо все прибавится нормально, либо мы сдвинемся в начало и начнем отсчет сначала
        active_flag = 1; //Таймер активирован
        if(time_2 < time_1) overflow_flag = 1; //Если было переполнение, то надо об этом знать
      }
    }
    bool check() //Функция проверки достижения нужного времени
    {
      volatile unsigned long time_now = millis(); //Считываем текущее время
      if(overflow_flag == 0) //Если не было переполнения
      {
        if ((time_now >= time_2) || (time_now < time_1)) //Если наше время дошло до или перешло за time2 (конец отсчёта) или мы как-то заступили за верхнюю границу, вследствие чего оказались в начале отсчета
        {
          //Действия при окончании таймера
          clean();
          return 1;
        }else return 0;
      }else   //Если было переполнение
      {
        if ((time_now >= time_2) && (time_now < time_1)) //Если наше время дошло до или перешло за time2 (конец отсчёта)
        {
          //Действия при окончании таймера
          clean();
          return 1;
        }
        else return 0;
      }
    }
};
class Alarm //Класс будильника
{
  public:
    bool active = 1;                                                              //Включен ли будильник
    String time_start = "09:00";                                                  //Время начала будильника
    bool days[7] = {0,0,0,0,0,0,0};                                               //В какие дни активен (Вс,Пн,Вт,Ср,Чт,Пт,Сб)
    int sum(bool array[], int length) //Метод для подсчёта контрольной суммы дней недели
    {
      int c = 0;
      for(int i=0;i<length;i++)
      {
        c += array[i];
      }
      return c;
    }
    int check_sum = sum(days,7); //Контрольная сумма, нужна, чтоб понимать, что будильник одноразовый  
    String track_name = "Bush.mp3";   //Имя трека
    unsigned long interval = 60000;                                               //Продолжительность будильника
    int volume = 21;                                                              //Громкость будильника (От 1 до 21)
    bool shift_active = 0;                                                        //Перенесен ли будильник
    unsigned long shift_interval = 30000;                                          //Продолжительность переноса будильника 30 секунд
    volatile bool run_flag = 0;                                                   //Флаг, показывающий, выполняется ли будильник
    volatile bool done_flag = 0;                                                  //Флаг, показывающий, завершён ли будильник
    Timer timer;

    void stop() //Функция остановки будильника
    {
      track_play_flag = 0;
      run_flag = 0;
      done_flag = 1;
      shift_active = 0;
      alarm_flag = 0; 
      alarm_time_mode = 0;
      if(check_sum == 0) //Если будильник одноразовый, то отключить его
        active = 0;
      timer.clean(); //Очистка таймера, ибо он может быть прерван из-за нажатия кнопки (Это нужная строчка!)
    }
    void shift() //Функция переноса будильника (остановка + таймер переноса)
    {
      run_flag = 0;
      done_flag = 0;
      shift_active = 1;
      alarm_flag = 0; 
      alarm_time_mode = 0;
      timer.clean(); //Очистка таймера
    }
    bool checkStart() //Функция проверки для старта будильника (с защитой от двойного старта и старта во время уже запущенного будильника)
    {
      if(run_flag == 0) //Если будильник ещё не выполняется
      {
        if(shift_active == 1) //Если будильник перенесли
        {
          timer.loop(shift_interval); //Запустить таймер для повторного запуска будильника (если он уже запущен, ничего не произойдет)
          if(timer.check()) //Если пора врубать повторный будильник
          {
            run_flag = 1;
            timer.loop(interval); //Очищаем старый таймер и пускаем новый для проигрывания музыки
            String path = "/Music/" + track_name;
            track_name_audio = path;
            track_set_flag = 1;
            track_play_flag = 1;
            audio.connecttoFS(SD,path.c_str());
            audio.setVolume(volume); //Устанавливаем громкость
            onflag = 1; //Включить экран
            regime = ALARM; //Переключить режим в состояние будильника
            return 1;
          }
        }else{
          if(active == 1) //Иначе е сли будильник активен
          {
            String moment = rtc.gettime("H:i:w");   //Текущий момент
            if(time_start == moment.substring(0,5)) //Если моменты времени совпали (момент = минута времени)
            {
              if(done_flag == 0)                    //Если будильник еще не был завершён
              {
                if((days[moment.substring(6,7).toInt()])||(check_sum == 0)) //Если текущий день недели и тот, что хранится в будильнике, совпадают или это активный одноразовый будильник, включить будильник
                {
                  run_flag = 1; //Будильник запущен
                  timer.loop(interval); //Запустить таймер
                  String path = "/Music/" + track_name; //Путь к файлу с треком
                  audio.connecttoFS(SD,path.c_str()); //Подрубаем музон
                  audio.setVolume(volume); //Устанавливаем громкость
                  onflag = 1; //Включить экран
                  regime = ALARM; //Переключить режим в состояние будильника
                  return 1;
                }
              }
            }else{
              done_flag = 0; //Когда моменты времени не совпадают, надо обнулить флаг, чтоб будильник мог зазвоить в другой день
            }
          }
        }
      }
      return 0;
    }
    void log()
    {
      Serial.println("active = " + (String)active);
      Serial.println("time_start = " + (String)time_start);
      Serial.println("runflag = " + (String)run_flag);
      Serial.println("done_flag = " + (String)done_flag);
      Serial.println("shift_active = " + (String)shift_active);
      Serial.println("alarm_flag = " + (String)alarm_flag);
      Serial.println("alarm_time_mode = " + (String)alarm_time_mode);
    }
    bool checkStop()
    {
      if(run_flag) //Будильник запущен
      {
        audio.loop();
        audio.loop();
        audio.loop(); //Забиваем буфер усилителя
        if(timer.check() && run_flag) //Если пора вырубать (run_flag для переноса будильника)
        {
          stop();
          return 1;
        }
      }
      return 0;
    }
    bool run() //Метод определения активности будильника
    {
      return run_flag;
    }
    void refresh(const char t_st[],const char tr_n[], unsigned long in,unsigned long sh_in,int v, bool act, bool d[]) //Метод быстрой настройки всех параметров
    {
      active = act;           //Включен ли будильник
      time_start = t_st;      //Время начала будильника
      for(int i=0;i<7;i++)
      {
        days[i] = d[i];       //В какие дни активен (Вс,Пн,Вт,Ср,Чт,Пт,Сб)
      }
      check_sum = sum(days,7); //Контрольная сумма, нужна, чтоб понимать, что будильник одноразовый  
      track_name = tr_n;      //Имя трека
      interval = in;          //Продолжительность будильника
      shift_interval = sh_in; //Продолжительность переноса будильника 3 секунды
      volume = v;             //Громкость будильника (От 1 до 21)
    }
    void refresh(const char t_st[],const char tr_n[], unsigned long in,unsigned long sh_in,int v, bool act) //Метод быстрой настройки всех параметров (Перегрузка 1)
    {
      active = act;           //Включен ли будильник
      time_start = t_st;      //Время начала будильника
      track_name = tr_n;      //Имя трека
      interval = in;          //Продолжительность будильника
      shift_interval = sh_in; //Продолжительность переноса будильника 3 секунды
      volume = v;             //Громкость будильника (От 1 до 21)
    }
    Alarm(){}
    ~Alarm(){} //Деструктор
};
class AlarmControl //Класс управления будильниками
{
  public:
    Alarm* alarms = nullptr;
    volatile int quantity_alarms = 0; //Количество будильников      
    volatile bool alarm_changed_flag = 0; //Флаг изменения текущего будильника 
    volatile int current_alarm = -1; //Индекс текущего будильника     

    int time_to_int(String time) //Метод преобразования строки времени в число для сортировки
    {
      return time.substring(0,2).toInt()*100 + time.substring(3,5).toInt();
    }
    void sort(Alarm *mas,int size) //Рекурсивная функция быстрой сортировки массива. Указатели в начало и в конец массива
    {
      if(size < 2) return; //Если элементов меньше 2-х - выход из функции (на этом моменте заканчивается рекурсия)
      int kL = 0;  //Счётчик меньших чисел
      int kM = 0;  //Счётчик равных чисел
      int kR = 0;  //Счётчик больших чисел
      int mid = time_to_int(mas[size/2].time_start); //Задаём центральный элемент массива (барьер)
      Alarm *L = new Alarm[size]; //Объявляем массив элементов, которые нужно поставить слева от барьера
      Alarm *R = new Alarm[size]; //Объявляем массив элементов, которые нужно поставить справа от барьера
      Alarm *M = new Alarm[size]; //Объявляем массив элементов, которые нужно поставить рядом с барьером
      for(int i=0;i<size;i++) //Заполняем наши дополнительные массивы значениями и подсчитываем длины этих массивов
      {
        if(time_to_int(mas[i].time_start) < mid)
        {
          L[kL] = mas[i];
          kL++;
        }else if(time_to_int(mas[i].time_start) > mid)
        {
          R[kR] = mas[i];
          kR++;
        }else
        {
          M[kM] = mas[i];
          kM++;
        }
      }
      //Если есть, куда делить, вызовется еще раз эта рекурсивная функция
      if(kL > 1)
      {
        //Левый кусок
        sort(L,kL);
      }
      if (kR > 1)
      {
        //Правый кусок
        sort(R,kR);
      }
      //Склеивание всех частей в 1 массив
      for(int i=0;i<kL;i++)
      {
        mas[i] = L[i];
      }
      for(int i=0;i<kM;i++)
      {
        mas[i+kL] = M[i];
      }
      for(int i=0;i<kR;i++)
      {
        mas[i+kL+kM] = R[i];
      }
      delete[] L;
      delete[] R;
      delete[] M;
      //loga += "sort was finised \n";
    }                                                 
    void newAlarm(const char t_st[],const char tr_n[], unsigned long in,unsigned long sh_in,int v,bool act,bool d[]) //Метод создания нового будильника
    {
      quantity_alarms++;
      if(quantity_alarms >= 2) //Если по итогу должно быть как минимум 2 будильника (Уже есть хотя бы 1 будильник)
      {
        Alarm *array = new Alarm[quantity_alarms-1];
        for(int i=0;i<quantity_alarms-1;i++)
        {
          array[i] = alarms[i]; //Копирование данных перед выделением памяти alarms
        }
        alarms = new Alarm[quantity_alarms]; //Выделение памяти с помощью Дефолтного конструктора Alarm()
        for(int i=0;i<quantity_alarms-1;i++) //Цикл копирования данных
        {
          alarms[i] = array[i]; //Копирование данных обратно из буфера
        }
        delete[] array; //Очищаем данные с буферного массива
      }else{
        alarms = new Alarm[quantity_alarms]; //Выделение памяти с помощью Дефолтного конструктора Alarm()
      }
      alarms[quantity_alarms-1].refresh(t_st, tr_n, in, sh_in, v, act,d);
      sort(alarms,quantity_alarms); //Сортировка всех будильников по времени
    }
    void newAlarm(const char t_st[],const char tr_n[], unsigned long in,unsigned long sh_in,int v,bool act) //Метод создания нового будильника (Перегрузка 1)
    {
      quantity_alarms++;
      if(quantity_alarms >= 2) //Если по итогу должно быть как минимум 2 будильника (Уже есть хотя бы 1 будильник)
      {
        Alarm *array = new Alarm[quantity_alarms-1];
        for(int i=0;i<quantity_alarms-1;i++)
        {
          array[i] = alarms[i]; //Копирование данных перед выделением памяти alarms
        }
        alarms = new Alarm[quantity_alarms]; //Выделение памяти с помощью Дефолтного конструктора Alarm()
        for(int i=0;i<quantity_alarms-1;i++) //Цикл копирования данных
        {
          alarms[i] = array[i]; //Копирование данных обратно из буфера
        }
        delete[] array; //Очищаем данные с буферного массива
      }else{
        alarms = new Alarm[quantity_alarms]; //Выделение памяти с помощью Дефолтного конструктора Alarm()
      }
      alarms[quantity_alarms-1].refresh(t_st, tr_n, in, sh_in, v, act);
      sort(alarms,quantity_alarms); //Сортировка всех будильников по времени
    }
    void deleteAlarm(int index) //Метод удаления будильника
    {
      if(index < 0 || index >= quantity_alarms) return; //Если индекс выходит за рамки массива, то выход из метода
      quantity_alarms--;
      if(quantity_alarms >= 1) //Если должен остаться хоть 1 будильник
      {
        Alarm *array = new Alarm[quantity_alarms];
        for(int i=0;i<index;i++)
        {
          array[i] = alarms[i]; //Идет копирование 1-й части массива
        }
        for(int i=index+1;i<quantity_alarms+1;i++)
        {
          array[i-1] = alarms[i]; //Идет копирование 2-й части массива со смещением
        }
        alarms = new Alarm[quantity_alarms];
        for(int i=0;i<quantity_alarms;i++)
        {
          alarms[i] = array[i];
        }
        delete[] array;
      }else{     //Иначе если удаляем последний будильник, то полностью освобождаем память
        delete[] alarms;
      }
    }
    void loop() //Циклический метод контроля будильников
    {
      if(alarm_stop_flag == 1) //Если надо остановить будильник, останавливаем все будильники
      {
        alarms[current_alarm].stop(); //Завершить будильник
        regime = CLOCK;
        current_alarm = -1;
        alarm_stop_flag = 0;
      }
      if(alarm_shift_flag == 1) //Если надо перенести будильник, переносим текущий
      {
        alarms[current_alarm].shift(); //Врубаем перенос текущего будильника
        regime = CLOCK;
        alarm_shift_flag = 0;
      }
      for(int i=0;i<quantity_alarms;i++) //Выбор одного самого позднего будильника из всех будильников
      {
        if(alarms[i].checkStart()) //Если выбранный будильник надо запустить
        {
          current_alarm = i; //Обновляем индекс текущего будильника
          alarm_changed_flag = 1; //Поднимаем флаг изменения текущего будильника
        }
      }
      if(alarm_changed_flag == 1) //Если другой будильник стал текущим, остановливаем остальные
      {
        for(int i=0;i<quantity_alarms;i++) //Остановка всех остальных будильников, которые уже были запущены перед текущим (current_alarm - крайний эл-т массива, который запущен => дальше него все будильники неактивированы)
        {
          if(i != current_alarm && alarms[i].run()) //Если будильник не текущий и активный, отрубаем
          {
            alarms[i].stop();
          }   
        }
        alarm_changed_flag = 0; //Зануляем флаг изменения текущего будильника
      }
      if(current_alarm != -1) //Если есть хоть 1 работающий будильник
      {
        if(alarms[current_alarm].checkStop()) //Если будильник пора завершить
        {
          current_alarm = -1;
          regime = CLOCK;
        }
      }
    }
    AlarmControl(){}  //Конструктор
    ~AlarmControl(){} //Деструктор
};
Timer timer_chatter; //Создаем таймер для контроля дребезга
Timer timer_dht; //Создаем таймер для контроля DHT22
Timer timer_wifi; //Создаем таймер для контроля wifi подключения
Timer timer_ws; //Создаем таймер для контроля ws
Timer timer_alarm_blink;//Создаем таймер для моргания экрана во время звонка будильника
volatile bool wifi_connect_status = 0;
IPAddress local_IP(192, 168, 1, 184); //Параметры статического IP-адреса
IPAddress gateway(192, 168, 1, 1); //Параметры шлюза
IPAddress subnet(255, 255, 0, 0); //Параметры маски подсети
IPAddress primaryDNS(8, 8, 8, 8);  //DNS1
IPAddress secondaryDNS(8, 8, 4, 4); //DNS2
AsyncWebServer server(80); //Установка WIFI-сервера. Порт 80
AlarmControl alarmManager;

/////////////////////////////////
////////////Функции//////////////
/////////////////////////////////

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
    if(timer_chatter.check() == 1) //Если таймер не был запущен
    {
      if(regime == ALARM) //Если в режиме будильника, другое поведение кнопок
      {
        alarm_shift_flag = 1; //Поднимаем флаг для переноса текущего будильника
      }else{
        regime++;
        if(regime == MAX_REGIME)
          regime = CLOCK;
      }
    }
    timer_chatter.loop(time_interval); //Установка таймера на 170 мс, если он еще не был запущен
  }
}
void IRAM_ATTR buttons_interrupt2() //Функция для обработки кнопки 2 после срабатывания прерывания 
{
  unsigned long time_interval = 170; //170мс
  if(timer_chatter.check() == 1) //Если таймер не был запущен (у обоих кнопок общий таймер)
  {
    if(regime == ALARM) //Если в режиме будильника, другое поведение кнопок
    {
      alarm_stop_flag = 1; //Поднимаем флаг для остановки будильника
    }else{
      onflag = !onflag;
    }
  }
  timer_chatter.loop(time_interval); //Установка таймера на 170 мс, если он еще не был запущен
}
void load_config() //Функция сохранения конфигурации (загрузка в энергонезависимую память конфигурации) 
{
  File file = SPIFFS.open("/config.json", FILE_WRITE);
    if (!file) {
      Serial.println(F("Ошибка при чтении файла"));
    return;
    }
    DynamicJsonDocument doc(1024);
    doc["brightness_display"] = brightness_display;
    doc["brightness_lighting"] = brightness_lighting;
    doc["red_led_value"] = red_led_value;
    doc["green_led_value"] = green_led_value;
    doc["blue_led_value"] = blue_led_value;
    if(serializeJsonPretty(doc, file) == 0)
    {
      Serial.println("Не удалось сохранить конфигурацию");
    }else{
      Serial.println("Новая конфигурация успешно сохранена!");
    }
    file.close();
}
void read_config() //Функция копирования конфигурации из конфига
{
  File file = SPIFFS.open("/config.json", "r");
  DynamicJsonDocument doc(1024);
  DeserializationError error;
  for(int i=0;i<=4;i++)
  {
    error = deserializeJson(doc, file);
    if(error) //если не удалось распарсить файл, то выдать ошибку, иначе заполнить переменные значениями из файла
    {
      Serial.println("Не удалось распарсить файл " + (String)i);
    }else{      
      Serial.println("Начало копирования");
      brightness_display = doc["brightness_display"];
      brightness_lighting = doc["brightness_lighting"];
      red_led_value = doc["red_led_value"];
      green_led_value = doc["green_led_value"];
      blue_led_value = doc["blue_led_value"];
      Serial.println("Копирование данных прошло успешно!");
      i=5;
    }
  }
  file.close();
}

void update() //Функция обновления для бесконечного цикла
{
  if(set_pwm_display == 1) //Если надо установить новое значение яркости дисплея
  {
    ledcSetup(disp_pwm_channel, 5000, 8);
    ledcAttachPin(DISP_BRIGHTNESS, disp_pwm_channel);
    ledcWrite(disp_pwm_channel, brightness_display);
    set_pwm_display = 0;
    load_config(); //Обновляем конфиг
  }

  if(set_pwm_lighting == 1) //Если надо установить новое значение яркости подсветки
  {
    ledcSetup(red_led_pwm_channel, 5000, 8);
    ledcAttachPin(RED_LED, red_led_pwm_channel);
    ledcWrite(red_led_pwm_channel, (int)((float)red_led_value * (float)brightness_lighting/100));
    
    ledcSetup(green_led_pwm_channel, 5000, 8);
    ledcAttachPin(GREEN_LED, green_led_pwm_channel);
    ledcWrite(green_led_pwm_channel, (int)((float)green_led_value * (float)brightness_lighting/100));
    
    ledcSetup(blue_led_pwm_channel, 5000, 8);
    ledcAttachPin(BLUE_LED, blue_led_pwm_channel);
    ledcWrite(blue_led_pwm_channel, (int)((float)blue_led_value * (float)brightness_lighting/100));
    set_pwm_lighting = 0;
    load_config(); //Обновляем конфиг
  }

  if(rtc_set_flag == 1) //Если надо установить дату и время
  { 
    rtc.settime(s,m,h,d,mon,y,dofw); // (СЕК, МИН, ЧАС, ДЕНЬ, МЕС, ГОД, ДН)
    rtc_set_flag = 0; //Зануляем флаг, чтоб больше не устонавливалось время
  }

  if(timer_wifi.check() == 1) //Если с частотой попыток подключения все нормально (Периодическая проверка подключения к WIFI)
  {
    if(WiFi.status() != WL_CONNECTED) //Если нет соединения, то выводим сообщение о подключении и начинаем повторное подключение к сети
    {
      Serial.println("Подсоединяемся к WiFi-сети...");
      wifi_connect_status = 0; //Статус подключения к WIFI приравниваем к 0
    }else
    {
      if(wifi_connect_status == 0) //Если статус подключения к WIFI всё еще равен 0, то надо 1 раз вывести сообщение с IP-адресом
        Serial.println(WiFi.localIP()); //Если только подключились, выводим IP-адрес
      wifi_connect_status = 1; //Обновляем статус подключения к WIFI
    }
    timer_wifi.loop(TIMER_WIFI); //Обновляем таймер для последующей проверки подключения
  }

  if(timer_dht.check() == 1) //Если с частотой обращений к датчику все нормально (считывание температуры и влажности)
  {
    switch(dht.read()) // Читаем показания датчика
    {  
      case DHT_OK: //Если все ок, выводим
        tem = dht.tem;
        hum = dht.hum;
        dht_error_flag = 0; //Зануляем флаг ошибки
        break;
      default:
        dht_error_flag = 1; //В случае ошибки поднимаем флаг
        break; 
    }
    timer_dht.loop(TIMER_DHT); //Обновляем таймер
  }
//.................................Моргание экрана...................................
  if((regime == ALARM)&&(timer_alarm_blink.check() == 1)) //Если будильник активен и уже пора переключить режим экрана
    {
      alarm_time_mode = !alarm_time_mode; //Сменить режим строки (вкл/выкл)
      timer_alarm_blink.loop(TIMER_ALARM); //Обновляем таймер для последующего моргания экраном
    }
  alarmManager.loop(); //Функция контроля будильников
}
