#include <spartan-edge-esp32-boot.h>
#include "ESP32IniFile.h"
#include "sea_esp32_qspi.h"
#include "string.h"

#include "AWS_IOT.h"
#include "WiFi.h"

#define LIMIT 500
#define ZERO 120
#define lt 5000

// initialize the spartan_edge_esp32_boot library
spartan_edge_esp32_boot esp32Cla;
AWS_IOT hornbill;

char WIFI_SSID[]="TP-LINK_8974";
char WIFI_PASSWORD[]="8806129xyz";
char HOST_ADDRESS[]="a1ovd07wi5j3um-ats.iot.us-east-1.amazonaws.com";
char CLIENT_ID[]= "MyTestClient";
char TOPIC_NAME[]= "$aws/things/myTestThing/shadow/update";

const char aws_root_ca_pem[]="-----BEGIN CERTIFICATE-----\n\
MIIMIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\
rqXRfboQnoZsG4q5WTP468SQvvG5\
-----END CERTIFICATE-----\n";

const char certificate_pem_crt[]="-----BEGIN CERTIFICATE-----\n\
MIIMIIDWTCCAkGgAwIBAgIUcJIq7RN7yUBXs38cP7UpbAkzUjMwDQYJKoZIhvcNAQEL\
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIwMDgwNTA2MTAz\
NFoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAK4nF2nfqDZXUR61qiuT\
rTqBthlksnFedvvXSOalVz0I3LNeoax7Wrpxp9sO4iP5fHKfHFBrMPG7GEhnkSUD\
PT0WkUIcwavPDSzUqGs6WysNQRqvPZ1q4i0iwsRFbJfbNVdDWD36QLBJanLJPXio\
yHo0AEpTz7u5u9of8Dr6QOWuH+Rh89VqLVltDFdEX92H0NoqSdljLcXL5SWDM6EM\
0Vsd65MJSrvNWO9wyX3FNQuyITZkrltRyJeVGDgwAV3XBT4iEUoMQV84r853fDL5\
3tKHHlQaT48+wp20KNC4ApsYn1AethnKbCFlnoSlkONDqtyOxk6D/ekODymWqI2G\
o1sCAwEAAaNgMF4wHwYDVR0jBBgwFoAUG6netR7afoqqlW+Zh9r2y/Nc+ngwHQYD\
VR0OBBYEFEEgqPqH6Knq9PpU4JFzyJlEqckNMAwGA1UdEwEB/wQCMAAwDgYDVR0P\
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQDQpxc+GrEtkD+LQuwarBisqWVp\
cbgftjFmr4Up2zmY9I0eYyJduCBetROnC6TKMlzCUPqEXgPUYmjnF7VicObDRB/t\
kfrVcDHBLP9Fyco6Q0QVNiHwESh/CKmJaZEQDEFPt2ZFwW45aWM3FcQda1g37ZVf\
+OXHg9CETeN38ZEZIePNCeWBXRup0FwhqCL0BitW0bpFrckdV4Vq1C5ajTwrcEOj\
ui3eGXJLJ8Zb5CcAQCDt+2LjzVClv9q8yrGCPMvnJ4rS9/XxliutmhEL0vZYldhe\
fH6SOPcDfiHToa6Mz6acJ5+0HTM3qALisTUTw3Huc4oTk4mXiSJLLAWysTPt\
-----END CERTIFICATE-----\n";


