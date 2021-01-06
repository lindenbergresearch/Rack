#include <ui.hpp>


namespace rack {
namespace ui {


void init() {
    setTheme(nvgRGBAf(0.1, 0.1, 0.1, 0.942), nvgRGBAf(0.95, 0.95, 0.95, 0.982));
}


void destroy() {
}


void setTheme(NVGcolor bg, NVGcolor fg) {
    // Assume dark background and light foreground

    BNDwidgetTheme w;
    w.outlineColor = nvgRGBAf(0.5, 0.5, 0.5, 0.3);
    w.itemColor = nvgRGBAf(0.9, 0.9, 0.9, 1.0);
    w.innerColor = bg;
    w.innerSelectedColor = nvgRGBAf(0.15, 0.15, 0.15, 0.982);
    w.textColor = nvgRGBAf(0.9, 0.9, 0.9, 1.0);
    w.textSelectedColor = fg;
    w.shadeTop = 0;
    w.shadeDown = 0;

    BNDtheme t;
    t.backgroundColor = bg;
    t.regularTheme = w;
    t.toolTheme = w;
    t.radioTheme = w;
    t.textFieldTheme = w;
    t.optionTheme = w;
    t.choiceTheme = w;
    t.numberFieldTheme = w;
    t.sliderTheme = w;
    t.scrollBarTheme = w;
    t.tooltipTheme = w;
    t.menuTheme = w;
    t.menuItemTheme = w;

    t.sliderTheme.itemColor = nvgRGBAf(0.106, 0.4722, 1, 0.75);
    t.sliderTheme.innerColor = color::plus(bg, nvgRGB(0x20, 0x20, 0x20));
    t.sliderTheme.innerSelectedColor = color::plus(bg, nvgRGB(0x30, 0x30, 0x30));

    t.textFieldTheme = t.sliderTheme;
    t.textFieldTheme.textColor = nvgRGBAf(0.9, 0.1, 0.1, 1.0);
    t.textFieldTheme.textSelectedColor = t.textFieldTheme.textColor;
    t.textFieldTheme.itemColor = color::plus(bg, nvgRGB(0x20, 0x20, 0x20));

    t.scrollBarTheme.itemColor = nvgRGBAf(0.102, 0.202, 0.552, 0.4);
    t.scrollBarTheme.innerSelectedColor = nvgRGBAf(0.602, 0.202, 0.552, 0.9);
    t.scrollBarTheme.outlineColor = nvgRGBAf(0, 0, 0, 0);
    t.scrollBarTheme.innerColor = nvgRGBAf(0.314, 0.314, 0.314, 0.106);
    t.scrollBarTheme.shadeTop = 3;
    t.scrollBarTheme.shadeDown = -3;

    t.menuTheme.innerColor = bg;
    t.menuTheme.innerSelectedColor = nvgRGBAf(0., 0., 0., 0.83);
    t.menuTheme.outlineColor = nvgRGBAf(0.5, 0.5, 0.5, 0.3);
    t.menuTheme.textColor = nvgRGBAf(0.95, 0.95, 0.95, 0.982);
    t.menuTheme.textSelectedColor = t.menuTheme.textColor;

    t.menuItemTheme.shadeDown = 10;
    t.menuItemTheme.shadeTop = -10;
    t.menuItemTheme.innerSelectedColor = nvgRGBAf(0.106, 0.4722, 1, 0.75);
    //t.menuItemTheme.innerColor = nvgRGBAf(0.102, 0.202, 0.502, 0.7);

    bndSetTheme(t);
}


} // namespace ui
} // namespace rack
