#pragma once

#include <widget/Widget.hpp>
#include <svg.hpp>


namespace rack {
namespace widget {

using namespace rack::math;


/** Draws an Svg */
struct SvgWidget : Widget {
    std::shared_ptr<Svg> svg;


    /** Sets the box size to the svg image size */
    void wrap();

    /** Sets and wraps the SVG */
    void setSvg(std::shared_ptr<Svg> svg);


    DEPRECATED void setSVG(std::shared_ptr<Svg> svg) {
        setSvg(svg);
    }


    static NVGcolor getNVGColor(uint32_t color);
    static NVGpaint getPaint(NVGcontext *vg, NSVGpaint *p);
    static float getLineCrossing(Vec p0, Vec p1, Vec p2, Vec p3);

    void drawShape(const DrawArgs &args, NSVGshape *shape);


    inline void draw(const DrawArgs &args) override {
        for (auto *shape = svg->handle->shapes; shape; shape = shape->next) {
            drawShape(args, shape);
        }
    }
};


DEPRECATED typedef SvgWidget SVGWidget;


} // namespace widget
} // namespace rack