const char private_pem_key[]="-----BEGIN RSA PRIVATE KEY-----\n\
MIIMIIEogIBAAKCAQEAricXad+oNldRHrWqK5OtOoG2GWSycV52+9dI5qVXPQjcs16h\
rHtaunGn2w7iI/l8cp8cUGsw8bsYSGeRJQM9PRaRQhzBq88NLNSoazpbKw1BGq89\
nWriLSLCxEVsl9s1V0NYPfpAsElqcsk9eKjIejQASlPPu7m72h/wOvpA5a4f5GHz\
1WotWW0MV0Rf3YfQ2ipJ2WMtxcvlJYMzoQzRWx3rkwlKu81Y73DJfcU1C7IhNmSu\
W1HIl5UYODABXdcFPiIRSgxBXzivznd8Mvne0oceVBpPjz7CnbQo0LgCmxifUB62\
GcpsIWWehKWQ40Oq3I7GToP96Q4PKZaojYajWwIDAQABAoIBAA4/oIjHBco8ZqSu\
lFewY3Hv/ugg9wCTjASa3poQWQzjVrtOOMgV55xthCBGHXZ4CRiPsA9cVcrRVfFG\
l/eBRldjK0tPcwENlbRfSdBMG/1vvi3ivJXOkubCe1D3pSTfiIO1PNkFSpmhyBXQ\
JTlkBdMJwiRSqyJe1eHIzyzkIL8p110PVJ3HUB96MhRKkP9hTubaj0D4IL7kPDE/\
zy5FIzuDTmhcLnZhKhcBdRCmfdZZZzez4oC/C2qZFULaCPo5kLDcubs6PIexLyyW\
dt2QxGVAi9WyFbPum0ARV01kb16cDw9FXkoYYTqGbMUB8I1AG49jlNE0J2Wn3Vgd\
1caD0IECgYEA2Zkjrlz8+HEtGdKHUMtp0CxJGCawlqdEH54EyglcoZHz1gTaZIQp\
xXM8p41wGncNDi2NHiQX9YNGl33nAl2ePR0Q9OR/4LGk6rTlqdsKOSzLl4+Qh0eL\
fyTcYbS4VFG6HhWHhzB+ONrLfBii9Q5wftam7MJdhk4QPBlTEqMfY6ECgYEAzOMh\
KdJg5MGP/IV4uWv0pLe9kotKhfp8Knu/tHNK5iJrNcAYOsq01jjoN5vxfORRKYRA\
+I141oiuYNaVM+Nb4I2pxBLbvRzVO4OsM70S+qEzHpSDIIJQeUtc6phuN9VI2FhE\
Oer8BhE91RrQBZLoVNHoB+qiwyUBT0rum4hbpXsCgYAlid+38t7JVWz8aW9iptTo\
TtuFSRdpxg1gjvgwipDqZq10HH44nPw/zfOGzEWsqyEbcwxHSN7BQhZpiBTUOaZU\
0LDeLpVJBBx5SR/dv6Xx2yJ8UQ4T6GnOU2OZj33FPhwnmHs6/UipMkWscOub7xPF\
Le66JYXQ56KxW4UTUATYQQKBgHbXsB7QFr/Zvqkcyl/TTL5WjbF49issleUWjqYe\
0e0XPdSZrfQ8LBSuQZQv8i0dSi6otf72IIdgFLW7AiRs1pgz06sVvTu+g7jXsYT5\
QQ77felY/45VyFPy8NxqulPMdUp4OGrX8IOccJ8xxEPXaMf6UXp2ER7cUhwqPKke\
uPoNAoGAEL5Pl3q3IUiN9dvlN80SQjF4jnanoMJQSDhGerJ6YA930MAeXHyuDifB\
eu7lvsVrWfAGr0/fjm6T0oEjOFNLBVGbGh+4d3NvJ7w2B7zcNiOXnLsuac8ex7KJ\
JV7jDmaFpyX9StSwmoCdnA515Pz+Q/izA7/Fjt9PaGzS54SlVSI=\
-----END RSA PRIVATE KEY-----\n";



const size_t bufferLen = 80;
char buffer[bufferLen];
char buffer1[bufferLen];
// the setup routine runs once when you press reset:


