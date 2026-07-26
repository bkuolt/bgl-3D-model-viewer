#include "gui/panel.hpp"

#include <format>

namespace bgl {

SettingsPanel::SettingsPanel(QWidget* parent)
    : QFrame(parent), _lightingBox("Lighting"), _boundingBoxBox("Bounding Box") {
    _renderModeBox.addItem("Solid");
    _renderModeBox.addItem("Textured");
    _renderModeBox.addItem("Wireframe");

    _layout.addWidget(new QLabel("Render Mode:", this));
    _layout.addWidget(&_renderModeBox);
    _layout.addWidget(&_lightingBox);
    _layout.addWidget(&_boundingBoxBox);
    _layout.addStretch();
    setLayout(&_layout);
}

RenderMode SettingsPanel::getRenderMode() const noexcept {
    switch (_renderModeBox.currentIndex()) {
        case 1:
            return RenderMode::Textured;
        case 2:
            return RenderMode::Wireframe;
        default:
            return RenderMode::Solid;
    }
}

bool SettingsPanel::isLightingEnabled() const noexcept {
    return _lightingBox.isChecked();
}

bool SettingsPanel::isBoundingBoxEnabled() const noexcept {
    return _boundingBoxBox.isChecked();
}

StatisticsPanel::StatisticsPanel(QWidget* parent)
    : QFrame(parent), _verticesLabel("Vertices: 0"), _trianglesLabel("Triangles: 0"),
      _materialsLabel("Materials: 0") {
    _layout.addWidget(&_trianglesLabel);
    _layout.addWidget(&_verticesLabel);
    _layout.addWidget(&_materialsLabel);
    _layout.addStretch();
    setLayout(&_layout);
}

void StatisticsPanel::updateStatistics(std::size_t vertices, std::size_t triangles,
                                       std::size_t materials) {
    _verticesLabel.setText(QString::fromStdString(std::format("Vertices: {}", vertices)));
    _trianglesLabel.setText(QString::fromStdString(std::format("Triangles: {}", triangles)));
    _materialsLabel.setText(QString::fromStdString(std::format("Materials: {}", materials)));
}

Panel::Panel(QWidget* parent) : QFrame(parent), _statistics(this), _settings(this) {
    _layout.addWidget(&_statistics);
    _layout.addWidget(&_settings);
    _layout.addStretch();
    setLayout(&_layout);
}

}  // namespace bgl
