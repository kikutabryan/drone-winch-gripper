void receiverInitialize(void)
{
  pinMode(channelPins[0], INPUT);
  pinMode(channelPins[1], INPUT);
  pinMode(channelPins[2], INPUT);
  pinMode(channelPins[3], INPUT);
//  pinMode(channelPins[4], INPUT);
//  pinMode(channelPins[5], INPUT);

  attachPCINT(digitalPinToPCINT(channelPins[0]), receiverCh1, CHANGE);
  attachPCINT(digitalPinToPCINT(channelPins[1]), receiverCh2, CHANGE);
  attachPCINT(digitalPinToPCINT(channelPins[2]), receiverCh3, CHANGE);
  attachPCINT(digitalPinToPCINT(channelPins[3]), receiverCh4, CHANGE);
//  attachPCINT(digitalPinToPCINT(channelPins[4]), receiverCh5, CHANGE);
//  attachPCINT(digitalPinToPCINT(channelPins[5]), receiverCh6, CHANGE);
}

//channel 1
void receiverCh1(void)
{
  receiverNow[0] = micros();

  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(channelPins[0]));
  if(trigger == RISING)
    receiverPast[0] = receiverNow[0];
  else
    receiverInput[0] = (receiverNow[0] - receiverPast[0]);
    receiverUpdateTime[0] = millis();
}

//channel 2
void receiverCh2(void)
{
  receiverNow[1] = micros();

  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(channelPins[1]));
  if(trigger == RISING)
    receiverPast[1] = receiverNow[1];
  else
    receiverInput[1] = (receiverNow[1] - receiverPast[1]);
    receiverUpdateTime[1] = millis();
}

//channel 3
void receiverCh3(void)
{
  receiverNow[2] = micros();

  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(channelPins[2]));
  if(trigger == RISING)
    receiverPast[2] = receiverNow[2];
  else
    receiverInput[2] = (receiverNow[2] - receiverPast[2]);
    receiverUpdateTime[2] = millis();
}

//channel 4
void receiverCh4(void)
{
  receiverNow[3] = micros();

  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(channelPins[3]));
  if(trigger == RISING)
    receiverPast[3] = receiverNow[3];
  else
    receiverInput[3] = (receiverNow[3] - receiverPast[3]);
    receiverUpdateTime[3] = millis();
}

////channel 5
//void receiverCh5(void)
//{
//  receiverNow[4] = micros();
//
//  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(channelPins[4]));
//  if(trigger == RISING)
//    receiverPast[4] = receiverNow[4];
//  else
//    receiverInput[4] = (receiverNow[4] - receiverPast[4]);
//    receiverUpdateTime[4] = millis();
//}
//
////channel 6
//void receiverCh6(void)
//{
//  receiverNow[5] = micros();
//
//  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(channelPins[5]));
//  if(trigger == RISING)
//    receiverPast[5] = receiverNow[5];
//  else
//    receiverInput[5] = (receiverNow[5] - receiverPast[5]);
//    receiverUpdateTime[5] = millis();
//}
