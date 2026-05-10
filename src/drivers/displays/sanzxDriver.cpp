#include "displayDriver.h"

#ifdef SANZX_DISPLAY

#include <TFT_eSPI.h>
#include "media/images_320_170.h"
#include "media/myFonts.h"
#include "media/Free_Fonts.h"
#include "version.h"
#include "monitor.h"
#include "OpenFontRender.h"
#include "rotation.h"

#define WIDTH 320
#define HEIGHT 170

OpenFontRender render;
TFT_eSPI tft = TFT_eSPI();                  // Invoke library, pins defined in platformio.ini
TFT_eSprite background = TFT_eSprite(&tft); // Invoke library sprite

extern monitor_data mMonitor;

void sanzx_Init(void)
{
  Serial.println("[SANZX] Initializing Display...");
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  tft.init();
  tft.setRotation(1); // Set to landscape (320x170)
  tft.setSwapBytes(true);

  background.createSprite(320, 170);
  background.setSwapBytes(true);
  render.setDrawer(background);
  render.setLineSpaceRatio(0.9);

  if (render.loadFont(DigitalNumbers, sizeof(DigitalNumbers)))
  {
    Serial.println("[SANZX] Initialise error: Font not loaded");
  }

  tft.fillScreen(TFT_BLACK);
  Serial.println("[SANZX] Display Initialized successfully");
}

void sanzx_AlternateScreenState(void)
{
  int screen_state = digitalRead(TFT_BL);
  digitalWrite(TFT_BL, !screen_state);
}

void sanzx_AlternateRotation(void)
{
  tft.setRotation( flipRotation(tft.getRotation()) );
}

void sanzx_MinerScreen(unsigned long mElapsed)
{
  mining_data data = getMiningData(mElapsed);

  background.pushImage(0, 0, MinerWidth, MinerHeight, MinerScreen);

  render.setFontSize(35);
  render.rdrawString(data.currentHashRate.c_str(), 118, 114, TFT_BLACK);
  render.setFontSize(18);
  render.rdrawString(data.totalMHashes.c_str(), 268, 138, TFT_BLACK);
  render.setFontSize(18);
  render.drawString(data.templates.c_str(), 186, 20, 0xDEDB);
  render.drawString(data.bestDiff.c_str(), 186, 48, 0xDEDB);
  render.setFontSize(18);
  render.drawString(data.completedShares.c_str(), 186, 76, 0xDEDB);
  render.setFontSize(14);
  render.rdrawString(data.timeMining.c_str(), 315, 104, 0xDEDB);
  render.setFontSize(24);
  render.drawString(data.valids.c_str(), 285, 56, 0xDEDB);
  render.setFontSize(10);
  render.rdrawString(data.temp.c_str(), 239, 1, TFT_BLACK);
  render.setFontSize(4);
  render.rdrawString(String(0).c_str(), 244, 3, TFT_BLACK);
  render.setFontSize(10);
  render.rdrawString(data.currentTime.c_str(), 286, 1, TFT_BLACK);

  background.pushSprite(0, 0);
}

void sanzx_ClockScreen(unsigned long mElapsed)
{
  clock_data data = getClockData(mElapsed);

  background.pushImage(0, 0, minerClockWidth, minerClockHeight, minerClockScreen);

  render.setFontSize(25);
  render.rdrawString(data.currentHashRate.c_str(), 94, 129, TFT_BLACK);

  background.setFreeFont(FSSB9);
  background.setTextDatum(TL_DATUM);
  background.setTextColor(TFT_BLACK);
  background.drawString(data.btcPrice.c_str(), 202, 3, GFXFF);

  render.setFontSize(18);
  render.rdrawString(data.blockHeight.c_str(), 254, 140, TFT_BLACK);

  background.setFreeFont(FF23);
  background.setTextSize(2);
  background.setTextColor(0xDEDB, TFT_BLACK);
  background.drawString(data.currentTime.c_str(), 130, 50, GFXFF);

  background.pushSprite(0, 0);
}

void sanzx_GlobalHashScreen(unsigned long mElapsed)
{
  coin_data data = getCoinData(mElapsed);

  background.pushImage(0, 0, globalHashWidth, globalHashHeight, globalHashScreen);

  background.setFreeFont(FSSB9);
  background.setTextDatum(TL_DATUM);
  background.setTextColor(TFT_BLACK);
  background.drawString(data.btcPrice.c_str(), 198, 3, GFXFF);
  background.drawString(data.currentTime.c_str(), 268, 3, GFXFF);

  background.setFreeFont(FSS9);
  background.setTextDatum(TR_DATUM);
  background.setTextColor(0x9C92);
  background.drawString(data.halfHourFee.c_str(), 302, 52, GFXFF);
  background.drawString(data.netwrokDifficulty.c_str(), 302, 88, GFXFF);

  render.setFontSize(17);
  render.rdrawString(data.globalHashRate.c_str(), 274, 145, TFT_BLACK);
  render.setFontSize(28);
  render.rdrawString(data.blockHeight.c_str(), 140, 104, 0xDEDB);

  int x2 = 2 + (138 * data.progressPercent / 100);
  background.fillRect(2, 149, x2, 168, 0xDEDB);

  background.setTextFont(FONT2);
  background.setTextSize(1);
  background.setTextDatum(MC_DATUM);
  background.setTextColor(TFT_BLACK);
  background.drawString(data.remainingBlocks.c_str(), 72, 159, FONT2);

  background.pushSprite(0, 0);
}

void sanzx_BTCprice(unsigned long mElapsed)
{
  clock_data data = getClockData(mElapsed);

  background.pushImage(0, 0, priceScreenWidth, priceScreenHeight, priceScreen);

  render.setFontSize(25);
  render.rdrawString(data.currentHashRate.c_str(), 94, 129, TFT_BLACK);
  render.setFontSize(18);
  render.rdrawString(data.blockHeight.c_str(), 254, 138, TFT_WHITE);

  background.setFreeFont(FSSB9);
  background.setTextDatum(TL_DATUM);
  background.setTextColor(TFT_BLACK);
  background.drawString(data.currentTime.c_str(), 222, 3, GFXFF);

  background.setFreeFont(FF24);
  background.setTextDatum(TR_DATUM);
  background.setTextColor(0xDEDB, TFT_BLACK);
  background.drawString(data.btcPrice.c_str(), 300, 58, GFXFF);

  background.pushSprite(0, 0);
}

void sanzx_LoadingScreen(void)
{
  tft.fillScreen(TFT_BLACK);
  tft.pushImage(0, 0, initWidth, initHeight, initScreen);
  tft.setTextColor(TFT_BLACK);
  tft.drawString(CURRENT_VERSION, 24, 147, FONT2);
}

void sanzx_SetupScreen(void)
{
  tft.pushImage(0, 0, setupModeWidth, setupModeHeight, setupModeScreen);
}

void sanzx_AnimateCurrentScreen(unsigned long frame) {}
void sanzx_DoLedStuff(unsigned long frame) {}

CyclicScreenFunction sanzxCyclicScreens[] = {sanzx_MinerScreen, sanzx_ClockScreen, sanzx_GlobalHashScreen, sanzx_BTCprice};

DisplayDriver sanzxDisplayDriver = {
    sanzx_Init,
    sanzx_AlternateScreenState,
    sanzx_AlternateRotation,
    sanzx_LoadingScreen,
    sanzx_SetupScreen,
    sanzxCyclicScreens,
    sanzx_AnimateCurrentScreen,
    sanzx_DoLedStuff,
    SCREENS_ARRAY_SIZE(sanzxCyclicScreens),
    0,
    320,
    170};
#endif
