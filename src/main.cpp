// Copyright 2021 Bastian Kuolt
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "window.hpp"

#include <QApplication>
#include <QMessageBox>


static void signal_handler(int signal) {
	std::exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	if (argc != 2) {
		QMessageBox::warning(nullptr, "Invalid Usage", "Usage: demo <path-to-model>");
		return EXIT_FAILURE;
	}

	std::signal(SIGINT, signal_handler);
	std::signal(SIGHUP, signal_handler);

	try {
		bgl::SimpleWindow window { "BGL Model Viewer" };
		window.show();
		return app.exec();
	} catch (const std::exception &exception) {
		QMessageBox::critical(nullptr, "Error", exception.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
