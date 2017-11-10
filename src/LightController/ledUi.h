#define LED_UI_PIN 12
#define LED_UI_OFF 0
#define LED_UI_OK 1
#define LED_UI_CONNECTING 2
#define LED_UI_ERROR 3

#define LED_UI_CONNECTING_BLINK_PERIOD 500
#define LED_UI_ERROR_BLINK_PERIOD 250

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
    case LED_UI_OK:
      digitalWrite(LED_UI_PIN, HIGH);
      break;
    case LED_UI_CONNECTING:
      digitalWrite(LED_UI_PIN, HIGH);
      ledUiTimerID = timer_h->setInterval(LED_UI_CONNECTING_BLINK_PERIOD, ledUiTimerTask);
      break;
    case LED_UI_ERROR:
      digitalWrite(LED_UI_PIN, HIGH);
      ledUiTimerID = timer_h->setInterval(LED_UI_ERROR_BLINK_PERIOD, ledUiTimerTask);
      break;
 }
}

