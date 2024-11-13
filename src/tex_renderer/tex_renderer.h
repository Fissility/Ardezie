#pragma once
#include <SFML/Graphics.hpp>
#include "renderer/render_formulas.hpp"
#include "../../vendor/lunasvg/lunasvg.h"

using namespace mfl::units_literals;

namespace TeXRenderer {

	struct TeXImageRenderedererResponse {
		sf::Image image;
		std::optional<std::string> error;
	};

	// WORKS ON MY MACHINE !!
	TeXRenderResponse TeX2svg(const std::string& inputTeX,mfl::points fontSize, mfl::pixels width, mfl::pixels height, mfl::pixels xOffset){
		std::vector<std::string> formulas = {
		inputTeX
		};
		return mfl::render_formulas({ .width = width, .height = height,.dpi = 140_dpi,.render_input = false,.input_offset = 0_px,.font_size = fontSize, .columns = {{.initial_offset = height / 1.8, .line_height = 0_px, .x = xOffset}} }, formulas);
	}

	TeXImageRenderedererResponse TeX2Image(const std::string& inputTeX,mfl::points fontSize,mfl::pixels width, mfl::pixels height, mfl::pixels xOffset) {
		TeXRenderResponse response = TeXRenderer::TeX2svg(inputTeX,fontSize,width,height,xOffset);
		sf::Image image;
		if (response.error.has_value()) return { image,response.error };
		auto document = lunasvg::Document::loadFromData(response.image);
		auto bitmap = document->renderToBitmap();
		bitmap.convertToRGBA();
		image.create(bitmap.width(), bitmap.height(), bitmap.data());
		return {image,std::nullopt};
	}
};
