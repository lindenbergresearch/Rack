#pragma once
#include <app/common.hpp>
#include <widget/TransparentWidget.hpp>
#include <widget/SvgWidget.hpp>
#include <widget/FramebufferWidget.hpp>

namespace rack {
namespace app {


struct RackRail : widget::TransparentWidget {
    widget::SvgWidget *busBoard, *busBoard2, *woodenbg;

	RackRail();
	void draw(const DrawArgs& args) override;
	void drawRail(const DrawArgs& args);

};


} // namespace app
} // namespace rack
