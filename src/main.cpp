#include <csignal>
#include <cstdlib>
#include <stdexcept>

#include "window.hpp"

#include <QApplication>  // NOLINT
#include <QMessageBox>   // NOLINT


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
		bgl::SimpleWindow window { "BGL Model Viewer" };
		window.show();
		return app.exec();
	} catch (const std::exception &exception) {
		QMessageBox::critical(nullptr, "Error", exception.what() );
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
