#define BLINKER_WIFI

#define BLINKER_MIOT_OUTLET

#include <Blinker.h>
//密钥
char auth[] = "*********";
//WIFI名称
char ssid[] = "MiaoSir";
//WIFI密码
char pswd[] = "miaosir233";

//继电器GPIO口 ESP8266建议使用 GPIO 4，5否者可能导致ESP8266不能正常工作
int RELAY = 4;
//开关状态口 GPIO 5
int SW_STATE = 5;

bool oState = false;         //继电器状态
int outlet_switch;      //用于存放开关状态


BlinkerButton Button1("btn-on");     //定义按钮数据
BlinkerButton Button2("btn-off");



void miotPowerState(const String & state)
{

  BLINKER_LOG("need set power state: ", state);

  if (state == BLINKER_CMD_ON) {     //小爱同学控制开命令

    digitalWrite(RELAY, HIGH);

    BlinkerMIOT.powerState("on");

    BlinkerMIOT.print();

    oState = true;
  }
  else if (state == BLINKER_CMD_OFF) {   //小爱同学控制关命令

    digitalWrite(RELAY, LOW);

    BlinkerMIOT.powerState("off");

    BlinkerMIOT.print();

    oState = false;
  }
}

void miotQuery(int32_t queryCode)      //小爱同学控制
{
  BLINKER_LOG("MIOT Query codes: ", queryCode);

  switch (queryCode)
  {
    case BLINKER_CMD_QUERY_ALL_NUMBER :
      BLINKER_LOG("MIOT Query All");
      BlinkerMIOT.powerState(oState ? "on" : "off");
      BlinkerMIOT.print();
      break;
    case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
      BLINKER_LOG("MIOT Query Power State");
      BlinkerMIOT.powerState(oState ? "on" : "off");
      BlinkerMIOT.print();
      break;
    default :
      BlinkerMIOT.powerState(oState ? "on" : "off");
      BlinkerMIOT.print();
      break;
  }
}

void dataRead(const String & data)      // 如果未绑定的组件被触发，则会执行其中内容
{
  BLINKER_LOG("Blinker readString: ", data);

  Blinker.vibrate();

  uint32_t BlinkerTime = millis();

  Blinker.print("millis", BlinkerTime);
}

void button1_callback(const String & state)     //点灯app内控制按键触发
{

  digitalWrite(RELAY, HIGH);
  BLINKER_LOG("get button state:on", state);
}
void button2_callback(const String & state)     //点灯app内控制按键触发
{
  digitalWrite(RELAY, LOW);
  BLINKER_LOG("get button state:off", state);

}

void setup()
{

  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);

  pinMode(SW_STATE, INPUT);             //定义GPIO 5为输入口
  pinMode(RELAY, OUTPUT);              //定义io口为输出
  digitalWrite(RELAY, LOW);           //定义io默认为高电平

  Blinker.begin(auth, ssid, pswd);

  outlet_switch = digitalRead(SW_STATE); //获取开关初始状态

  Blinker.attachData(dataRead);

  BlinkerMIOT.attachPowerState(miotPowerState);

  BlinkerMIOT.attachQuery(miotQuery);

  Button1.attach(button1_callback);
  Button2.attach(button2_callback);
}

void loop()
{
  Blinker.run();

  if (outlet_switch != digitalRead(SW_STATE)) {
    outlet_switch = digitalRead(SW_STATE);
    digitalWrite(RELAY, !oState);
    oState = !oState;
  }

}
