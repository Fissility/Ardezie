#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "../vendor/lunasvg/lunasvg.h"

#include "rendering/App.hpp"
#include "slate/SlateContext.h"
#include "rendering/font/Fonts.h"


#include "slate/objects/set/ZSet.h"
#include "slate/objects/set/Set.h"
#include "slate/objects/set/NSet.h"
#include "slate/objects/tuple/Tuple.h"
#include <format>
#include "profiling/Profiler.hpp"


int main() {

	Fonts::loadFonts();
	App::renderLoop();

	SlateDefinitions::load();
	SlateContext s;
	ExpressionInfo ei = s.newExpression();
	ei.line = "a b c";
	s.parse();

	return 0;
}