int status = WL_IDLE_STATUS;
int tick=0,msgCount=0,msgReceived = 0,tick1=0;
char payload[512];
char rcvdPayload[512];
uint8_t data1[4] = {41,42,43,44};
uint8_t data2[32];
void mySubCallBackHandler (char *topicName, int payloadLen, char *payLoad)
{
    strncpy(rcvdPayload,payLoad,payloadLen);
    rcvdPayload[payloadLen] = 0;
    msgReceived = 1;
}
//////////////////////////////////////////////////////
//读取数据成16字节
    int16_t read(int ad)
    {
        
        uint8_t c[4];
        int16_t *b;
        SeaTrans.read(ad,c,4);
       uint8_t a[2]={c[2],c[3]};
        b=(int16_t*)a;
        return *b;
    }

    //设置状态
    int8_t state()
    {
        int16_t dat=read(0);
        if(dat-ZERO>LIMIT)
            return 1;
        else if(dat+ZERO<-LIMIT)
            return -1;
        else
            return 0; 
    }

/////////////////////////////////////////////////////


void setup()
{
 // initialization 
  esp32Cla.begin();

  // check the .ini file exist or not
  const char *filename = "/board_config.ini";
  IniFile ini(filename);
  if (!ini.open()) {
    Serial.print("Ini file ");
    Serial.print(filename);
    Serial.println(" does not exist");
    return;
  }
  Serial.println("Ini file exists");

  // check the .ini file valid or not
  if (!ini.validate(buffer, bufferLen)) {
    Serial.print("ini file ");
    Serial.print(ini.getFilename());
    Serial.print(" not valid: ");
    return;
  }

  // Fetch a value from a key which is present
  if (ini.getValue("Overlay_List_Info", "Overlay_Dir", buffer, bufferLen)) {
    Serial.print("section 'Overlay_List_Info' has an entry 'Overlay_Dir' with value ");
    Serial.println(buffer);
  }
  else {
    Serial.print("Could not read 'Overlay_List_Info' from section 'Overlay_Dir', error was ");
  }

  // Fetch a value from a key which is present
  if (ini.getValue("Board_Setup", "overlay_on_boot", buffer1, bufferLen)) {
    Serial.print("section 'Board_Setup' has an entry 'overlay_on_boot' with value ");
    Serial.println(buffer1);
  }
  else {
    Serial.print("Could not read 'Board_Setup' from section 'overlay_on_boot', error was ");
  }

  // Splicing characters
  strcat(buffer,buffer1);
  
  // XFPGA pin Initialize
  esp32Cla.xfpgaGPIOInit();

  // loading the bitstream
  esp32Cla.xlibsSstream(buffer);
/////////////////////////////////////////////////////////////
  //jiekou
  Serial.begin(115200);
  SeaTrans.begin();
  delay(2000);

    while (status != WL_CONNECTED)
    {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(WIFI_SSID);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

        // wait 5 seconds for connection:
        delay(5000);
    }

    Serial.println("Connected to wifi");

    if(hornbill.connect(HOST_ADDRESS,CLIENT_ID)== 0)
    {
        Serial.println("Connected to AWS");
        delay(1000);

        if(0==hornbill.subscribe(TOPIC_NAME,mySubCallBackHandler))
        {
            Serial.println("Subscribe Successfully");
        }
        else
        {
            Serial.println("Subscribe Failed, Check the Thing Name and Certificates");
            while(1);
        }
    }
    else
    {
        Serial.println("AWS connection failed, Check the HOST Address");
        while(1);
    }

    delay(2000);
  
}

void loop()
{
    ////////////////////////////new count
    //主函数
    uint8_t i;
    int count2=0; //次数（一上一下算两次）
    int count=0;
    int ps=0;    //前一个状态
    int cs=0;    //后一个状态
    Serial.println("请开始摇动");
    unsigned long st=millis();    //st为前一刻时间
    while(millis()-st<lt)
    {
      cs=state();
      if(((cs==1)||(cs==-1))&&(cs!=ps))    count2++;
      ps=cs;
      
    }
    count=count2/2;

    sprintf(payload,"%d秒内摇动次数为%d",lt/1000,count);
   
   if(hornbill.publish(TOPIC_NAME,payload) == 0)
        {        
            Serial.println("Publish successfully!");
            Serial.println(payload);
        }
        else
        {
            Serial.println("Publish failed!");
        }
    
    msgReceived = 0;
    vTaskDelay(1000 / portTICK_RATE_MS); 
   
    tick++;
}

    
