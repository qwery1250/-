/*取得重新啟動的理由:
      5為睡眠喚醒
      3 軟體看門狗
      6RST按鈕啟動
      0上電啟動*/
int rst_reason(){
  rst_info *Rst_Info;    
  Rst_Info = ESP.getResetInfoPtr();   
  return (*Rst_Info).reason;
}
int Get_Tick(){
  byte data = 0;
  if(rst_reason()==5){//如果是睡眠喚醒
    system_rtc_mem_read(70,&data,sizeof(data));
    data++;
    system_rtc_mem_write(70, &data,sizeof(data)); 
    Serial.println("wake");
  }else{              //如果是第一次啟動
    data = 5;
    system_rtc_mem_write(70,&data,sizeof(data)); //offset is 65
    Serial.println("Start up");
  }
  return data;
}
void Set_Tick(byte data){
  system_rtc_mem_write(70, &data,sizeof(data));
}
