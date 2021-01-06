#include <ui/Slider.hpp>


namespace rack {
namespace ui {


static const float SENSITIVITY = 0.001f;
static const float SLIDER_MARGIN = 5;

Slider::Slider() {
	box.size.y = BND_WIDGET_HEIGHT*0.9+SLIDER_MARGIN*2;
}

void Slider::draw(const DrawArgs& args) {
	BNDwidgetState state = BND_DEFAULT;
	if (APP->event->hoveredWidget == this)
		state = BND_HOVER;
	if (APP->event->draggedWidget == this)
		state = BND_ACTIVE;

	float progress = quantity ? quantity->getScaledValue() : 0.f;
	std::string text = quantity ? quantity->getString() : "";
	bndSlider(args.vg, 0.0, SLIDER_MARGIN, box.size.x, box.size.y-SLIDER_MARGIN*2, BND_CORNER_NONE, state, progress, text.c_str(), NULL, 13);
}

void Slider::onDragStart(const event::DragStart& e) {
	if (e.button != GLFW_MOUSE_BUTTON_LEFT)
		return;

	APP->window->cursorLock();
}

void Slider::onDragMove(const event::DragMove& e) {
	if (quantity) {
		quantity->moveScaledValue(SENSITIVITY * e.mouseDelta.x);
	}
}

void Slider::onDragEnd(const event::DragEnd& e) {
	APP->window->cursorUnlock();
}

void Slider::onDoubleClick(const event::DoubleClick& e) {
	if (quantity)
		quantity->reset();
}


} // namespace ui
} // namespace rack
