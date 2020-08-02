#include "sea_esp32_qspi.h"
#include "SimpleDHT.h"
#include "string.h"

#include "AWS_IOT.h"
#include "WiFi.h"

AWS_IOT hornbill;

char WIFI_SSID[]="TP-LINK_Lin";
char WIFI_PASSWORD[]="linjia123";
char HOST_ADDRESS[]="a145ms7ckbado4-ats.iot.us-west-2.amazonaws.com";
char CLIENT_ID[]= "ESP32client";
char TOPIC_NAME[]= "$aws/things/esp32_lsc/shadow/update";


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


void setup()
{
  
  Serial.begin(115200);
  SeaTrans.begin();
  //SeaTrans.write(0, data1, 4);
  //SeaTrans.read(0, data2, 4);
  //Serial.printf("%d %d %d %d\r\n",data2[0],data2[1],data2[2],data2[3]);
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
    uint8_t i;
    //SeaTrans.write(0, data1, 4);
    /*
    //result in uart
    SeaTrans.read(0, data2, 32);
    for (i=0;i<=31;i=i+2)
    {
      Serial.printf("data2[%d:%d] = %2x%2x \r\n",i+1,i,data2[i+1],data2[i]);
    }
    
    if(msgReceived == 1)
    {
        msgReceived = 0;
        Serial.print("Received Message:");
        Serial.println(rcvdPayload);
    }
    */
    if(tick >= 5)   // publish to topic every 5seconds
    {
        tick=0;
        //result in lot
        SeaTrans.read(0, data2, 16);
        //UART
        /*
        for (i=0;i<=16;i=i+1)
        {
          Serial.printf("%02x ",data2[i]);
        }
        */
        Serial.print("\r\n");
        
        //sprintf(payload,"Hello from hornbill ESP32 : %d",msgCount++);
        //sprintf(payload,"data2[%d:%d] = %2x %2x \r\n",i+1,i,data2[i+1],data2[i]);
        //sprintf(payload," ");
        /*
        for (i=0;i<16;i=i+1)
        {
          payload[i] = data1[i];
        }
        */
        sprintf(payload,"%.2x %.2x - %.2x %.2x %.2x %.2x %.2x %.2x - %.2x %.2x %.2x %.2x %.2x %.2x",data2[2],data2[3],data2[4],data2[5],data2[6],data2[7],data2[8],data2[9],data2[10],data2[11],data2[12],data2[13],data2[14],data2[15]);
        if(hornbill.publish(TOPIC_NAME,payload) == 0)
        {        
            Serial.println("Publish successfully!");
            Serial.println(payload);
        }
        else
        {
            Serial.println("Publish failed!");
        }
    }  
    msgReceived = 0;
    vTaskDelay(1000 / portTICK_RATE_MS); 
    //msgReceived = 0;
    tick++;
}
