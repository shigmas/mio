#include <lvgl.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI(); /* TFT instance */
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];


lv_disp_drv_t disp_drv;

#define LVGL_TICK_PERIOD 5

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint16_t c;

  tft.startWrite(); /* Start new TFT transaction */
  tft.setAddrWindow(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1)); /* set the working window */
  for (int y = area->y1; y <= area->y2; y++) {
    for (int x = area->x1; x <= area->x2; x++) {
      c = color_p->full;
      tft.writeColor(c, 1);
      color_p++;
    }
  }
  tft.endWrite(); /* terminate TFT transaction */
  lv_disp_flush_ready(disp); /* tell lvgl that flushing is done */
}

/* Interrupt driven periodic handler */
static void lv_tick_handler(void)
{

  lv_tick_inc(LVGL_TICK_PERIOD);
}

static const char *on = "ON";
static const char *off = "OFF";

void eventHandler(lv_obj_t *obj, lv_event_t event);

class App {
  public:
 App() : _isOn(false) {
      _root = lv_win_create(lv_scr_act(), NULL);
      lv_win_set_title(_root, "Window");

      _button = lv_btn_create(lv_scr_act(), NULL);
      lv_obj_set_event_cb(_button, eventHandler);
      lv_obj_align(_button, NULL, LV_ALIGN_CENTER, 0, -40);
      lv_btn_set_checkable(_button, true);

      _label = lv_label_create(_button, NULL);
      lv_label_set_text(_label, getText());
    }

    const char *getText() {
      _isOn = !_isOn;
      return _isOn ? on : off;
    }
    
    bool _isOn;
    lv_obj_t *_root;
    lv_obj_t *_label;
    lv_obj_t *_button;
};

App *app;

void eventHandler(lv_obj_t *obj, lv_event_t event) {
  lv_label_set_text(app->_label,app->getText());
}



void setup() {
  lv_init();
  tft.begin();
  tft.setRotation(3);
  lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);

  /*Initialize the display*/
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 320;
  disp_drv.ver_res = 240;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.buffer = &disp_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the touch pad*/
  /*
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = read_encoder;
    lv_indev_drv_register(&indev_drv);
  */
  // put your setup code here, to run once:
  app = new App();
}

void loop() {
  // put your main code here, to run repeatedly:
  lv_task_handler(); /* let the GUI do its work */
  delay(LVGL_TICK_PERIOD);
}
