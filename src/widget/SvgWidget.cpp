#include <widget/SvgWidget.hpp>
#include <app.hpp>


namespace rack {
namespace widget {

using namespace rack::math;


NVGcolor SvgWidget::getNVGColor(uint32_t color) {
    return nvgRGBA(
            (color >> 0) & 0xff,
            (color >> 8) & 0xff,
            (color >> 16) & 0xff,
            (color >> 24) & 0xff);
}


NVGpaint SvgWidget::getPaint(NVGcontext *vg, NSVGpaint *p) {
    NVGpaint paint;

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
    if (svg == nullptr || svg->handle == nullptr) {
        WARN("%p Invalid svg handle!", this);
        direct = true;
        return;
    }

    this->svg = svg;
    wrap();

    if (svg->handle->width > 1000 || svg->handle->height > 1000) oversample = 2;
    else oversample = 2;

    TRACE("%p set svg: %s oversample is: %dx", this, svg->filename.c_str(), oversample);

    if (svg->fb) {
        TRACE("%p framebuffer already setup for svg: %s.", svg->fb, svg->filename.c_str());
    }

  //  if (svg->handle->width <= 80 || svg->handle->height <= 80) direct = true;

    if (!direct) {
        createFbo();
    } else {
        TRACE("%p using direct draw for svg: %s", svg->filename.c_str());
    }
}


void SvgWidget::draw(const DrawArgs &args) {
    if (!foo && !direct && svg->fb != nullptr && svg->fb->image && svg != nullptr) {
        drawFromFbo(args);
    } else {
        drawDirect(args);
        foo = false;
    }
}


void SvgWidget::drawDirect(const DrawArgs &args) {
    auto start = timer::nanoTime();
    for (auto *shape = svg->handle->shapes; shape; shape = shape->next) {
        drawShape(args.vg, shape);
    }
    drawTime = (double) (timer::nanoTime() - start) / 10e5;
}


void SvgWidget::drawFromFbo(const DrawArgs &args) {
    TRACE("%p rendering svg image via framebuffer for file: %s", this, svg->filename.c_str());

    auto start = timer::nanoTime();

    int fboWidth, fboHeight;
    nvgImageSize(args.vg, svg->fb->image, &fboWidth, &fboHeight);

    if (fboWidth < 4 || fboWidth > 5000 || fboHeight < 4 || fboHeight > 5000) {
        WARN("%p Invalid FBO detected: (%d, %d)", this, fboWidth, fboHeight);

        // switch to direct drawing.
        direct = true;
        return;
    }

    TRACE("%p box=(%f, %f) fbo=(%d, %d)", this, box.size.x, box.size.y, fboWidth, fboHeight);

    if (!svg->fb || !svg->fb->image) {
        WARN("%p invalid FBO!");
        return;
    }

    nvgSave(args.vg);

    nvgBeginPath(args.vg);
    nvgScale(args.vg, 1.f / (float) oversample, 1.f / (float) oversample);
    nvgRect(args.vg, 0, 0, (float) fboWidth, (float) fboHeight);
    auto paint = nvgImagePattern(args.vg, 0, 0, (float) fboWidth, (float) fboHeight, 0.0, svg->fb->image, 1.0);

    nvgFillPaint(args.vg, paint);
    nvgFill(args.vg);

    nvgRestore(args.vg);

    if (drawTime > 0) {
        bufferTime = (double) (timer::nanoTime() - start) / 10e5;
        auto ratio = drawTime / bufferTime;

        if (ratio < 2)
            DEBUG("%p HANDLE TIMING  buffer=%.4f direct=%.4f", this, bufferTime, drawTime);

        if (ratio < 2)
            /* DEBUG("%p ratio: %.5f %s", this, ratio, svg->filename.c_str());
         else*/
            WARN("%p ratio: %.5f %s", this, ratio, svg->filename.c_str());

    }
}


void SvgWidget::drawShape(NVGcontext *vg, NSVGshape *shape) {
    if (svg && svg->handle) {
        if (!visible) return;

        nvgSave(vg);

        nvgGlobalAlpha(vg, shape->opacity);
        nvgBeginPath(vg);

        for (auto *path = shape->paths; path; path = path->next) {
            nvgMoveTo(vg, path->pts[0], path->pts[1]);

            for (auto i = 1; i < path->npts; i += 3) {
                auto *p = &path->pts[2 * i];
                nvgBezierTo(vg, p[0], p[1], p[2], p[3], p[4], p[5]);
            }

            if (path->closed) nvgClosePath(vg);

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

            if (crossings % 2 == 0) nvgPathWinding(vg, NVG_SOLID);
            else nvgPathWinding(vg, NVG_HOLE);

        }

        if (shape->fill.type) {
            switch (shape->fill.type) {
                case NSVG_PAINT_COLOR: {
                    auto color = SvgWidget::getNVGColor(shape->fill.color);
                    nvgFillColor(vg, color);
                }
                    break;
                case NSVG_PAINT_LINEAR_GRADIENT:
                case NSVG_PAINT_RADIAL_GRADIENT: {
                    nvgFillPaint(vg, SvgWidget::getPaint(vg, &shape->fill));
                }
                    break;
            }

            nvgFill(vg);
        }

        if (shape->stroke.type) {
            nvgStrokeWidth(vg, shape->strokeWidth);
            nvgLineCap(vg, (NVGlineCap) shape->strokeLineCap);
            nvgLineJoin(vg, (int) shape->strokeLineJoin);

            switch (shape->stroke.type) {
                case NSVG_PAINT_COLOR: {
                    auto color = SvgWidget::getNVGColor(shape->stroke.color);
                    nvgStrokeColor(vg, color);
                    break;
                }
                case NSVG_PAINT_LINEAR_GRADIENT:
                case NSVG_PAINT_RADIAL_GRADIENT: {
                    nvgStrokePaint(vg, SvgWidget::getPaint(vg, &shape->stroke));
                    break;
                }
            }
            nvgStroke(vg);
        }

        nvgRestore(vg);
    }
}


void SvgWidget::createFbo() {
    if (svg == nullptr || svg->handle == nullptr || svg->fb) return; //skip

    //  if (fb != nullptr) nvgluDeleteFramebuffer(fb);


    auto vg = APP->window->vg;
    int winWidth, winHeight;
    int fbWidth, fbHeight;
    float pxRatio;

    glfwGetWindowSize(APP->window->win, &winWidth, &winHeight);
    glfwGetFramebufferSize(APP->window->win, &fbWidth, &fbHeight);

    // Calculate pixel ration for hi-dpi devices.
    pxRatio = (float) fbWidth / (float) winWidth *  (float) oversample;

    // if (fb == nullptr) {
    TRACE("%p creating framebuffer for: %s", this, svg->filename.c_str());

    if (!svg || !svg->handle || svg->handle->width <= 0 | svg->handle->height <= 0) {
        WARN("%p invalid svg: %s dimensions=(%d, %d)", this, svg->filename.c_str(), svg->handle ? svg->handle->width : 0, svg->handle ? svg->handle->height : 0);
        return;
    }

    TRACE("%p SVG=(%f, %f) box=(%f, %f)", this, svg->handle->width, svg->handle->height, box.size.x, box.size.y);
    TRACE("%p win framebuffer: fbo=(%d, %d) win=(%d, %d) r=1:%d", this, fbWidth, fbHeight, winWidth, winHeight, pxRatio);

    int fboWidth = (int) ceil((box.size.x * pxRatio));
    int fboHeight = (int) ceil((box.size.y * pxRatio));

    svg->fb = nvgluCreateFramebuffer(vg, fboWidth, fboHeight, 0);

    if (!svg->fb) {
        WARN("%p unable to create framebuffer for svg: %s", this, svg->filename.c_str());
        return;
    }

    nvgluBindFramebuffer(svg->fb);

    glViewport(0, 0, fboWidth, fboHeight);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    nvgBeginFrame(vg, box.size.x, box.size.y, pxRatio);
    nvgBeginPath(vg);

    // auto start = timer::nanoTime();
    for (auto *shape = svg->handle->shapes; shape; shape = shape->next) {
        drawShape(vg, shape);
    }
    //  drawTime = (double) (timer::nanoTime() - start) / 10e5;

    // DEBUG("%p draw-time for svg: %s %.4f", this, svg->filename.c_str(), drawTime * 10e3);

    nvgEndFrame(vg);
    nvgReset(vg);

    nvgluBindFramebuffer(nullptr);
    // }
}


} // namespace widget
} // namespace rack
