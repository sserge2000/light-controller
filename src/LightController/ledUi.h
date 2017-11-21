#define LED_UI_PIN 12
#define LED_UI_OFF 0
#define LED_UI_ON 1
#define LED_UI_SLOW_BLINK 2
#define LED_UI_FAST_BLINK 3

#define LED_UI_SLOW_BLINK_BLINK_PERIOD 500
#define LED_UI_FAST_BLINK_BLINK_PERIOD 150

BlynkTimer* timer_h;

int ledUiState;
int ledUiBlinkPeriod;
int ledUiTimerID = -1;

void ledUiTimerTask()
{
  digitalWrite(LED_UI_PIN, !digitalRead(LED_UI_PIN));
}

void initLedUi(BlynkTimer* timer)
{
  timer_h = timer;
  ledUiState = LED_UI_OFF;
  pinMode(LED_UI_PIN, OUTPUT);
  digitalWrite(LED_UI_PIN, LOW);
}

void setLedUiState(int state)
{
 if (ledUiState == state )
    return;
 ledUiState = state;    
 if (ledUiTimerID >= 0)
    timer_h->deleteTimer(ledUiTimerID);
 switch (ledUiState)
 {
    case LED_UI_OFF:
      digitalWrite(LED_UI_PIN, LOW);
      break;
    case LED_UI_ON:
      digitalWrite(LED_UI_PIN, HIGH);
      break;
    case LED_UI_SLOW_BLINK:
      digitalWrite(LED_UI_PIN, HIGH);
      ledUiTimerID = timer_h->setInterval(LED_UI_SLOW_BLINK_BLINK_PERIOD, ledUiTimerTask);
      break;
    case LED_UI_FAST_BLINK:
      digitalWrite(LED_UI_PIN, HIGH);
      ledUiTimerID = timer_h->setInterval(LED_UI_FAST_BLINK_BLINK_PERIOD, ledUiTimerTask);
      break;
 }
}

