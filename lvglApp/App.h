#ifndef _App_
#define _App_
#include <lvgl.h>

static const char *on = "ON";
static const char *off = "OFF";

class App;

void log() {
}

class App {
public:
    typedef enum FivePointEvent {
        UpEvent = 0,
        DownEvent,
        LeftEvent,
        RightEvent,
        PressEvent,
        ReleaseEvent
    };
    static App * GetInstance() {
        if (_app == NULL) {
            _app = new App();
        }
        return _app;
    }

    App() :
        _y(120),
        _x(160),
        _pressed(false),
        _sensitivity(2),
        _isOn(false) {
        _root = lv_win_create(lv_scr_act(), NULL);
        lv_win_set_title(_root, "Window");

        _button = lv_btn_create(lv_scr_act(), NULL);
        lv_obj_set_event_cb(_button, eventHandler);
        lv_obj_align(_button, NULL, LV_ALIGN_CENTER, 0, -40);
        lv_btn_set_checkable(_button, true);

        _label = lv_label_create(_button, NULL);
        lv_label_set_text(_label, getText());

        // WIO's "mouse" can be the little joystick
        lv_indev_drv_init(&_indev_drv);
        _indev_drv.type = LV_INDEV_TYPE_POINTER;
        _indev_reg = lv_indev_drv_register(&_indev_drv);
    }

    static void fivePointHandler(lv_indev_drv_t *drv, lv_indev_data_t *data) {
        App *app = GetInstance();
        data->point.x = app->_x;
        data->point.y = app->_y;
    }

    static void eventHandler(lv_obj_t *obj, lv_event_t event) {
        App *app = GetInstance();
        lv_label_set_text(app->_label, app->getText());
    }

    void updateFivePoint(FivePointEvent fpEvent) {
        
        switch (fpEvent) {
        case UpEvent:
            _y += _sensitivity;
            break;
        case DownEvent:
            _y -= _sensitivity;
            break;
        case LeftEvent:
            _x += _sensitivity;
            break;
        case RightEvent:
            _x -= _sensitivity;
            break;
        case PressEvent:
            _pressed = true;
            break;
        default:
            _pressed = false;
        }
    }

    const char *getText() {
        _isOn = !_isOn;
        return _isOn ? on : off;
    }

    // singleton because callbacks need to use C funcion pointers. AKA static
    // functions of this class.
    static inline App *_app;

    int _y;
    int _x;
    bool _pressed;
    int _sensitivity;
    bool _isOn;
    lv_obj_t *_root;
    lv_obj_t *_label;
    lv_obj_t *_button;
    lv_indev_drv_t _indev_drv;
    lv_indev_t *_indev_reg;
 
};

#endif // _App_
