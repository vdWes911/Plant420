void setup()
{
  
}

void loop()
{
  
}
// THE CLASS WILL NEED TO BE WRITTEN IN C, WITH THE HEADER FILE DEFINING THE CLASS AND THE CPP FILE IMPLEMENTING THE CLASS. iNCLUDE ONLY THE HEADER FILE INTO MAIN PROJCET THEN.    GOOGLE ARDUINO CREATING A CLASS
#ifndef I2CThing_h
#define I2CThing_h
  class I2CThing
  {
    private:
      int SDA;
      int SCL;
  
    public:
      I2CThing();
      void SendSetup();
      float ReceiveTemp();
  };
  
  I2CThing::I2CThing()
  {
    //SDA = 12;
  }
  
  float I2CThing::ReceiveTemp()
  {
  
    return 0;
  }
#endif
