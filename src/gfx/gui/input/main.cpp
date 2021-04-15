// Copyright 2021 Bastian Kuolt
#include <map>
#include <list>
#include <iomanip>
#include <sstream>

#include <QApplication>
#include <QGamepad>
#include <QGamepadManager>
#include <QObject>
#include <QWindow>
#include <QMessageBox>




void button_changed(bool b) {
    
   //      QMessageBox::information(nullptr, "", "sdsdsd");
}

void foo(int index) {
    static QGamepad gamepad(index);

    QObject::connect(&gamepad, &QGamepad::buttonXChanged, button_changed);
}


namespace {


void button_changed(int button, bool pressed) noexcept {

    //
}
//////////////////////////
///
//deviceId


struct {
    std::map<int, QGamepad> gamepads;
} App;

////////////////////////////


void gamepad_changed() noexcept {
    const QList<int> connected_gamepads { QGamepadManager::instance()->connectedGamepads() };

    // add new gamepads
    for (int id : connected_gamepads) {
        App.gamepads.insert(id, QGamePad { id });
        std::ostringstream os;
        os << "Gamepad " << std::quoted(QGamepadManager::instance()->gamepadName(index).toStdString()) << " connected.";
        QMessageBox::information(nullptr, "", os.str().c_str());
    }

    // remove disconnected gamepads
    std::list<int> disconnected_gamepads;
    for (auto [id, gamepad] : App.gamepads) {
        if (!App.gamepads.contains(id)) {
            disconnected_gamepads.push_back(id);
        }
    }
    for (int id : disconnected_gamepads) {
        App.gamepads.erase(id);
        QMessageBox::information(nullptr, "", "Gamepad disconnected.");
    }
}

}  // anonymous namespace


int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QGamepadManager * const gamepad_manager { QGamepadManager::instance() };

    const QList<int> connected_gamepads { gamepad_manager->connectedGamepads() };
    if (connected_gamepads.empty()) {
        QMessageBox::critical(nullptr, "Error", "No gamepad found.");
        return EXIT_FAILURE;
    }

    for (int index : connected_gamepads) {
        known_gamepads.insert(index);
        foo(index);

        std::ostringstream os;
        os << "gamepad " << std::quoted(gamepad_manager->gamepadName(index).toStdString()) << " found.";
        QMessageBox::information(nullptr, "", os.str().c_str());
    }

    QObject::connect(QGamepadManager::instance(), &QGamepadManager::connectedGamepadsChanged, &gamepad_changed);

    QWindow window;
    return app.exec();
}
