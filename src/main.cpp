// Copyright 2021 Bastian Kuolt
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "gfx/gfx.hpp"
#include "window.hpp"
#include <QApplication>

using namespace bgl;


static void signal_handler(int signal) {
	std::cerr << console_color::red << "\rrequested program termination" << std::endl;
	std::exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cout << console_color::red << "usage: "
				  << console_color::white << "./bgl "
				  << console_color::blue << "<path-to-model>" << std::endl;
		return EXIT_FAILURE;
	}

	std::signal(SIGINT, signal_handler);
	std::signal(SIGHUP, signal_handler);

	try {
		QApplication app(argc, argv);
		SimpleWindow window { "BGL Model Viewer" };
		window.show();
		return app.exec();
	} catch (const std::exception &exception) {
		// TODO(bkuolt): show message box
		std::cout << console_color::red << "error: " << exception.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
