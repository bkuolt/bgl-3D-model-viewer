#include "window.hpp"

#include <csignal>
#include <cstdlib>
#include <stdexcept>

#include <QApplication>
#include <QMessageBox>


static void signal_handler(int signal) {
	std::exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	if (argc != 2) {
		QMessageBox::critical(nullptr, "Error", "usage: bgl <path-to-model>");
		return EXIT_FAILURE;
	}

	std::signal(SIGINT, signal_handler);
	std::signal(SIGHUP, signal_handler);

	try {
		QApplication app(argc, argv);
		bgl::SimpleWindow window { "BGL 3D Model Viewer" };
		window.show();
		return app.exec();
	} catch (const std::exception &error) {
		QMessageBox::critical(nullptr, "Error", error.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
