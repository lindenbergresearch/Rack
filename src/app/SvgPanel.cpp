#include <app/SvgPanel.hpp>


namespace rack {
namespace app {


void PanelBorder::draw(const DrawArgs& args) {
	NVGcolor borderColor = nvgRGBAf(0.5, 0.5, 0.5, 0.5);
	nvgBeginPath(args.vg);
	nvgRect(args.vg, 0.5, 0.5, box.size.x - 1.0, box.size.y - 1.0);
	nvgStrokeColor(args.vg, borderColor);
	nvgStrokeWidth(args.vg, 1.0);
	nvgStroke(args.vg);

	nvgBeginPath(args.vg);
    nvgRect(args.vg, 0., 0., box.size.x , box.size.y );
    auto paint = nvgLinearGradient(args.vg, box.size.x / 2, 100, box.size.x / 2, box.size.y, nvgRGBAf(1, 1, 1, 0.05), nvgRGBAf(0, 0, 0, 0.4));
    nvgFillPaint(args.vg, paint);
    nvgFill(args.vg);

}


void SvgPanel::step() {
	if (math::isNear(APP->window->pixelRatio, 1.0)) {
		// Small details draw poorly at low DPI, so oversample when drawing to the framebuffer
		oversample = 2.0;
	}
	FramebufferWidget::step();
}

void SvgPanel::setBackground(std::shared_ptr<Svg> svg) {
	widget::SvgWidget* sw = new widget::SvgWidget;
	sw->setSvg(svg);
	addChild(sw);

	// Set size
	box.size = sw->box.size.div(RACK_GRID_SIZE).round().mult(RACK_GRID_SIZE);

	PanelBorder* pb = new PanelBorder;
	pb->box.size = box.size;
	addChild(pb);
}


} // namespace app
} // namespace rack
