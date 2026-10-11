#include "core/app/app.h"

int main() {
	App app;
	if (!app.initialize()) return -1;

	app.run();
}
