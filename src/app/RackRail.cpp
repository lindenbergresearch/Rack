#include <app/RackRail.hpp>
#include <widget/SvgWidget.hpp>
#include <widget/FramebufferWidget.hpp>
#include <app.hpp>
#include <asset.hpp>
#include <svg.hpp>


namespace rack {
namespace app {

using namespace rack::widget;


RackRail::RackRail() {
    busBoard = new SvgWidget;
    busBoard->setSvg(APP->window->loadSvg(asset::system("res/ComponentLibrary/RackBusboard.svg")));

    busBoard2 = new SvgWidget;
    busBoard2->setSvg(APP->window->loadSvg(asset::system("res/ComponentLibrary/RackBusboard2.svg")));

    woodenbg = new SvgWidget;
    woodenbg->setSvg(APP->window->loadSvg(asset::system("res/ComponentLibrary/RackWoodenBG.svg")));
}


void RackRail::drawRail(const DrawArgs &args) {

}


void RackRail::draw(const DrawArgs &args) {
    const float railHeight = 16;

    // Background color
    nvgBeginPath(args.vg);
    nvgRect(args.vg, 0.0, 0.0, box.size.x, box.size.y);
    nvgFillColor(args.vg, nvgRGBAf(0.19, 0.18, 0.19, 1.0));
    nvgFill(args.vg);

    auto shadowColor = nvgRGBAf(0.0, 0, 0.0, 0.12);
    auto shadowColorRail = nvgRGBAf(0.0, 0., 0.0, 0.34);
    auto transparentColor = nvgRGBAf(0, 0.0, 0, 0.);
    auto shadowRailFactor = 3;
    const auto shadowHeight = 10;
    auto yindex = 0;
    float holeRadius = 4.0;

    for (float y = 0; y < box.size.y; y += RACK_GRID_HEIGHT) {
        const float woodenbgWidth = woodenbg->svg->handle->width;
        const float busBoardWidth = busBoard->svg->handle->width;
        const float busBoardHeight = busBoard->svg->handle->height;
        const float busBoardY = y + (RACK_GRID_HEIGHT - busBoardHeight) / 2;


      /*  for (int x = 0; x < std::ceil(box.size.x / woodenbgWidth); x++) {
            nvgSave(args.vg);
            nvgTranslate(args.vg, x * woodenbgWidth, y);

            woodenbg->draw(args);

            nvgRestore(args.vg);
        }*/

        nvgBeginPath(args.vg);
        nvgRect(args.vg, 0, y, box.size.x, railHeight);
        nvgFillColor(args.vg, nvgRGBAf(0.1, 0.1, 0.1, 1));
        nvgFill(args.vg);

        nvgBeginPath(args.vg);
        nvgRect(args.vg, 0, y + RACK_GRID_HEIGHT - railHeight, box.size.x, railHeight);
        nvgFillColor(args.vg, nvgRGBAf(0.1, 0.1, 0.1, 1));
        nvgFill(args.vg);

        // top rail shadow
        nvgBeginPath(args.vg);
        nvgRect(args.vg, 0, y, box.size.x, railHeight + railHeight * shadowRailFactor);
        nvgFillPaint(args.vg, nvgLinearGradient(args.vg, box.size.x / 2, y, box.size.x / 2, y + railHeight * (shadowRailFactor - 1), shadowColorRail, transparentColor));
        nvgFill(args.vg);

        // bottom rail shadow
        nvgBeginPath(args.vg);
        nvgRect(args.vg, 0, y + RACK_GRID_HEIGHT - railHeight * shadowRailFactor, box.size.x, railHeight * shadowRailFactor);
        nvgFillPaint(args.vg,
                     nvgLinearGradient(args.vg, box.size.x / 2, y + RACK_GRID_HEIGHT, box.size.x / 2, y + RACK_GRID_HEIGHT - railHeight * (shadowRailFactor - 1), shadowColorRail, transparentColor));
        nvgFill(args.vg);


        nvgFillColor(args.vg, nvgRGBAf(0.7, 0.7, 0.7, 1));
        nvgStrokeWidth(args.vg, 1.3);
        nvgStrokeColor(args.vg, nvgRGBAf(0.3, 0.3, 0.3, 1.0));

        // Top rail
        nvgBeginPath(args.vg);
        nvgRect(args.vg, 0, y, box.size.x, railHeight);

        for (float x = 0; x < box.size.x; x += RACK_GRID_WIDTH) {
            nvgCircle(args.vg, x + RACK_GRID_WIDTH / 2, y + railHeight / 2, holeRadius);
            nvgPathWinding(args.vg, NVG_HOLE);
        }

        nvgFill(args.vg);

        nvgBeginPath(args.vg);
        nvgMoveTo(args.vg, 0, y + railHeight - 0.5);
       // nvgLineTo(args.vg, box.size.x, y + railHeight - 0.5);
        nvgStroke(args.vg);

        // Bottom rail
        nvgBeginPath(args.vg);
        nvgRect(args.vg, 0, y + RACK_GRID_HEIGHT - railHeight, box.size.x, railHeight);
        for (float x = 0; x < box.size.x; x += RACK_GRID_WIDTH) {
            nvgCircle(args.vg, x + RACK_GRID_WIDTH / 2, y + RACK_GRID_HEIGHT - railHeight + railHeight / 2, holeRadius);
            nvgPathWinding(args.vg, NVG_HOLE);
        }

        nvgFill(args.vg);

        nvgBeginPath(args.vg);
        nvgMoveTo(args.vg, 0, y + RACK_GRID_HEIGHT - 0.5);
        nvgLineTo(args.vg, box.size.x, y + RACK_GRID_HEIGHT - 0.5);
        nvgStroke(args.vg);











        // Top rail
        /* nvgBeginPath(args.vg);
         nvgFillColor(args.vg, nvgRGBAf(0.1, 0.1, 0.12, 0.7));
         nvgStrokeWidth(args.vg, 2.0);
         nvgStrokeColor(args.vg, nvgRGBAf(0.1, 0.1, 0.1, 1.));

         nvgRect(args.vg, 0, y, box.size.x, railHeight);
         nvgStroke(args.vg);
         nvgFill(args.vg);

         // Bottom rail
         nvgBeginPath(args.vg);
         nvgFillColor(args.vg, nvgRGBAf(0.1, 0.1, 0.12, 0.7));
         nvgStrokeWidth(args.vg, 2.0);
         nvgStrokeColor(args.vg, nvgRGBAf(0.1, 0.1, 0.1, 1.0));

         nvgRect(args.vg, 0, y + RACK_GRID_HEIGHT - railHeight, box.size.x, railHeight);
         nvgStroke(args.vg);
         nvgFill(args.vg);*/

        nvgBeginPath(args.vg);
        nvgRect(args.vg, 0, busBoardY - shadowHeight * 3, box.size.x, busBoardHeight + shadowHeight * 6);
        nvgFillPaint(args.vg, nvgBoxGradient(args.vg, 0, busBoardY - shadowHeight, box.size.x, busBoardHeight + shadowHeight * 2, shadowHeight, 12.5, shadowColor, transparentColor));
        nvgFill(args.vg);

        for (int x = 0; x < std::ceil(box.size.x / busBoardWidth); x++) {
            nvgSave(args.vg);
            nvgTranslate(args.vg, x * busBoardWidth, busBoardY);

            //  if (x % 6 == 0) busBoard2->draw(args);
            //  else busBoard->draw(args);

            /* if (yindex % 2 == 0)
                 busBoard2->draw(args);
             else*/
            busBoard->draw(args);

            nvgRestore(args.vg);
        }

        yindex++;

    }
}


} // namespace app
} // namespace rack
