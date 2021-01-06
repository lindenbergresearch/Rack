#pragma once

#include <widget/Widget.hpp>
#include <svg.hpp>


namespace rack {
namespace widget {

using namespace rack::math;


/**
 * @brief Framebuffered Svg rendering widget.
 */
struct SvgWidget : Widget {
    std::shared_ptr<Svg> svg;bool foo = true;

    /** Trigger recreation of framebuffer */
    bool dirty = false;

    /** Determine if draw via framebuffer or direct */
    bool direct = false;

    double drawTime = -1;
    double bufferTime = -1;

    unsigned int oversample = 2;
    /**
     * @brief Sets the box size to the svg image size.
     */
    void wrap();

    /**
     * @brief Sets and wraps the SVG.
     * @param svg
     */
    void setSvg(std::shared_ptr<Svg> svg);

    /**
     * @brief Renders a svg shape.
     * @param vg
     * @param shape NSVGshape from nonosvg loader.
     */
    void drawShape(NVGcontext *vg, NSVGshape *shape);

    /**
     * @brief Draw svg.
     * @param args
     */
    void draw(const DrawArgs &args) override;

    /**
     * @brief Draw svg direct using svg vector data.
     * @param args
     */
    void drawDirect(const DrawArgs &args);

    /**
     * @brief Draw svg via prepared FBO.
     * @param args
     */
    void drawFromFbo(const DrawArgs &args);

    /**
     * @brief Create the FBO.
     */
    void createFbo();

    /**
     * @brief Converts a 32bit encoded color to RGBA.
     * @param color
     * @return
     */
    static NVGcolor getNVGColor(uint32_t color);

    /**
     * @brief Prepare complex paints using gradients
     * @param vg
     * @param p
     * @return
     */
    static NVGpaint getPaint(NVGcontext *vg, NSVGpaint *p);

    /**
     * @brief Returns the parameterized value of the line p2--p3 where it intersects with p0--p1
     * @param p0
     * @param p1
     * @param p2
     * @param p3
     * @return
     */
    static float getLineCrossing(Vec p0, Vec p1, Vec p2, Vec p3);


    DEPRECATED void setSVG(std::shared_ptr<Svg> svg) {
        setSvg(svg);
    }
};


DEPRECATED typedef SvgWidget SVGWidget;


} // namespace widget
} // namespace rack
