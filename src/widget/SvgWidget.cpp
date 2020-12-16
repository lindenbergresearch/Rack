#include <widget/SvgWidget.hpp>
#include <app.hpp>


namespace rack {
namespace widget {


using namespace rack::math;


/**
 * @brief Converts a 32bit encoded color to RGBA
 * @param color
 * @return
 */
NVGcolor SvgWidget::getNVGColor(uint32_t color) {
    return nvgRGBA(
            (color >> 0) & 0xff,
            (color >> 8) & 0xff,
            (color >> 16) & 0xff,
            (color >> 24) & 0xff);
}


/**
 * @brief Prepare complex paint with gradients
 * @param vg
 * @param p
 * @return
 */
NVGpaint SvgWidget::getPaint(NVGcontext *vg, NSVGpaint *p) {
    NVGpaint paint;
    //save++;

    // check for supported gradients
    assert(p->type == NSVG_PAINT_LINEAR_GRADIENT || p->type == NSVG_PAINT_RADIAL_GRADIENT);

    // a gradient need's at least 2 stops
    assert(p->gradient->nstops >= 1);

    auto g = p->gradient;
    auto startGradientStop = g->stops[0];
    auto endGradientStop = g->stops[g->nstops - 1];

    auto startOffset = startGradientStop.offset;
    auto endOffset = endGradientStop.offset;

    NVGcolor innerColor = getNVGColor(startGradientStop.color);
    NVGcolor outerColor = getNVGColor(endGradientStop.color);

    float inverse[6];
    //if (save < 3) printf("before: scale(%f, %f) skew(%f, %f) transform(%f, %f)\n", g->xform[0], g->xform[3], g->xform[1], g->xform[2], g->xform[4], g->xform[5]);
    nvgTransformInverse(inverse, g->xform);
    //if (save < 3) printf("after : scale(%f, %f) skew(%f, %f) transform(%f, %f)\n", inverse[0], inverse[3], inverse[1], inverse[2], inverse[4], inverse[5]);

    Vec s, e;
    //printf("A sx: %f sy: %f ex: %f ey: %f\n", s.x, s.y, e.x, e.y);
    // Is it always the case that the gradient should be transformed from (0, 0) to (0, 1)?
    nvgTransformPoint(&s.x, &s.y, inverse, 0, startOffset);
    nvgTransformPoint(&e.x, &e.y, inverse, 0, endOffset);


    //if (save < 3) printf("A S(%f, %f) E(%f, %f)\n", s.x, s.y, e.x, e.y);
    //if (save < 3) printf("B fx(%f, %f) offs(%f, %f)\n---\n\n", g->fx, g->fy, startOffset, endOffset);

    if (p->type == NSVG_PAINT_LINEAR_GRADIENT)
        paint = nvgLinearGradient(vg, s.x, s.y, e.x, e.y, innerColor, outerColor);
    else
        paint = nvgRadialGradient(vg, s.x, s.y, 0.0, 160, innerColor, outerColor);

    return paint;
}


/**
 * @brief Returns the parameterized value of the line p2--p3 where it intersects with p0--p1
 * @param p0
 * @param p1
 * @param p2
 * @param p3
 * @return
 */
float SvgWidget::getLineCrossing(Vec p0, Vec p1, Vec p2, Vec p3) {
    auto b = p2.minus(p0);
    auto d = p1.minus(p0);
    auto e = p3.minus(p2);

    auto m = d.x * e.y - d.y * e.x;

    // Check if lines are parallel, or if either pair of points are equal
    if (std::abs(m) < 1e-6)
        return NAN;

    return -(d.x * b.y - d.y * b.x) / m;
}


void SvgWidget::wrap() {
    if (svg && svg->handle) {
        box.size = math::Vec(svg->handle->width, svg->handle->height);
    } else {
        box.size = math::Vec();
    }
}


void SvgWidget::setSvg(std::shared_ptr<Svg> svg) {
    this->svg = svg;
    shape = svg->handle->shapes;

    wrap();
}


void SvgWidget::draw(const DrawArgs &args) {
    if (svg && svg->handle) {
        if (!visible) return;

        nvgSave(args.vg);

        nvgGlobalAlpha(args.vg, shape->opacity);
        nvgBeginPath(args.vg);

        for (auto *path = shape->paths; path; path = path->next) {
            nvgMoveTo(args.vg, path->pts[0], path->pts[1]);

            for (auto i = 1; i < path->npts; i += 3) {
                auto *p = &path->pts[2 * i];
                nvgBezierTo(args.vg, p[0], p[1], p[2], p[3], p[4], p[5]);
            }

            if (path->closed) nvgClosePath(args.vg);

            auto crossings = 0;
            auto p0 = Vec(path->pts[0], path->pts[1]);
            auto p1 = Vec(path->bounds[0] - 1.0f, path->bounds[1] - 1.0f);

            for (auto *path2 = shape->paths; path2; path2 = path2->next) {
                if (path2 == path) continue;
                if (path2->npts < 4) continue;

                for (auto i = 1; i < path2->npts + 3; i += 3) {
                    auto *p = &path2->pts[2 * i];
                    auto p2 = Vec(p[-2], p[-1]);
                    auto p3 = (i < path2->npts) ? Vec(p[4], p[5]) : Vec(path2->pts[0], path2->pts[1]);
                    auto crossing = SvgWidget::getLineCrossing(p0, p1, p2, p3);
                    auto crossing2 = SvgWidget::getLineCrossing(p2, p3, p0, p1);

                    if (0.0 <= crossing && crossing < 1.0 && 0.0 <= crossing2) crossings++;

                }
            }

            if (crossings % 2 == 0) nvgPathWinding(args.vg, NVG_SOLID);
            else nvgPathWinding(args.vg, NVG_HOLE);

        }

        if (shape->fill.type) {
            switch (shape->fill.type) {
                case NSVG_PAINT_COLOR: {
                    auto color = SvgWidget::getNVGColor(shape->fill.color);
                    nvgFillColor(args.vg, color);
                }
                    break;
                case NSVG_PAINT_LINEAR_GRADIENT:
                case NSVG_PAINT_RADIAL_GRADIENT: {
                    nvgFillPaint(args.vg, SvgWidget::getPaint(args.vg, &shape->fill));
                }
                    break;
            }

            nvgFill(args.vg);
        }

        if (shape->stroke.type) {
            nvgStrokeWidth(args.vg, shape->strokeWidth);
            nvgLineCap(args.vg, (NVGlineCap) shape->strokeLineCap);
            nvgLineJoin(args.vg, (int) shape->strokeLineJoin);

            switch (shape->stroke.type) {
                case NSVG_PAINT_COLOR: {
                    auto color = SvgWidget::getNVGColor(shape->stroke.color);
                    nvgStrokeColor(args.vg, color);
                    break;
                }
                case NSVG_PAINT_LINEAR_GRADIENT:
                case NSVG_PAINT_RADIAL_GRADIENT: {
                    nvgStrokePaint(args.vg, SvgWidget::getPaint(args.vg, &shape->stroke));
                    break;
                }
            }
            nvgStroke(args.vg);
        }

        nvgRestore(args.vg);
    }
}


} // namespace widget
} // namespace rack
